import SwiftUI

struct ContentView: View {
    @StateObject private var inputEventLogger = InputEventLogger()
    
    var body: some View {
        ZStack {
            ScrollView {
                ScrollViewReader { proxy in
                    VStack(alignment: .leading) {
                        ForEach(inputEventLogger.events.indices, id: \.self) { index in
                            Text(inputEventLogger.events[index])
                                .font(.footnote)
                                .id(index)
                        }
                    }
                    .onChange(of: inputEventLogger.events.count) {
                        // Scroll to the bottom when a new event is added.
                        if let lastIndex = inputEventLogger.events.indices.last {
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
        .environmentObject(inputEventLogger)
    }
}
