#pragma once

#include"../includes/GLAD/glad.h"
#include"../includes/GLFW/glfw3.h"

#include"../Platforms/OpenGL/OpenGL_Sha.h"
#include"../Platforms/OpenGL/OpenGL_Win.h"
#include"../Platforms/OpenGL/OpenGL_App.h"
#include"../Platforms/OpenGL/OpenGL_Buff.h"

using namespace OpenGL;
using Window = OpenGL_Win;
using Shader = OpenGL_Sha;
using Application = OpenGL_App;