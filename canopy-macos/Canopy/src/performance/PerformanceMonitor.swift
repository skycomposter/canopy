/// Canonical value used to mark that a frame time was not recorded.
private let invalidFrameTime = -1.0

/// The number of seconds between frames at 60 FPS.
private let secondsPerFrame60FPS = 1.0 / 60

class PerformanceMonitor {
  /// Measured interval between the start of the the two latest frames in seconds.
  var frameInterval: TimeInterval {
    return latestRecordedFrameInterval
  }
  
  // Assume 60 FPS to begin with.
  private var latestRecordedFrameInterval = secondsPerFrame60FPS
  private var latestFrameStartTime = invalidFrameTime
  
  /// Marks the beginning of the current frame. For accuracy, this should be called at the very
  /// start of each frame.
  func onFrameStart() {
    let previousFrameStartTime = latestFrameStartTime
    latestFrameStartTime = ProcessInfo.processInfo.systemUptime
    
    if (previousFrameStartTime != invalidFrameTime) {
      latestRecordedFrameInterval = latestFrameStartTime - previousFrameStartTime
    }
  }
}
