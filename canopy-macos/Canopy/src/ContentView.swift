import SwiftUI

struct ContentView: View {
  @StateObject private var engineWrapper = CanopyEngineWrapper()
  @StateObject private var eventLogger: EventLogger

  private var windowEventMonitor: WindowEventMonitor
  
  init() {
    let logger = EventLogger()
    self._eventLogger = StateObject(wrappedValue :logger)
    self.windowEventMonitor = WindowEventMonitor(eventLogger: logger)
  }
  
  var body: some View {
    ZStack(alignment: .topLeading) {
      // The bottom layer is a fullscreen view that displays the rendered frames.
      MetalView()
        .frame(maxWidth: .infinity, maxHeight: .infinity)
      
      // (temporary) View printing all events as they happen.
      ScrollView {
        ScrollViewReader { proxy in
          VStack(alignment: .leading) {
            ForEach(eventLogger.events.indices, id: \.self) { index in
              Text(eventLogger.events[index])
                .font(.footnote)
                .id(index)
            }
          }
          .onChange(of: eventLogger.events.count) {
            // Scroll to the bottom when a new event is added.
            if let lastIndex = eventLogger.events.indices.last {
              proxy.scrollTo(lastIndex, anchor: .bottom)
            }
          }
        }
      }
      .frame(maxHeight: 200)

      // The top layer is transparent and only serves to capture input.
      InputMonitoringView()
        .onAppear {
          DispatchQueue.main.async {
            NSApp.keyWindow?.makeFirstResponder(nil)
            if let nsView =
              NSApp.keyWindow?.contentView?.subviews
                .first(where: { $0 is InputMonitoringNSView }) {
              NSApp.keyWindow?.makeFirstResponder(nsView)
            }
          }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
    }
    .resizeMonitor(eventLogger: eventLogger)
    .environmentObject(engineWrapper)
    .environmentObject(eventLogger)
  }
}
