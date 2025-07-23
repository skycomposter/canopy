import MetalKit
import SwiftUI

/// A view whose content is rendered using Metal.
struct MetalView: NSViewRepresentable {
    @EnvironmentObject var engineWrapper: CanopyEngineWrapper
    
    func makeCoordinator() -> MetalRenderer {
        MetalRenderer(engineWrapper.engine)
    }

    func makeNSView(context: Context) -> MTKView {
        let mtkView = MTKView()
        // The delegate is the renderer just created as the coordinator.
        mtkView.delegate = context.coordinator
        // The device is provided by the renderer.
        mtkView.device = context.coordinator.device
        mtkView.preferredFramesPerSecond = 60
        // Format is RGBA (from most to least significant byte), normalized.
        mtkView.colorPixelFormat = .rgba8Unorm
        return mtkView
    }

    func updateNSView(_ nsView: MTKView, context: Context) {
        // TODO: do something here if needed.
    }
}
