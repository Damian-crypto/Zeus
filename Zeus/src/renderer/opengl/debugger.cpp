#include "corepch.h"
#include "debugger.h"
#include "util/logger.h"

#include <glad/gl.h>

bool Validate()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string msg{};
		switch (error)
		{
		case GL_INVALID_ENUM:
			msg = "Runtime Error: Invalid enum provided!";
			break;
		case GL_INVALID_OPERATION:
			msg = "Runtime Error: Invalid operation done!";
			break;
		case GL_STACK_OVERFLOW:
			msg = "Runtime Error: Stack overflow happened!";
			break;
		case GL_STACK_UNDERFLOW:
			msg = "Runtime Error: Stack undeflow happened!";
			break;
		case GL_OUT_OF_MEMORY:
			msg = "Runtime Error: Memory limit exceeded!";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			msg = "Runtime Error: Invalid framebuffer operation!";
			break;
		case GL_CONTEXT_LOST:
			msg = "Runtime Error: OpenGL context lost!";
			break;
		default:
			msg = "Runtime Error: Unknown error!";
			break;
		}

		std::cerr << msg << std::endl;
		return false;
	}

	return true;
}

void Check(const char* function, const char* file, int line)
{
	if (!Validate())
	{
		std::string msg = "OpenGL Error: Found at " + std::string(file) + "[" + std::to_string(line) + "] -> " + std::string(function);
		LOG_ENGINE(Error, msg.c_str());
	}
}
