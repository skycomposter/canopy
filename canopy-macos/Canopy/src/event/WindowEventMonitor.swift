import SwiftUI

/// A class responsible for listening for and responding to window-related events.
class WindowEventMonitor: ObservableObject {
  private var eventLogger: EventLogger
  
  init(eventLogger: EventLogger) {
    self.eventLogger = eventLogger

    NotificationCenter.default.addObserver(
      self,
      selector: #selector(handleWindowWillClose),
      name: NSWindow.willCloseNotification,
      object: nil)
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

/// A view modifier with the responsibility of capturing and responding to changes in view size.
///
/// Assumes that the view is full size, and treats the changes in its dimensions as changes to the
/// window itself.
struct ResizeMonitor: ViewModifier {
  private var eventLogger: EventLogger
  
  init(eventLogger: EventLogger) {
    self.eventLogger = eventLogger
  }
      
  func body(content: Content) -> some View {
    content
      .background(
        GeometryReader { proxy in
          Color.clear
            .onAppear {
              eventLogger.log(
                "Window Appeared: width=\(String(format: "%.1f", proxy.size.width)), " +
                  "height=\(String(format: "%.1f", proxy.size.height))")
            }
            .onChange(of: proxy.size) { oldSize, newSize in
              eventLogger.log(
                "Window Resized: width=\(String(format: "%.1f", proxy.size.width)), " +
                  "height=\(String(format: "%.1f", proxy.size.height))")
            }
        })
  }
}

extension View {
  /// Simple extension to add a resize monitoring to a view.
  func resizeMonitor(eventLogger: EventLogger) -> some View {
    self.modifier(ResizeMonitor(eventLogger: eventLogger))
  }
}
