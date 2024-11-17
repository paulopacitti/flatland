CC=clang++
INCLUDE_FLAGS=-I"./libs"
LINKER_FLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
COMPILER_FLAGS=-Wall -std=c++17
DEBUG_FLAGS=-g

all: clean build run

build:
	@mkdir -p build
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_FLAGS) src/*.cpp $(LINKER_FLAGS) -o build/flatland

debug: clean
	@mkdir -p build
	$(CC) $(COMPILER_FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) src/*.cpp $(LINKER_FLAGS) -o build/flatland

clean:
	@if [ -d build ]; then rm -r build; fi

run:
	@./build/flatland

