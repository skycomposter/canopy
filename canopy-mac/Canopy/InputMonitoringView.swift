import SwiftUI

struct InputMonitoringView: NSViewRepresentable {
    @EnvironmentObject var eventLogger: EventLogger

    func makeNSView(context: Context) -> some NSView {
        return InputMonitoringNSView(
            frame: .zero,
            eventLogger: eventLogger,
        )
    }
    
    func updateNSView(_ nsView: NSViewType, context: Context) {
        // No updates needed for this simple event monitor.
    }

}
