
CXX = g++ --std=c++11
CFLAGS = -Wall
BUILD_NAME = ./Exe/test
OBJ_PATH = .
INC_PATH = -I./includes
LINKER_MAC = -L. .././includes/lib/mac/libglfw.3.3.dylib
LINKER_WIN = -L. .././includes/lib/win/libglfw3dll.a
DIR_OFF = .././src/
ASSEMBLE_CMD = 	${CXX} ${CFLAGS} -c
	
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "MAC OS"
endif

ifeq ($(OS), Windows_NT) #WINDOWS
	ECHO_MESSAGE = "Windows"
endif

FILE_DEPENDENCIES = main.obj glad.obj Shader.obj Window.obj

# Short hand for combining all the object file into one executable form
WIN_COMMAND = ${CXX} ${CFLAGS} -o ${BUILD_NAME} ${OBJ_PATH}/*.obj ${INC_PATH} ${LINKER_WIN}
MAC_COMMAND = ${CXX} ${CFLAGS} -o ${BUILD_NAME} ${OBJ_PATH}/*.obj ${INC_PATH} ${LINKER_MAC}

build_m: ${FILE_DEPENDENCIES}
ifeq (${UNAME_S}, Darwin)
	${MAC_COMMAND}
else
	$(WIN_COMMAND)
endif

main.obj: ${DIR_OFF}main.cpp
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/main.obj ${DIR_OFF}main.cpp

glad.obj: ${DIR_OFF}glad.cpp
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/glad.obj ${DIR_OFF}glad.cpp

Shader.obj: ${DIR_OFF}Helpers/Shader.cpp ${DIR_OFF}Helpers/Shader.h
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/Shader.obj ${DIR_OFF}Helpers/Shader.cpp

Window.obj: ${DIR_OFF}Helpers/Window.cpp ${DIR_OFF}Helpers/Window.h
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/Window.obj ${DIR_OFF}Helpers/Window.cpp

clean:
ifeq (${UNAME_S}, Darwin)
	rm -f *.obj
else
	del -f *.obj
endif