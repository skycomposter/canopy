import SwiftUI

struct ContentView: View {
    @StateObject private var eventLogger: EventLogger
    
    private var windowEventMonitor: WindowEventMonitor
    
    init() {
        let logger = EventLogger()
        self._eventLogger = StateObject(wrappedValue :logger)
        self.windowEventMonitor = WindowEventMonitor(eventLogger: logger)
    }
    
    var body: some View {
        ZStack {
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
        .background(
            GeometryReader { proxy in
                Color.clear
                    .onAppear {
                        eventLogger.log(
                            "Window Appeared: width=\(String(format: "%.1f", proxy.size.width)), " +
                                "height=\(String(format: "%.1f", proxy.size.height))"
                        )

                    }
                    .onChange(of: proxy.size) { oldSize, newSize in
                        eventLogger.log(
                            "Window Resized: width=\(String(format: "%.1f", proxy.size.width)), " +
                                "height=\(String(format: "%.1f", proxy.size.height))"
                        )
                    }
            }
        )
        .environmentObject(eventLogger)
    }
}
