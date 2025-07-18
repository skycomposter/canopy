import SwiftUI

class WindowEventMonitor: ObservableObject {
    private var eventLogger: EventLogger
    
    init(eventLogger: EventLogger) {
        self.eventLogger = eventLogger

        // Window closing.
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(handleWindowWillClose),
            name: NSWindow.willCloseNotification,
            object: nil,
        )
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    @objc private func handleWindowWillClose(_ notification: Notification) {
        if let window = notification.object as? NSWindow {
            eventLogger.log("Window Will Close: \(window.title)")
        }
    }
}
