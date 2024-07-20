#!/bin/bash
[ ! -d "./exe" ] && mkdir exe

# Files in src folder inside external (indirect)
SRC_FILES=$(find ./src/external -type f -name "*.cpp" | sort -u | tr '\n' ' ')
# ALL_FILES=$(find . -type f -name "*.cpp" -o -name "*.h" | sort -u | tr '\n' ' ')
INCLUDE_FOLDER= ``

# SRC_FILE=$(ls ./src/external/imgui/ | grep \.cpp)
# echo "$SRC_FILES"
# echo "$ALL_FILES"
# exit 0
g++ --std=c++14 ./src/glad.cpp ./src/main.cpp $SRC_FILES -Wall -I ./src -I ./src/components -I ./src/external/imgui/backend/ -I ./src/external/imgui/ -I ./includes -L. ./lib/mac/libglfw.3.3.dylib -o ./exe/test -framework OpenGL
