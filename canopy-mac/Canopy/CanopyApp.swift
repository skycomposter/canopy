import SwiftUI

@main
struct CanopyApp: App {
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    
    var body: some Scene {
        WindowGroup("Canopy") {
            ContentView()
                .frame(
                    minWidth: 800,
                    idealWidth: 800,
                    minHeight: 600,
                    idealHeight: 600,
                )
        }
    }
}
