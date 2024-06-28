#pragma once

#include"../includes/GLAD/glad.h"
#include"../includes/GLFW/glfw3.h"

#include"../Platforms/OpenGL/OpenGL_Shader.h"
#include"../Platforms/OpenGL/OpenGL_Window.h"
#include"../Platforms/OpenGL/OpenGL_Application.h"
#include"../Platforms/OpenGL/OpenGL_Buffer.h"
#include"../Platforms/OpenGL/OpenGL_TextRenderer.hpp"
#include"../Platforms/OpenGL/OpenGL_Texture.h"
#include"../Platforms/OpenGL/OpenGL_Timer.hpp"

using namespace OpenGL;
using Window = OpenGL_Window;
using Shader = OpenGL_Shader;
using Application = OpenGL_Application;

#define TESTMODE
#include"../Util/Util.cpp"