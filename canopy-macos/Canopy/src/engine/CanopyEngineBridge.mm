#import "CanopyEngineBridge.h"

#include "canopy_engine.h"
#include "input_event.h"

@implementation CanopyEngineBridge {
    /** The internal engine instance that fulfills the requests received by this bridge. */
    @private
    CanopyEngine engine;
}

- (void)setBufferPointer:(void *)pixelBuffer withSize:(size_t)size {
    engine.SetBufferPointer(pixelBuffer, size);
}

- (void)renderFrameWithWidth:(int)width andHeight:(int)height {
    engine.RenderFrame(width, height);
}

- (void)onInputEvent:(InputEvent)event {
    if (event.type != POINTER_MOVED) {
        // Only accept POINTER_MOVED events for now.
        return;
    }
    
    engine.OnInputEvent(event);
}

@end
