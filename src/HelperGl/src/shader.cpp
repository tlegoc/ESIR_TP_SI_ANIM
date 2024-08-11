#include <HelperGl/Shader.h>
#include <string.h>

namespace HelperGl
{


	size_t Shader::streamSize()
	{
		m_shader.seekg(m_shader.end) ;
		size_t result = (size_t)m_shader.tellg() ;
		m_shader.seekg(m_shader.beg) ;
		return result ;
	}

	Shader::Shader( ShaderType type ) : m_shaderType(type), m_shaderId(0)
	{
		assert(type==vertex || type==fragment) ;
	}

	bool Shader::loadFromFile( const ::std::string & filename )
	{
		std::ifstream file;
		// Opens the file
		file.open(filename.c_str());
		if (!file.good() )
		{
			std::cout << "Shader: Failed to open file: " << filename << std::endl;
			::std::cerr << "Error: " << strerror(errno) << std::endl;
			return false ;
		}
		// Dump the contents of the file into the stream
		m_shader << file.rdbuf();
		// Close the file
		file.close();
		return true;
	}

	bool Shader::compile()
	{
		if(isCompiled()) { return true ; }
		//// Compile the shader
		m_shaderId = glCreateShader(m_shaderType) ;
		::std::string program = m_shader.str() ;
		const char * str = program.c_str() ;
		glShaderSource(m_shaderId, 1, &str, NULL) ;
		glCompileShader(m_shaderId) ;
		// Check the compilation status and report any errors
		GLint shaderStatus;
		glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &shaderStatus);
		// If the shader failed to compile, display the info log and quit out
		if (shaderStatus == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(m_shaderId, infoLogLength, NULL, strInfoLog);
			std::cerr<<"Shader: shader compilation failed: "<<strInfoLog<<std::endl;
			delete[] strInfoLog;
			glDeleteShader(m_shaderId) ;
			m_shaderId = 0 ;
			return false ;
		}
		return true ;
	}
}
