/// A simple wrapper that allows the engine bridge to be made an environment object.
class CanopyEngineWrapper: ObservableObject {
  let engine: CanopyEngineBridge
    
  init() {
    self.engine = CanopyEngineBridge()
  }
}
