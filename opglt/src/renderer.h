#pragma once
#include<GL/glew.h>
#include <iostream>

#define uint32 unsigned int
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__))
void GLClearError();
bool GLLogCall(const char* function, const char* filename, int line);

