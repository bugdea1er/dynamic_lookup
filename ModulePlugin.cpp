#include "AbstractPlugin.hpp"
#include <iostream>

class ModulePlugin final : public AbstractPlugin {
public:
  void run() override {
    preface();
    std::cout << "Bye world" << std::endl;
  }
};

extern "C" {
AbstractPlugin* createPlugin() { return new ModulePlugin(); }
}
