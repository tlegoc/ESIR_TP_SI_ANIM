#pragma once

#include <gl3/Shader.h>
#include <gl3/VertexBufferObject.h>
#include <gl3/proxy/SetUniform.h>
#include <gl3/proxy/SetAttribute.h>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

namespace gl3
{
	/// <summary>
	/// A linked shader program.
	/// </summary>
	class ShaderProgram
	{
		GLuint m_programId;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderProgram"/> class with a vertex and fragment shaders.
		/// Throws an exception of link failed.
		/// </summary>
		/// <param name="vertex">The vertex shader.</param>
		/// <param name="fragment">The fragment shader.</param>
		ShaderProgram(const Shader & vertex, const Shader & fragment)
		{
			assert(vertex.getType() == ShaderType::vertexShader && "ShaderPorgram::ShaderProgram invalid shader type as first parameter, should be a vertex shader");
			assert(fragment.getType() == ShaderType::fragmentShader && "ShaderPorgram::ShaderProgram invalid shader type as second parameter, should be a fragement shader");
			m_programId = glCreateProgram();
			glAttachShader(m_programId, vertex.getId());
			glAttachShader(m_programId, fragment.getId());
			link();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderProgram"/> class with a vertex, geometry and fragment shaders.
		/// Throws an exception of link failed.
		/// </summary>
		/// <param name="vertex">The vertex shader.</param>
		/// <param name="geometry">The geometry shader.</param>
		/// <param name="fragment">The fragment shader.</param>
		ShaderProgram(const Shader & vertex, const Shader & geometry, const Shader & fragment)
		{
			assert(vertex.getType() == ShaderType::vertexShader && "ShaderPorgram::ShaderProgram invalid shader type as first parameter, should be a vertex shader");
			assert(geometry.getType() == ShaderType::geometryShader && "ShaderPorgram::ShaderProgram invalid shader type as second parameter, should be geometry shader");
			assert(fragment.getType() == ShaderType::fragmentShader && "ShaderPorgram::ShaderProgram invalid shader type as third parameter, should be a fragment shader");
			m_programId = glCreateProgram();
			glAttachShader(m_programId, vertex.getId());
			glAttachShader(m_programId, geometry.getId());
			glAttachShader(m_programId, fragment.getId());
			link();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderProgram"/> class from the source files of the shaders
		/// </summary>
		/// <param name="vertexSource">The vertex shader source.</param>
		/// <param name="fragmentSource">The fragment shader source.</param>
		ShaderProgram(const std::filesystem::path & vertexSource, const std::filesystem::path & fragmentSource)
			: ShaderProgram(Shader(ShaderSource(vertexSource), ShaderType::vertexShader), Shader(ShaderSource(fragmentSource), ShaderType::fragmentShader))
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderProgram"/> class from the sources files of the shaders
		/// </summary>
		/// <param name="vertexSource">The vertex shader source.</param>
		/// <param name="geometrySource">The geometry shader source.</param>
		/// <param name="fragmentSource">The fragment shader source.</param>
		ShaderProgram(const std::filesystem::path & vertexSource, const std::filesystem::path & geometrySource, const std::filesystem::path & fragmentSource)
			: ShaderProgram(Shader(ShaderSource(vertexSource), ShaderType::vertexShader), 
				Shader(ShaderSource(geometrySource), ShaderType::geometryShader),
				Shader(ShaderSource(fragmentSource), ShaderType::fragmentShader))
		{}

		/// <summary>
		/// Default constructor. The shader program is invalid.
		/// </summary>
		ShaderProgram() : m_programId(0) {}

		/// <summary>
		/// Finalizes an instance of the <see cref="ShaderProgram"/> class.
		/// </summary>
		~ShaderProgram()
		{
			if (m_programId != 0) { glDeleteProgram(m_programId); }
		}

		ShaderProgram & operator=(const ShaderProgram &) = delete;
		ShaderProgram(const ShaderProgram &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		ShaderProgram(ShaderProgram && other)
		{
			m_programId = other.m_programId;
			other.m_programId = 0;
		}

		ShaderProgram & operator= (ShaderProgram && other)
		{
			if (m_programId != 0) { glDeleteProgram(m_programId); }
			m_programId = other.m_programId;
			other.m_programId = 0;
			return (*this);
		}

		/// <summary>
		/// Determines whether this instance is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_programId != 0; }

		/// <summary>
		/// Gets the identifier of the shader program.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { assert(isValid() && "ShaderProgram::getId called on an invalid shader program"); return m_programId; }

		/// <summary>
		/// Binds the shader program.
		/// </summary>
		void use() const { assert(isValid() && "ShaderProgram::use called on an invalid shader program"); glUseProgram(m_programId); }

		/// <summary>
		/// Unbinds the shader program.
		/// </summary>
		void unuse() const { assert(isValid() && "ShaderProgram::unuse called on an invalid shader program"); glUseProgram(0); }

		/// <summary>
		/// Gets a uniform location.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <returns> -1 if the uniform does not exist, the uniform id otherwise</returns>
		GLint getUniformLocation(const std::string & name) const
		{
			assert(isValid() && "ShaderProgram::getUniformLocation called on an invalid shader program") ;
			return glGetUniformLocation(m_programId, name.c_str());
		}

		/// <summary>
		/// Sets the value of a uniform based on its name.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <param name="value">The value.</param>
		/// <param name="size">The size of the array if the reference designate the first element of an array.</param>
		template <typename Type>
		void setUniform(const std::string & name, const Type & value, size_t size = 1)
		{
			setUniform(getUniformLocation(name), value, size);
		}

		/// <summary>
		/// Tries to sets the value of a uniform based on its name.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <param name="value">The value.</param>
		/// <param name="size">The size of the array if the reference designate the first element of an array.</param>
		/// <return> true if the uniform has been set, false otherwise (uniform not found) </return>
		template <typename Type>
		bool trySetUniform(const std::string & name, const Type & value, size_t size = 1)
		{
			GLint id = getUniformLocation(name);
			if (id != -1)
			{
				setUniform(id, value, size);
				return true;
			}
			return false;
		}

		/// <summary>
		/// Sets the values of a uniform (which is an array) based on its name.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <param name="value">The values.</param>
		template <typename Type>
		void setUniform(const std::string & name, const std::vector<Type> & v)
		{
			setUniform(getUniformLocation(name), v);
		}

		/// <summary>
		/// Tries to set the values of a uniform (which is an array) based on its name.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <param name="value">The values.</param>
		/// <return> true of the uniform has been set, false otherwise (uniform not found) </return>
		template <typename Type>
		bool trySetUniform(const std::string & name, const std::vector<Type> & v)
		{
			GLint id = getUniformLocation(name);
			if (id != -1)
			{
				setUniform(id, v);
				return true;
			}
			return false;
		}

		template <typename T>
		void setUniform(GLint id, const std::vector<T> & v)
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			setUniform(id, *v.data(), v.size());
		}

		void setUniform(GLint id, const GLint & value, size_t size = 1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniform1iv(id, size, &value);
		}

		void setUniform(GLint id, const GLfloat & value, size_t size=1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniform1fv(id, size, &value);
		}

		void setUniform(GLint id, const glm::vec2 & v, size_t size=1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniform2fv(id, size, glm::value_ptr(v));
		}

		void setUniform(GLint id, const glm::vec3 & v, size_t size=1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniform3fv(id, size, glm::value_ptr(v));
		}

		void setUniform(GLint id, const glm::vec4 & v, size_t size=1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniform4fv(id, size, glm::value_ptr(v));
		}

		void setUniform(GLint id, const glm::mat2x2 & m, size_t size = 1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniformMatrix2fv(id, size, GL_FALSE, glm::value_ptr(m));
		}

		void setUniform(GLint id, const glm::mat3x3 & m, size_t size = 1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniformMatrix3fv(id, size, GL_FALSE, glm::value_ptr(m));
		}

		void setUniform(GLint id, const glm::mat4x4 & m, size_t size = 1) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			glUniformMatrix4fv(id, size, GL_FALSE, glm::value_ptr(m));
		}

		template <typename DataType>
		void setUniform(GLint id, const std::vector<DataType> & vector) const
		{
			assert(id >= 0 && "ShaderProgram::setUniform called with an invalid uniform index");
			assert(isValid() && isActive()) ;
			setUniform(id, vector[0], vector.size());
		}

		/// <summary>
		/// Gets a proxy to a set uniform call.
		/// </summary>
		/// <param name="name">The name of the uniform.</param>
		/// <returns></returns>
		template <class Type>
		proxy::SetUniform<Type> setUniformProxy(const std::string & name)
		{
			GLint id = getUniformLocation(name);
			return [this, id](const Type & value) { setUniform(id, value); };
		}

		/// <summary>
		/// Gets the attribute location.
		/// </summary>
		/// <param name="name">The name of the attribute.</param>
		/// <returns></returns>
		GLint getAttributeLocation(const std::string & name) const
		{
			assert(isValid()) ;
			return glGetAttribLocation(m_programId, name.c_str());
		}

		/// <summary>
		/// Sets an attribute of the shader.
		/// </summary>
		/// <param name="name">The name of the attibute.</param>
		/// <param name="buffer">The vertex buffer object to associate with the attribute.</param>
		/// <param name="nbInstances">The number of instances to skip until updating the attribute (for geometry instancing purpose).</param>
		void setAttribute(const std::string & name, const VertexBufferObject * buffer, GLuint nbInstances=0)
		{
			assert(isValid());
			assert(isActive());
			GLint location = getAttributeLocation(name);
			buffer->attribPointer(location, nbInstances);
		}

		/// <summary>
		/// Sets the attribute.
		/// </summary>
		/// <param name="name">The name of the attribute.</param>
		/// <param name="attrib">The attribute pointer proxy.</param>
		void setAttribute(const std::string & name, const proxy::AttribPointer & attrib)
		{
			assert(isValid());
			assert(isActive());
			GLint location = getAttributeLocation(name);
			attrib.attribPointer(location);
		}

		/// <summary>
		/// Returns a proxy to the setAttribute methods.
		/// </summary>
		/// <param name="name">The name of the attribute. </param>
		/// <returns></returns>
		proxy::SetAttribute setAttributeProxy(const std::string & name)
		{
			assert(isValid());
			GLint location = getAttributeLocation(name);
			return proxy::SetAttribute(
				[this, location](const VertexBufferObject * buffer, GLuint nbInstances) 
				{
					assert(isActive());
					buffer->attribPointer(location, nbInstances);
				},
				[this, location](const proxy::AttribPointer & attrib)
				{
					assert(isActive());
					attrib.attribPointer(location);
				}
			);
		}

		/// <summary>
		/// Determines whether this shader program is active.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this shader program is active; otherwise, <c>false</c>.
		/// </returns>
		bool isActive() const { return getActive() == m_programId; }

		

		/// <summary>
		/// Gets the identifier of the active shader program.
		/// </summary>
		/// <returns></returns>
		static GLint getActive()
		{
			GLint id;
			glGetIntegerv(GL_CURRENT_PROGRAM, &id);
			return id;
		}

	private:
		/// <summary>
		/// Links the shader program.
		/// </summary>
		void link()
		{
			// We link the program
			glLinkProgram(m_programId);
			// Check the status
			GLint linkStatus;
			glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);
			if (linkStatus == GL_FALSE) // Just in case GL_FLASE is not 0...
			{
				GLint infoLogLength = 4096;
				//glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);
				GLchar *strInfoLog = new GLchar[infoLogLength + 1];
				glGetProgramInfoLog(m_programId, infoLogLength, NULL, strInfoLog);
				std::cerr << "ShaderProgram: Failed to link shader program." << std::endl << strInfoLog << std::endl;
				delete[] strInfoLog;
				m_programId = 0;
				throw std::runtime_error("ShaderProgram: failed to link program");
			}
		}
	};
}