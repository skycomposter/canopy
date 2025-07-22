import SwiftUI

/// A view providing keyboard and mouse input monitoring capabilities.
struct InputMonitoringView: NSViewRepresentable {
    @EnvironmentObject var engineWrapper: CanopyEngineWrapper
    @EnvironmentObject var eventLogger: EventLogger

    func makeNSView(context: Context) -> some NSView {
        return InputMonitoringNSView(
            frame: .zero,
            engine: engineWrapper.engine,
            eventLogger: eventLogger,
        )
    }
    
    func updateNSView(_ nsView: NSViewType, context: Context) {
        // No updates needed for this simple event monitor.
    }
}

/// The internal view that does the actual input monitoring and logging. This should never be used
/// directly, but only using the wrapper above.
class InputMonitoringNSView: NSView {
    private weak var engine: CanopyEngineBridge?
    private weak var eventLogger: EventLogger?
    // Used to capture mouse movements within the frame.
    private var trackingArea: NSTrackingArea?
    
    // By default, macOS uses the bottom-left corner as the origin of its coordinate space.
    // Overriding this field allows us to use the top-left, comforming to the project-wide
    // default.
    override var isFlipped: Bool {
            return true
        }

    init(frame frameRect: NSRect, engine: CanopyEngineBridge, eventLogger: EventLogger) {
        self.engine = engine
        self.eventLogger = eventLogger

        super.init(frame: frameRect)

        // Ensure the view can become first responder to receive key events.
        self.wantsLayer = true
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    // MARK: - Responder Chain and Focus

    /// Allows this view to become the first responder, which is necessary for receiving key events.
    override var acceptsFirstResponder: Bool { true }
    
    // MARK: - Tracking Area
    
    override func updateTrackingAreas() {
        super.updateTrackingAreas()
        
        // Any previously created tracking area needs to be removed before a new one is created.
        if let trackingArea = trackingArea {
            removeTrackingArea(trackingArea)
        }

        // Create a new tracking area that covers the entire view's bounds.
        let newTrackingArea =
            NSTrackingArea(
                rect: bounds,
                options: [.activeAlways, .mouseMoved],
                owner: self,
                userInfo: nil,
            )
        addTrackingArea(newTrackingArea)
        self.trackingArea = newTrackingArea
    }

    // MARK: - Keyboard Events

    override func keyDown(with event: NSEvent) {
        let characters = event.charactersIgnoringModifiers ?? ""
        let keyCode = event.keyCode
        let modifiers = event.modifierFlags
        eventLogger?.log(
            "Key Down: '\(characters)' (keyCode: \(keyCode), modifiers: \(modifiers))"
        )
        super.keyDown(with: event)
    }

    override func keyUp(with event: NSEvent) {
        let characters = event.charactersIgnoringModifiers ?? ""
        let keyCode = event.keyCode
        let modifiers = event.modifierFlags
        eventLogger?.log(
            "Key Up: '\(characters)' (keyCode: \(keyCode), modifiers: \(modifiers))"
        )
        super.keyUp(with: event)
    }
    

    override func flagsChanged(with event: NSEvent) {
        let modifiers = event.modifierFlags
        eventLogger?.log("Flags Changed: \(modifiers)")
        super.flagsChanged(with: event)
    }

    // MARK: - Mouse Events

    override func mouseDown(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Down (Left): x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y)), clicks=\(event.clickCount)"
        )
        super.mouseDown(with: event)
    }

    override func mouseUp(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Up (Left): x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.mouseUp(with: event)
    }

    override func mouseDragged(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Dragged: x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.mouseDragged(with: event)
    }

    override func rightMouseDown(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Down (Right): x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.rightMouseDown(with: event)
    }

    override func rightMouseUp(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Up (Right): x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.rightMouseUp(with: event)
    }

    override func otherMouseDown(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Down (Other Button \(event.buttonNumber)): " +
                "x=\(String(format: "%.1f", location.x)), y=\(String(format: "%.1f", location.y))"
        )
        super.otherMouseDown(with: event)
    }

    override func otherMouseUp(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        eventLogger?.log(
            "Mouse Up (Other Button \(event.buttonNumber)): " +
                "x=\(String(format: "%.1f", location.x)), y=\(String(format: "%.1f", location.y))"
        )
        super.otherMouseUp(with: event)
    }

    override func scrollWheel(with event: NSEvent) {
        eventLogger?.log(
            "Scroll Wheel: deltaX=\(String(format: "%.2f", event.deltaX)), " +
                "deltaY=\(String(format: "%.2f", event.deltaY)), " +
                "deltaZ=\(String(format: "%.2f", event.deltaZ))"
        )
        super.scrollWheel(with: event)
    }
    
    override func mouseMoved(with event: NSEvent) {
        let location = convert(event.locationInWindow, from: nil)
        
        var inputEvent = InputEvent()
        inputEvent.type = POINTER_MOVED
        inputEvent.position = getScaledPosition(location)
        engine?.onInputEvent(inputEvent)
        
        eventLogger?.log(
            "Mouse Moved: x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.mouseMoved(with: event)
    }
    
    /// Takes a window position in points and turns it into its equivalent in pixels.
    private func getScaledPosition(_ position: NSPoint) -> Point2D {
        var scaleFactor = 1.0
        if let window = self.window {
            scaleFactor = window.backingScaleFactor
        }
        
        var scaledPosition = Point2D()
        scaledPosition.x = position.x * scaleFactor
        scaledPosition.y = position.y * scaleFactor
        
        return scaledPosition
    }
}
