import SwiftUI
import MetalKit

struct MetalView: NSViewRepresentable {
    func makeCoordinator() -> MetalRenderer {
        MetalRenderer()
    }

    func makeNSView(context: Context) -> MTKView {
        let mtkView = MTKView()
        // Assign the device from the coordinator (which was just created).
        mtkView.device = context.coordinator.device
        mtkView.delegate = context.coordinator
        mtkView.preferredFramesPerSecond = 60
        mtkView.colorPixelFormat = .rgba8Unorm // Set the pixel format here
        // No need to set framebufferOnly = false unless you explicitly want to read back pixels
        // from the drawable for CPU processing. For general rendering, true (default) is fine and
        // often better for performance.
        // mtkView.framebufferOnly = false
        return mtkView
    }

    func updateNSView(_ nsView: MTKView, context: Context) {
        // TODO: do something here if needed.
    }
}
