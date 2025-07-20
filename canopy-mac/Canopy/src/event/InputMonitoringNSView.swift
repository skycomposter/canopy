import SwiftUI

class InputMonitoringNSView: NSView {
    private weak var eventLogger: EventLogger?
    private var trackingArea: NSTrackingArea?

    init(frame frameRect: NSRect, eventLogger: EventLogger) {
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
        super.keyDown(with: event) // Pass event up the responder chain if needed
    }

    override func keyUp(with event: NSEvent) {
        let characters = event.charactersIgnoringModifiers ?? ""
        let keyCode = event.keyCode
        let modifiers = event.modifierFlags
        eventLogger?.log(
            "Key Up: '\(characters)' (keyCode: \(keyCode), modifiers: \(modifiers))"
        )
        super.keyUp(with: event) // Pass event up the responder chain if needed
    }
    

    override func flagsChanged(with event: NSEvent) {
        let modifiers = event.modifierFlags
        eventLogger?.log("Flags Changed: \(modifiers)")
        super.flagsChanged(with: event) // Pass event up the responder chain if needed
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
        eventLogger?.log(
            "Mouse Moved: x=\(String(format: "%.1f", location.x)), " +
                "y=\(String(format: "%.1f", location.y))"
        )
        super.mouseMoved(with: event)
    }
}
