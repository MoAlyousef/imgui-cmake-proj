# imgui-cmake-proj
CMake project example for imgui using statically-linked SDL2 and OpenGL, works with both emscripten and native.

For native:
```
git clone https://github.com/MoAlyousef/imgui-cmake-proj
cd imgui-cmake-proj
cmake -B bin
cmake --build bin --parallel
./bin/myapp
```

For emscripten:
```
git clone https://github.com/MoAlyousef/imgui-cmake-proj
cd imgui-cmake-proj
emcmake cmake -B bin
cmake --build bin --parallel
emrun bin/myapp.html
```
