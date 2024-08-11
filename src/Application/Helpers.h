#pragma once
#include <GL/gl.h>

#include <iostream>

namespace Helpers
{
	inline bool ShaderCompileStatus(GLuint shader)
	{
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE) {
			std::cout << "Shader compilation error: " << std::endl;
			GLint logSize = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			std::vector<char> log(logSize);
			glGetShaderInfoLog(shader, logSize, &logSize, &log[0]);

			std::cout << log.data() << std::endl;
			return false;
		}

		return true;
	}

	inline bool ProgramCompileStatus(GLuint program)
	{
		GLint success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success != GL_TRUE) {
			std::cout << "Program compilation error: " << std::endl;
			GLint logSize = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			std::vector<char> log(logSize);
			glGetProgramInfoLog(program, logSize, &logSize, &log[0]);

			std::cout << log.data() << std::endl;
			return false;
		}

		return true;
	}
}
