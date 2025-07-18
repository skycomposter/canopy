import SwiftUI

struct EventMonitoringView: NSViewRepresentable {
    @EnvironmentObject var eventLogger: EventLogger

    func makeNSView(context: Context) -> some NSView {
        return EventMonitoringNSView(
            frame: .zero,
            eventLogger: eventLogger,
        )
    }
    
    func updateNSView(_ nsView: NSViewType, context: Context) {
        // No updates needed for this simple event monitor.
    }

}
