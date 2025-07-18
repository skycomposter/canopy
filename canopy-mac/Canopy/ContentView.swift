import SwiftUI

struct ContentView: View {
    @StateObject private var eventLogger = EventLogger()
    
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

            EventMonitoringView()
                .onAppear {
                    DispatchQueue.main.async {
                        NSApp.keyWindow?.makeFirstResponder(nil)
                        if let nsView =
                            NSApp.keyWindow?.contentView?.subviews
                                .first(where: { $0 is EventMonitoringNSView }) {
                            NSApp.keyWindow?.makeFirstResponder(nsView)
                        }
                    }
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .environmentObject(eventLogger)
    }
}
