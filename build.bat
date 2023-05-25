if not exist bin mkdir bin
g++ editor\src\*.cpp -o bin\editor -std=c++14 -O2 -Ideps\include -Ldeps\lib\mingw -llua54 -lenet -lws2_32 -lraylib -lopengl32 -lgdi32 -lwinmm
g++ runtime\src\*.cpp -o bin\runtime -std=c++14 -O2 -Ideps\include -Ldeps\lib\mingw -llua54 -lenet -lws2_32 -lraylib -lopengl32 -lgdi32 -lwinmm
.\bin\editor.exe
