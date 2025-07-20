import SwiftUI
import MetalKit

class MetalRenderer: NSObject, MTKViewDelegate {
    let device: MTLDevice
    let commandQueue: MTLCommandQueue

    var pixelBuffer: MTLBuffer?
    var renderTexture: MTLTexture?

    var pipelineState: MTLRenderPipelineState!
    var vertexBuffer: MTLBuffer!
    
    override init() {
        guard let defaultDevice = MTLCreateSystemDefaultDevice() else {
            fatalError("Metal is not supported on this device")
        }

        self.device = defaultDevice
        self.commandQueue = defaultDevice.makeCommandQueue()!

        super.init()
            
        setUpMetal()
    }
    
    func setUpMetal() {
        // Create a simple vertex buffer for a quad.
        let vertices: [Float] = [
            -1.0, -1.0, 0.0, 1.0,
             1.0, -1.0, 1.0, 1.0,
            -1.0,  1.0, 0.0, 0.0,
             1.0,  1.0, 1.0, 0.0,
        ]
        vertexBuffer =
            device.makeBuffer(
                bytes: vertices,
                length: vertices.count * MemoryLayout<Float>.stride,
                options: [],
            )

        // Create a simple shader library and pipeline state
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
    
    func setupBuffers(width: Int, height: Int) {
        // 0-sized buffers can happen if the view has not been created yet.
        // This would cause buffer creation to crash.
        guard width > 0 && height > 0 else { return }
        
        let bufferLength = width * height * 4
        
        // Allocate an MTLBuffer that can be shared with the CPU.
        pixelBuffer = device.makeBuffer(length: bufferLength, options: .storageModeShared)
        
        let textureDescriptor =
            MTLTextureDescriptor.texture2DDescriptor(
                pixelFormat: .rgba8Unorm,
                width: width,
                height: height,
                mipmapped: false,
            )
        // Readable by the GPU for rendering.
        textureDescriptor.usage = [.shaderRead, .renderTarget]
        
        renderTexture = device.makeTexture(descriptor: textureDescriptor)
    }
    
    // Called when the view changes size.
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        setupBuffers(width: Int(size.width), height: Int(size.height))
    }

    // Called for each frame.
    func draw(in view: MTKView) {
        guard let drawable = view.currentDrawable,
              let renderPassDescriptor = view.currentRenderPassDescriptor else {
            return
        }

        // 1. Render the frame into the shared pixelBuffer.
        // Engine should do this, but for now we simulate it locally.
        renderGameFrame(Int(view.drawableSize.width), Int(view.drawableSize.height))
        
        // 2. Create a command buffer.
        let commandBuffer = commandQueue.makeCommandBuffer()!
        
        // 3. Blit command to copy data from pixelBuffer to renderTexture.
        if let pixelBuffer = pixelBuffer, let renderTexture = renderTexture {
            let region = MTLRegionMake2D(0, 0, renderTexture.width, renderTexture.height)
            let blitEncoder = commandBuffer.makeBlitCommandEncoder()!
            blitEncoder.copy(
                from: pixelBuffer,
                sourceOffset: 0,
                sourceBytesPerRow: renderTexture.width * 4,
                sourceBytesPerImage: 0,
                sourceSize: MTLSize(width: renderTexture.width, height: renderTexture.height, depth: 1),
                to: renderTexture,
                destinationSlice: 0,
                destinationLevel: 0,
                destinationOrigin: region.origin,
            )
            blitEncoder.endEncoding()
        }

        // 4. Render the texture to the screen
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0.0, green: 0.0, blue: 0.0, alpha: 1.0)
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].storeAction = .store
        
        let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
        renderEncoder.setRenderPipelineState(pipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.setFragmentTexture(renderTexture, index: 0) // Pass our rendered texture to the fragment shader
        renderEncoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4)
        renderEncoder.endEncoding()

        // 5. Present the drawable to the screen
        commandBuffer.present(drawable)
        commandBuffer.commit()
    }
    
    private func renderGameFrame(_ width: Int, _ height: Int) {
        // Get the raw pointer to the buffer's CPU-accessible memory
        guard let baseAddress = pixelBuffer?.contents() else {
            print("Failed to get contents of pixelBuffer.")
            return
        }

        let bytesPerPixel = MemoryLayout<UInt32>.size
        let bytesPerRow = width * bytesPerPixel

        for y in 0..<height {
            for x in 0..<width {
                let r = UInt32(x * 255 / (width - 1))
                let g = UInt32(y * 255 / (height - 1))
                let b = UInt32((x + y) * 255 / (height + width))
                let a: UInt32 = 255

                let color: UInt32 = (a << 24) + (b << 16) + (g << 8) + r

                let byteOffset = (y * bytesPerRow) + (x * bytesPerPixel)
                baseAddress.storeBytes(of: color, toByteOffset: byteOffset, as: UInt32.self)
            }
        }
    }
}
