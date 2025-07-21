#import "CanopyEngineBridge.h"
#import "canopy_engine.h"

@implementation CanopyEngineBridge {
    /** The internal engine instance that fulfills the requests received by this bridge. */
    @private
    CanopyEngine engine;
}

- (void)setBufferPointer:(void*)pixelBuffer withSize:(size_t)size {
    engine.setBufferPointer(pixelBuffer, size);
}

- (void)renderFrameWithWidth:(int)width andHeight:(int)height {
    engine.renderFrame(width, height);
}

@end
