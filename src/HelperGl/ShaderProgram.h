#pragma once
#include <HelperGl/Shader.h>
#include <Math/Vectorf.h>
#include <Math/Matrix4x4f.h>
#include <HelperGl/Buffer.h>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	ShaderProgram
	///
	/// \brief	Shader program class.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	09/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ShaderProgram
	{
	protected:
		/// \brief	The vertex shader.
		Shader * m_vertexShader;
		/// \brief	The fragment shader.
		Shader * m_fragmentShader;
		/// \brief	Identifier for the program.
		GLuint   m_programId;

		::std::map<::std::string, GLint> m_attributes;
		::std::map<::std::string, GLint> m_uniforms;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	ShaderProgram::ShaderProgram(Shader * vertexShader=NULL, Shader * fragmentShader=NULL)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param	vertexShader  	(optional) [in,out] If non-null, the vertex shader.
		/// \param	fragmentShader	(optional) [in,out] If non-null, the fragment shader.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ShaderProgram(Shader * vertexShader = NULL, Shader * fragmentShader = NULL)
			: m_vertexShader(vertexShader), m_fragmentShader(fragmentShader), m_programId(0)
		{
			assert(m_vertexShader == NULL || m_vertexShader->getType() == Shader::vertex);
			assert(m_fragmentShader == NULL || m_fragmentShader->getType() == Shader::fragment);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderProgram::setVertexShader(Shader * shader)
		///
		/// \brief	Sets vertex shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param [in,out]	shader	If non-null, the shader.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setVertexShader(Shader * shader)
		{
			assert(shader->getType() == Shader::vertex);
			m_vertexShader = shader;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderProgram::setFragmentShader(Shader * shader)
		///
		/// \brief	Sets fragment shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param [in,out]	shader	If non-null, the shader.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setFragmentShader(Shader * shader)
		{
			assert(shader->getType() == Shader::fragment);
			m_fragmentShader = shader;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderProgram::setShader(Shader * shader)
		///
		/// \brief	Sets a shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param [in,out]	shader	If non-null, the shader.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setShader(Shader * shader)
		{
			switch (shader->getType())
			{
			case Shader::vertex:
				m_vertexShader = shader;
				break;
			case Shader::fragment:
				m_fragmentShader = shader;
				break;
			default:
				::std::cerr << "ShaderProgram: wrong shader type" << ::std::endl;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool ShaderProgram::compileAndLink()
		///
		/// \brief	Compiles dans links the shader program.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \return	true if it succeeds, false if it fails.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool compileAndLink();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLuint ShaderProgram::getId() const
		///
		/// \brief	Gets the identifier.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \return	The identifier.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLuint getId() const
		{
			return m_programId;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool ShaderProgram::isLinked() const
		///
		/// \brief	Query if this object is linked.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \return	true if linked, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool isLinked() const
		{
			return m_programId != 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderProgram::enable()
		///
		/// \brief	Enables this shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void enable()
		{
			assert(isLinked());
			glUseProgram(m_programId);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ShaderProgram::disable()
		///
		/// \brief	Disables this shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void disable()
		{
			glUseProgram(0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLint ShaderProgram::getAttributeLocation(const ::std::string & name)
		///
		/// \brief	Gets an attribute location.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param	name	The name of the attribute.
		///
		/// \return	The attribute location.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLint getAttributeLocation(const ::std::string & name);

		/// <summary>
		/// Connects the attribute to the provided buffer.
		/// </summary>
		/// <param name="name">The name of the attribute.</param>
		/// <param name="buffer">The buffer.</param>
		template <typename Type>
		void setAttribute(const ::std::string & name, const Buffer<Type> & buffer)
		{
			setAttribute(getAttributeLocation(name), buffer);
		}

		/// <summary>
		/// Connects the attribute to the provided buffer.
		/// </summary>
		/// <param name="id">The identifier of the attribute.</param>
		/// <param name="buffer">The buffer.</param>
		template <typename Type>
		void setAttribute(GLuint id, const Buffer<Type> & buffer)
		{
			static_assert(false, "Unknown buffer type for setAttribute");
		}

		template <size_t nbDim>
		void setAttribute(GLuint id, const Buffer<Math::Vector<float, nbDim>> & buffer)
		{
			static_assert(nbDim <= 4, "Allowed vectors can have a maximum of 4 dimensions");
			buffer.bindBuffer();
			glVertexAttribPointer(id, nbDim, GL_FLOAT, false, 0, buffer.begin());
			buffer.unbindBuffer();
		}

		template <>
		void setAttribute(GLuint id, const Buffer<float> & buffer)
		{
			buffer.bindBuffer();
			glVertexAttribPointer(id, 1, GL_FLOAT, false, 0, buffer.begin());
			buffer.unbindBuffer();
		}

		template <size_t nbDim>
		void setAttribute(GLuint id, const Buffer<float[nbDim]> & buffer)
		{
			static_assert(nbDim <= 4, "Allowed vectors can have a maximum of 4 dimensions");
			buffer.bindBuffer();
			glVertexAttribPointer(id, nbDim, GL_FLOAT, false, 0, buffer.begin());
			buffer.unbindBuffer();
		}
		
		template <>
		void setAttribute(GLuint id, const Buffer<GLuint> & buffer)
		{
			buffer.bindBuffer();
			glVertexAttribPointer(id, 1, GL_UNSIGNED_INT, false, 0, buffer.begin());
			buffer.unbindBuffer();
		}

		template <size_t nbDim>
		void setAttribute(GLuint id, const Buffer<GLuint[nbDim]> & buffer)
		{
			static_assert(nbDim <= 4, "Allowed vectors can have a maximum of 4 dimensions");
			buffer.bindBuffer();
			glVertexAttribPointer(id, nbDim, GL_UNSIGNED_INT, false, 0, buffer.begin());
			buffer.unbindBuffer();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLuint ShaderProgram::getUniformLocation(const ::std::string & name)
		///
		/// \brief	Gets a uniform location.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param	name	The name of the uniform.
		///
		/// \return	The uniform location.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLuint getUniformLocation(const ::std::string & name);

		/// <summary>
		/// Sets the value of a uniform.
		/// </summary>
		/// <param name="name">The name.</param>
		/// <param name="value">The value.</param>
		template <typename Type>
		void setUniform(const ::std::string & name, const Type & value)
		{
			setUniform(getUniformLocation(name), value);
		}

		void setUniform(GLuint id, GLuint value)
		{
			glUniform1ui(id, value);
		}

		void setUniform(GLuint id, float value)
		{
			glUniform1f(id, value);
		}

		void setUniform(GLuint id, const Math::Vector3f & value)
		{
			glUniform3f(id, value[0], value[1], value[2]);
		}

		void setUniform(GLuint id, const Math::Matrix4f & value)
		{
			float matrix[16];
			value.toBuffer(matrix);
			glUniformMatrix4fv(id, 1, false, matrix);
		}

	};

}