#pragma once

#include <GL/glew.h>
#include <gl3/ShaderSource.h>
#include <iostream>
#include <cassert>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// A compiled shader class
	/// </summary>
	class Shader
	{
	private:
		ShaderType m_type;
		GLuint m_shaderId;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Shader"/> class from a source code.
		/// Throws an exception if the shader source does not compile.
		/// </summary>
		/// <param name="source">The source code of the shader.</param>
		/// <param name="type">The type of the shader.</param>
		Shader(const ShaderSource & source, ShaderType type)
		{
			m_type = type;
			m_shaderId = glCreateShader(static_cast<GLenum>(m_type));
			::std::string program = source.toString();
			const char * str = program.c_str();
			glShaderSource(m_shaderId, 1, &str, NULL);
			glCompileShader(m_shaderId);
			// Check the compilation status and report any errors
			GLint shaderStatus;
			glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &shaderStatus);
			// If the shader failed to compile, display the info log and throw a runtime_error exception
			if (shaderStatus == GL_FALSE)
			{
				GLint infoLogLength;
				glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *strInfoLog = new GLchar[infoLogLength + 1];
				glGetShaderInfoLog(m_shaderId, infoLogLength, NULL, strInfoLog);
				std::cerr << "Shader: shader compilation failed. " << std::endl << strInfoLog << std::endl;
				delete[] strInfoLog;
				glDeleteShader(m_shaderId);
				m_shaderId = 0;
				throw std::runtime_error("Shader: failed to compile source code.");
			}
		}

		/// <summary>
		/// Default constructor. The created buffer is invalid.
		/// </summary>
		Shader() : m_shaderId(0) {}

		~Shader()
		{
			if (m_shaderId != 0) { glDeleteShader(m_shaderId); }
		}

		Shader & operator=(const Shader &) = delete;

		Shader(const Shader &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		Shader(Shader && other)
		{
			m_type = other.m_type;
			m_shaderId = other.m_shaderId;
			other.m_shaderId = 0;
		}

		/// <summary>
		/// Move assignment.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		Shader & operator= (Shader && other)
		{
			if (m_shaderId != 0) { glDeleteShader(m_shaderId); }
			m_type = other.m_type;
			m_shaderId = other.m_shaderId;
			other.m_shaderId = 0;
		}

		/// <summary>
		/// Determines whether this instance is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_shaderId != 0; }

		/// <summary>
		/// Gets the type of the shader.
		/// </summary>
		/// <returns></returns>
		ShaderType getType() const { assert(isValid() && "Shader::getType called on an invalid Shader"); return m_type; }

		/// <summary>
		/// Gets the identifier of the shader.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { assert(isValid() && "Shader::getId called on an invalid Shader"); return m_shaderId; }
	};
}