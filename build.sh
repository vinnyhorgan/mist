mkdir -p bin
clang++ -std=c++14 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I deps/include runtime/src/main.cpp deps/lib/libraylib.a -o bin/runtime
./bin/runtime