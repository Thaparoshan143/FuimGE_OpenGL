#!/bin/bash
[ ! -d "./exe" ] && mkdir exe

# Files in src folder inside external (indirect)
IMGUI_FILES=$(find ./src/external -type f -name "*.o" | sort -u | tr '\n' ' ')
# IMGUI_FILES=$(find ./src/external -type f -name "*.cpp" | sort -u | tr '\n' ' ')
# ALL_FILES=$(find . -type f -name "*.cpp" -o -name "*.h" | sort -u | tr '\n' ' ')

g++ --std=c++14 ./src/glad.cpp ./src/main.cpp $IMGUI_FILES -Wall -I ./src -I ./src/components -I ./src/external/imgui/backend/ -I ./src/external/imgui/ -I ./includes -L. ./lib/mac/libglfw.3.3.dylib -o ./exe/test -framework OpenGL
