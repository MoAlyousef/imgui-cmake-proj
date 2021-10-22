# imgui-cmake-proj
CMake project example for imgui using statically-linked SDL2 and OpenGL, works with both emscripten and native.
Live wasm demo can be found here:
https://moalyousef.github.io/imgui-cmake-proj/


For native:
```
git clone https://github.com/MoAlyousef/imgui-cmake-proj
cd imgui-cmake-proj
cmake -B bin
cmake --build bin --parallel
./bin/calc
```

For emscripten:
```
git clone https://github.com/MoAlyousef/imgui-cmake-proj
cd imgui-cmake-proj
emcmake cmake -B bin
cmake --build bin --parallel
emrun bin/index.html
```

The emscripten generated binary can be run using a server like emrun, python3 -m http.server etc.
