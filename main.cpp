#include "AbstractPlugin.hpp"
#include <dlfcn.h>
#include <filesystem>

int main() {
  auto module = std::filesystem::absolute("libModulePlugin.so");
  auto* handle = dlopen(module.c_str(), RTLD_NOW);
  auto* constructor = (AbstractPlugin*(*)()) dlsym(handle, "createPlugin");
  auto* plugin = constructor();
  plugin->run();

  delete plugin;
  dlclose(handle);
}
