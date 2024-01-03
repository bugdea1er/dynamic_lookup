This example shows different behaviour of cmake project build when building MODULEs on Linux and Mac OS

The example consists of three parts:
- `AbstractPlugin` OBJECT library defines a virtual class AbstractPlugin. It has one defined method `preface` and one pure virtual method `run`. The `preface` method **required** to be defined in the object library 
- `dynamic_lookup` executable links with the AbstractPlugin library and therefore contains a definition for `AbstractPlugin::preface` method
- `ModulePlugin` MODULE library defines an implementation of AbstractPlugin. The resulting `.so` file contains the `run` definition and does not contain the `preface` method definition.

The CMakeLists.txt file describes an option DYNAMIC_LOOKUP (off by default) which adds a `-undefined dynamic_lookup` linker flag for the MODULE target.

`dynamic_lookup` executable lists `preface` symbol as defined:
```shell
$ nm -g build/dynamic_lookup | grep AbstractPlugin.preface | grep T
0000000100003010 T __ZN14AbstractPlugin7prefaceEv
```

and `libModulePlugin` module lists `preface` symbol as undefined:
```shell
$ nm -g build/libModulePlugin.so | grep AbstractPlugin.preface | grep T
                 U __ZN14AbstractPlugin7prefaceEv
```

Linux build ([Linux Build CI](https://github.com/bugdea1er/dynamic_lookup/actions/workflows/LinuxBuild.yml)) completes just fine: no need for extra flags on top of marking the libs as OBJECT and MODULE where needed:
```shell
$ cmake -B build
$ cmake --build build
[ 20%] Building CXX object CMakeFiles/AbstractPlugin.dir/AbstractPlugin.cpp.o
[ 20%] Built target AbstractPlugin
[ 40%] Building CXX object CMakeFiles/ModulePlugin.dir/ModulePlugin.cpp.o
[ 60%] Linking CXX shared module libModulePlugin.so
[ 60%] Built target ModulePlugin
[ 80%] Building CXX object CMakeFiles/dynamic_lookup.dir/main.cpp.o
[100%] Linking CXX executable dynamic_lookup
[100%] Built target dynamic_lookup
```

Mac OS build ([Mac Build Without Flags CI](https://github.com/bugdea1er/dynamic_lookup/actions/workflows/MacBuildWithoutFlags.yml)) fails under the same conditions:
```shell
$ cmake -B build
$ cmake --build build
[ 20%] Building CXX object CMakeFiles/AbstractPlugin.dir/AbstractPlugin.cpp.o
[ 20%] Built target AbstractPlugin
[ 40%] Building CXX object CMakeFiles/ModulePlugin.dir/ModulePlugin.cpp.o
[ 60%] Linking CXX shared module libModulePlugin.so
Undefined symbols for architecture x86_64:
  "AbstractPlugin::preface()", referenced from:
      ModulePlugin::run() in ModulePlugin.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [libModulePlugin.so] Error 1
make[1]: *** [CMakeFiles/ModulePlugin.dir/all] Error 2
make: *** [all] Error 2
```

Mac OS build needs a separate linker flag `-undefined dynamic_lookup` in order to achive the same results ([Mac Build With Flags CI](https://github.com/bugdea1er/dynamic_lookup/actions/workflows/MacBuildWithFlags.yml)):
```shell
$ cmake -B build -DDYNAMIC_LOOKUP=ON
$ cmake --build build
[ 20%] Building CXX object CMakeFiles/AbstractPlugin.dir/AbstractPlugin.cpp.o
[ 20%] Built target AbstractPlugin
[ 40%] Building CXX object CMakeFiles/ModulePlugin.dir/ModulePlugin.cpp.o
[ 60%] Linking CXX shared module libModulePlugin.so
[ 60%] Built target ModulePlugin
[ 80%] Building CXX object CMakeFiles/dynamic_lookup.dir/main.cpp.o
[100%] Linking CXX executable dynamic_lookup
[100%] Built target dynamic_lookup
```
