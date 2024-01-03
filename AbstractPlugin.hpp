#pragma once

class AbstractPlugin {
public:
  /// This symbol must be defined in the module
  virtual void run() = 0;

  virtual ~AbstractPlugin() = default;

protected:
  /// This symbol should not be defined in the module
  void preface();
};
