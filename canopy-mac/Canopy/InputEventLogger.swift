import SwiftUI

class InputEventLogger: ObservableObject {
    @Published var events: [String] = []
    
    init() {
        // Window resizing.
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(handleWindowDidResize),
            name: NSWindow.didResizeNotification,
            object: nil,
        )

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

    func log(_ message: String) {
        DispatchQueue.main.async {
            self.events.append(message)
            // Keep the list from growing indefinitely (keep last 100 events).
            if self.events.count > 100 {
                self.events.removeFirst()
            }
        }
    }
    
    @objc private func handleWindowDidResize(_ notification: Notification) {
        if let window = notification.object as? NSWindow {
            let frame = window.frame
            log(
                "Window Resized: width=\(String(format: "%.1f", frame.width)), " +
                    "height=\(String(format: "%.1f", frame.height))"
            )
        }
    }
    
    @objc private func handleWindowWillClose(_ notification: Notification) {
        if let window = notification.object as? NSWindow {
            log("Window Will Close: \(window.title)")
        }
    }
}
