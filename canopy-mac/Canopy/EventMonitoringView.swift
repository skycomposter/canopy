import SwiftUI

struct EventMonitoringView: NSViewRepresentable {
    @EnvironmentObject var inputEventLogger: InputEventLogger

    func makeNSView(context: Context) -> some NSView {
        return EventMonitoringNSView(
            frame: .zero,
            inputEventLogger: inputEventLogger,
        )
    }
    
    func updateNSView(_ nsView: NSViewType, context: Context) {
        // No updates needed for this simple event monitor.
    }

}
