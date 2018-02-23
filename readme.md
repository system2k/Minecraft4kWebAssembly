An incomplete port of Minecraft4k to WebAssembly.


Compiling:


emcc program.cpp -s WASM=1 -o program.wasm -g -O3 -Os -s SIDE_MODULE=1 -s ONLY_MY_CODE=1 -ffast-math