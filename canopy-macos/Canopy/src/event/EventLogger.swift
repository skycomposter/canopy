import SwiftUI

/// A logger that stores and publishes a series of events for development and debugging purposes.
class EventLogger: ObservableObject {
    @Published var events: [String] = []
    
    private let eventLimit = 100
    
    /// Posts a new event to be recorded.
    ///
    /// - Parameter event: A String representation of the event to be recorded.
    func log(_ event: String) {
        DispatchQueue.main.async {
            self.events.append(event)
            // Keep the list from growing indefinitely.
            if self.events.count > self.eventLimit {
                self.events.removeFirst()
            }
        }
    }

}
