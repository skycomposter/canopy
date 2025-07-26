import MetalKit

/// Size of a single pixel in the frame buffer in bytes.
private let bytesPerPixel = 4

/// A renderer that uses Metal to render frames provided by the engine to a view.
class MetalRenderer: NSObject, MTKViewDelegate {
  /// Represents the GPU.
  let device: MTLDevice
  
  /// Queue used to post commands to the device.
  private let commandQueue: MTLCommandQueue
  
  /// Used to measure performance (e.g delta time) and provide it when needed.
  private let performanceMonitor: PerformanceMonitor
  
  /// The  engine that actually renders each frame to a buffer.
  private weak var engine: CanopyEngineBridge?

  /// The buffer whose underlying data is shared with and written to by the engine.
  private var pixelBuffer: MTLBuffer?
  
  private var pipelineState: MTLRenderPipelineState!
  private var renderTexture: MTLTexture?
  private var vertexBuffer: MTLBuffer!

  init(_ engine: CanopyEngineBridge) {
    // Use the Metal framework to create a representation of the GPU.
    guard let defaultDevice = MTLCreateSystemDefaultDevice() else {
      // This should never happen with modern machines. I think.
      fatalError("Metal is not supported on this device")
    }

    self.device = defaultDevice
    self.commandQueue = defaultDevice.makeCommandQueue()!
    self.performanceMonitor = PerformanceMonitor()
    self.engine = engine

    super.init()
        
    setUpMetal()
  }
  
  private func setUpMetal() {
    // Simple vertex buffer for a quad. We get the prepared pixel values from the internal engine,
    // so this is just a quad for the whole screen.
    let vertices: [Float] = [
      -1.0, -1.0, 0.0, 1.0,
       1.0, -1.0, 1.0, 1.0,
      -1.0,  1.0, 0.0, 0.0,
       1.0,  1.0, 1.0, 0.0,
    ]
    vertexBuffer = device.makeBuffer(
      bytes: vertices,
      length: vertices.count * MemoryLayout<Float>.stride,
      options: [])

    // Create a simple shader library and pipeline state, both needed to draw the frame buffer to
    // the view.
    let library = device.makeDefaultLibrary()!
    let vertexFunction = library.makeFunction(name: "vertexShader")
    let fragmentFunction = library.makeFunction(name: "fragmentShader")
    let pipelineDescriptor = MTLRenderPipelineDescriptor()
    pipelineDescriptor.vertexFunction = vertexFunction
    pipelineDescriptor.fragmentFunction = fragmentFunction
    pipelineDescriptor.colorAttachments[0].pixelFormat = .rgba8Unorm

    do {
      pipelineState = try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
    } catch {
      fatalError("Failed to create pipeline state: \(error)")
    }
  }
  
  /// Prepares or updates the shared frame buffer and render texture with the given width and height.
  private func setupBuffers(width: Int, height: Int) {
    // 0-sized buffers can happen if the view has not been created yet. This would cause buffer
    // creation to crash.
    guard width > 0 && height > 0 else { return }
    
    // Each pixel is encoded as 4 bytes.
    let bufferLength = width * height * bytesPerPixel
    
    // Allocate an MTLBuffer that can be shared with the CPU.
    pixelBuffer = device.makeBuffer(length: bufferLength, options: .storageModeShared)
    
    // Pass the raw pointer to the engine so it can write to it.
    if let ptr = pixelBuffer?.contents() {
      engine?.setBufferPointer(ptr, withSize: bufferLength)
    }
    
    let textureDescriptor = MTLTextureDescriptor.texture2DDescriptor(
      pixelFormat: .rgba8Unorm,
      width: width,
      height: height,
      mipmapped: false)
    // Readable by the GPU for rendering.
    textureDescriptor.usage = [.shaderRead, .renderTarget]
    
    renderTexture = device.makeTexture(descriptor: textureDescriptor)
  }
  
  /// Called when the view changes size.
  func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    setupBuffers(width: Int(size.width), height: Int(size.height))
  }

  /// Called for each frame to generate and draw a frame to the view.
  func draw(in view: MTKView) {
    // Make sure that the delta time is up-to-date.
    performanceMonitor.onFrameStart()
    
    // Make sure that the view is set up correctly, or we will crash.
    guard let drawable = view.currentDrawable,
          let renderPassDescriptor = view.currentRenderPassDescriptor else {
      return
    }

    // 1. Render the frame into the shared pixel buffer.
    engine?.renderFrame(
      withWidth: view.drawableSize.width,
      andHeight: view.drawableSize.height,
      deltaTime: performanceMonitor.deltaTime)
    
    // 2. Create a command buffer.
    let commandBuffer = commandQueue.makeCommandBuffer()!
    
    // 3. Copy data from the pixel buffer to the render texture.
    if let pixelBuffer = pixelBuffer, let renderTexture = renderTexture {
        let region = MTLRegionMake2D(0, 0, renderTexture.width, renderTexture.height)
        let blitEncoder = commandBuffer.makeBlitCommandEncoder()!
      blitEncoder.copy(
        from: pixelBuffer,
        sourceOffset: 0,
        sourceBytesPerRow: renderTexture.width * bytesPerPixel,
        sourceBytesPerImage: 0,
        sourceSize: MTLSize(width: renderTexture.width, height: renderTexture.height, depth: 1),
        to: renderTexture,
        destinationSlice: 0,
        destinationLevel: 0,
        destinationOrigin: region.origin)
      blitEncoder.endEncoding()
    }

    // 4. Render the texture to the screen.
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(
      red: 0.0, green: 0.0, blue: 0.0, alpha: 1.0)
    renderPassDescriptor.colorAttachments[0].loadAction = .clear
    renderPassDescriptor.colorAttachments[0].storeAction = .store
    
    let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
    renderEncoder.setRenderPipelineState(pipelineState)
    renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
    // Use the fragment shader to draw the render texture.
    renderEncoder.setFragmentTexture(renderTexture, index: 0)
    renderEncoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4)
    renderEncoder.endEncoding()

    commandBuffer.present(drawable)
    commandBuffer.commit()
  }
}
