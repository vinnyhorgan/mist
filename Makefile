EXE = editor.exe

CC = g++
CFLAGS = -Ideps/include -Wall -std=c++14 -O2 -Wno-unknown-pragmas -Wno-enum-compare -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS = -Ldeps/lib/mingw -llua54 -lenet -lws2_32 -lraylib -lopengl32 -lgdi32 -lwinmm

SRCS = $(wildcard editor/src/*.cpp)
OBJS := $(SRCS:%=bin/%.o)

all: bin/$(EXE) run

bin/$(EXE): $(OBJS)
	$(CC) $(OBJS) -o bin/$(EXE) $(LDFLAGS)

bin/%.cpp.o: %.cpp
	@if not exist bin mkdir bin
	@if not exist bin\editor mkdir bin\editor
	@if not exist bin\editor\src mkdir bin\editor\src

	$(CC) $(CFLAGS) -c $< -o $@

run: bin/$(EXE)
	cd editor && ..\bin\$(EXE)

.PHONY: clean
clean:
	rmdir /s bin
