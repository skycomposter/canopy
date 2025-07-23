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

- (void)renderFrameWithWidth:(int)width
                   andHeight:(int)height
               frameInterval:(double)frameInterval {
    engine.RenderFrame(width, height, frameInterval);
}

- (void)onInputEvent:(InputEvent)event {
    engine.OnInputEvent(event);
}

@end
