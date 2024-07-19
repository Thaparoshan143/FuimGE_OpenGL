#!/bin/bash
[ ! -d "./exe" ] && mkdir exe
g++ --std=c++14 ./src/glad.cpp ./src/main.cpp -Wall -I ./src -I ./src/components -I ./src/external -I ./includes -L. ./lib/mac/libglfw.3.3.dylib -o ./exe/test -framework OpenGL
