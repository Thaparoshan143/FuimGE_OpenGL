CXX = g++ --std=c++11
CFLAGS = -Wall
BUILD_NAME = ./Exe/test
OBJ_PATH = .
INC_PATH = -I .././includes -I .././includes/freetypeinc -I .././includes/freetypeinc/freetype
LINKER_MAC = -L. .././lib/mac/libglfw.3.3.dylib -L. .././lib/mac/libfreetype.6.dylib
LINKER_WIN = -L. .././lib/win/libglfw3dll.a -L. .././lib/win/libfreetype.dll.a
SANDBOX_DIR_OFF = .././Sandbox/
PLATFORM_DIR_OFF = ../Platforms/OpenGL/
ASSEMBLE_CMD = 	${CXX} ${CFLAGS} -c
	
UNAME_S := $(shell uname -s)

FILE_DEPENDENCIES = main.obj glad.obj window.obj application.obj shader.obj buffer.obj

WIN_COMMAND = ${CXX} ${CFLAGS} -o ${BUILD_NAME} ${OBJ_PATH}/*.obj ${INC_PATH} ${LINKER_WIN}
MAC_COMMAND = ${CXX} ${CFLAGS} -o ${BUILD_NAME} ${OBJ_PATH}/*.obj ${INC_PATH} ${LINKER_MAC}

sandbox_clean: clean sandbox

sandbox: ${FILE_DEPENDENCIES}
ifeq (${UNAME_S}, Darwin)
	${MAC_COMMAND}
else
	$(WIN_COMMAND)
endif

main.obj: ${SANDBOX_DIR_OFF}Main.cpp
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/main.obj ${SANDBOX_DIR_OFF}Main.cpp

glad.obj: ${SANDBOX_DIR_OFF}glad.cpp
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/glad.obj ${SANDBOX_DIR_OFF}glad.cpp

window.obj: ${PLATFORM_DIR_OFF}OpenGL_Win.cpp ${PLATFORM_DIR_OFF}OpenGL_Win.h
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/window.obj ${PLATFORM_DIR_OFF}OpenGL_Win.cpp

application.obj: ${PLATFORM_DIR_OFF}OpenGL_App.cpp ${PLATFORM_DIR_OFF}OpenGL_App.h
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/application.obj ${PLATFORM_DIR_OFF}OpenGL_App.cpp

shader.obj: ${PLATFORM_DIR_OFF}OpenGL_Sha.cpp ${PLATFORM_DIR_OFF}OpenGL_Sha.h
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/shader.obj ${PLATFORM_DIR_OFF}OpenGL_Sha.cpp

buffer.obj: ${PLATFORM_DIR_OFF}OpenGL_Buff.cpp ${PLATFORM_DIR_OFF}OpenGL_Buff.cpp
	${ASSEMBLE_CMD} -o ${OBJ_PATH}/buffer.obj ${PLATFORM_DIR_OFF}OpenGL_Buff.cpp

clean:
ifeq (${UNAME_S}, Darwin)
	rm -f *.obj
else
	del -f *.obj
endif