mkdir -p bin
clang++ -std=c++14 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I deps/include editor/src/*.cpp deps/lib/osx/libraylib.a -o bin/editor
clang++ -std=c++14 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -I deps/include runtime/src/*.cpp deps/lib/osx/libraylib.a -o bin/runtime
./bin/editor
