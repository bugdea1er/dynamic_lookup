#include "AbstractPlugin.hpp"
#include <iostream>

void AbstractPlugin::preface() {
  std::cout << "Hello world" << std::endl;
}
