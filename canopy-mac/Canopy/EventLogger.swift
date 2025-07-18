import SwiftUI

class EventLogger: ObservableObject {
    @Published var events: [String] = []
    
    func log(_ message: String) {
        DispatchQueue.main.async {
            self.events.append(message)
            // Keep the list from growing indefinitely (keep last 100 events).
            if self.events.count > 100 {
                self.events.removeFirst()
            }
        }
    }
}
