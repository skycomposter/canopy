#import <Foundation/Foundation.h>

/** A bridge class allowing us access to the engine's C++ APIs from this SwiftUI project. */
@interface CanopyEngineBridge : NSObject

/**
 * Assigns a valid buffer to the engine that it will use to render the game frames.
 *
 * @param pixelBuffer A pointer to the shared buffer.
 * @param size The allocated size of the buffer.
 */
- (void)setBufferPointer:(void*)pixelBuffer withSize:(size_t)size;

/**
 * Renders the next frame to the previously assigned buffer.
 *
 * @param width The width of the frame in pixels.
 * @param height The height of the frame in pixels.
 */
- (void)renderFrameWithWidth:(int)width andHeight:(int)height;

@end
