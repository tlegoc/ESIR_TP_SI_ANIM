#include <HelperGl/ShaderProgram.h>

namespace HelperGl
{
	bool ShaderProgram::compileAndLink()
	{
		assert(m_vertexShader != NULL);
		assert(m_fragmentShader != NULL);
		// Compiles the shaders (if necessary)
		bool successVertex = m_vertexShader->compile();
		bool successFragment = m_fragmentShader->compile();
		if (!successFragment || !successVertex)
		{
			return false; // A compilation error occurred
		}
		// Compiles the progam
		m_programId = glCreateProgram();
		glAttachShader(m_programId, m_vertexShader->getId());
		glAttachShader(m_programId, m_fragmentShader->getId());
		glLinkProgram(m_programId);
		// Check the status
		GLint linkStatus;
		glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLint infoLogLength = 4096;
			//glGetShaderiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(m_programId, infoLogLength, NULL, strInfoLog);
			std::cout << "ShaderProgram: Shader program linking failed. ERROR:" << strInfoLog << std::endl;
			delete[] strInfoLog;
			return false;
		}
		return true;
	}

	GLint ShaderProgram::getAttributeLocation(const ::std::string & name)
	{
		GLint result = glGetAttribLocation(m_programId, name.c_str());
		if (result == -1) // Error case
		{
			::std::cerr << "ShaderProgram: attribute " << name << " not found!" << ::std::endl;
			return result;
		}
		m_attributes[name] = result;
		return result;
	}

	GLuint ShaderProgram::getUniformLocation(const ::std::string & name)
	{
		GLint result = glGetUniformLocation(m_programId, name.c_str());
		if (result == -1) // Error case
		{
			::std::cerr << "ShaderProgram: uniform " << name << " not found!" << ::std::endl;
			return result;
		}
		m_uniforms[name] = result;
		return result;
	}
}