#pragma once

#include <gl3/VertexBufferObject.h>
#include <gl3/ElementBufferObject.h>
#include <gl3/ShaderProgram.h>

namespace gl3
{
	/// <summary>
	/// A vertex array class
	/// </summary>
	class VertexArrayObject
	{
		GLuint m_id;
		size_t m_eboSize;

		template <typename Type>
		static std::vector<std::pair<GLint, Type>> convert(const ShaderProgram & program, const std::vector<std::pair<std::string, Type>> & vbos)
		{
			std::vector<std::pair<GLint, Type>> result;
			result.reserve(vbos.size());
			for (auto it = vbos.begin(), end = vbos.end(); it != end; ++it)
			{
				GLint location = program.getAttributeLocation(it->first);
				assert(location != -1);
				result.push_back({location, it->second});
			}
			return std::move(result);
		}
		
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArrayObject"/> class.
		/// </summary>
		/// <param name="vbo">The vertex buffer objects stored in the VAO provided as pairs (shader program attribute, vertex buffer object) </param>
		/// <param name="ebo">The element buffer array or nullptr.</param>
		VertexArrayObject(const std::vector<std::pair<GLint, const VertexBufferObject *>> & vbo, const ElementBufferObject * ebo = nullptr)
		{
			// Creation and binding of the vertex array object
			glGenVertexArrays(1, &m_id);
			glBindVertexArray(m_id);
			// Association of vertex buffer objects with the vertex array object and the shader program
			for (auto it = vbo.begin(), end = vbo.end(); it != end; ++it)
			{
				const std::pair<GLuint, const VertexBufferObject *> & current = (*it);
				// We bind the buffer
				//current.second->bind();
				// We associate the buffer with the input of the shader program
				current.second->attribPointer(current.first);
				//glVertexAttribPointer(current.first, current.second->vectorDimension(), current.second->glScalarType(), GL_FALSE, 0, (void*)0);
				//glEnableVertexAttribArray(current.first);
			}
			// Association of the element buffer object with the vertex array object
			if (ebo != nullptr) { ebo->bind(); }
			// We unbind the VAO
			glBindVertexArray(0);
			//// We store the configuration
			//m_vbos = vbo;
			//m_ebo = ebo;
			m_eboSize = ebo->size();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArrayObject"/> class.
		/// </summary>
		/// <param name="program">The shader program.</param>
		/// <param name="vbos">The association between an attrbute name of the shader program and its corresponding vbo.</param>
		/// <param name="ebo">The ebo or nullptr.</param>
		VertexArrayObject(const ShaderProgram & program, const std::vector<std::pair<std::string, const VertexBufferObject *>> & vbos, const ElementBufferObject * ebo = nullptr)
			: VertexArrayObject(convert(program, vbos), ebo)
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArrayObject"/> class.
		/// </summary>
		/// <param name="vbo">The association between the attributes of a vertex shader and the vbos.</param>
		/// <param name="ebo">The element buffer object or nullptr.</param>
		VertexArrayObject(const std::vector<std::pair<GLint, proxy::AttribPointer>> & vbo, const ElementBufferObject * ebo = nullptr)
		{
			// Creation and binding of the vertex array object
			glGenVertexArrays(1, &m_id);
			glBindVertexArray(m_id);
			// Association of vertex buffer objects with the vertex array object and the shader program
			for (auto it = vbo.begin(), end = vbo.end(); it != end; ++it)
			{
				const std::pair<GLuint, proxy::AttribPointer> & current = (*it);
				// We associate the buffer with the input of the shader program
				current.second.attribPointer(current.first);
			}
			// Association of the element buffer object with the vertex array object
			if (ebo != nullptr) { ebo->bind(); }
			// We unbind the VAO
			glBindVertexArray(0);
			m_eboSize = ebo->size();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArrayObject"/> class.
		/// </summary>
		/// <param name="program">The shader program.</param>
		/// <param name="vbos">The association between an attrbute name of the shader program and its corresponding vbo.</param>
		/// <param name="ebo">The ebo or nullptr.</param>
		VertexArrayObject(const ShaderProgram & program, const std::vector<std::pair<std::string, proxy::AttribPointer>> & vbos, const ElementBufferObject * ebo = nullptr)
			: VertexArrayObject(convert(program, vbos), ebo)
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="VertexArrayObject"/> class. The buffer is invalid.
		/// </summary>
		VertexArrayObject() :m_id(0) {}
	
		/// <summary>
		/// Finalizes an instance of the <see cref="VertexArrayObject"/> class.
		/// </summary>
		~VertexArrayObject()
		{
			if (m_id != 0) { glDeleteVertexArrays(1, &m_id); }
		}

		VertexArrayObject(const VertexArrayObject &) = delete;
		VertexArrayObject operator = (const VertexArrayObject &) = delete;

		/// <summary>
		/// Move contructor.
		/// </summary>
		/// <param name="other">The other.</param>
		VertexArrayObject(VertexArrayObject && other)
		{
			m_id = other.m_id;
			//m_vbos = std::move(other.m_vbos);
			//m_ebo = other.m_ebo;
			m_eboSize = other.m_eboSize;
			other.m_id = 0;
		}

		/// <summary>
		/// Move assignment.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		VertexArrayObject & operator= (VertexArrayObject && other)
		{
			if (m_id != 0) { glDeleteVertexArrays(1, &m_id); }
			m_id = other.m_id;
			//m_vbos = std::move(other.m_vbos);
			//m_ebo = other.m_ebo;
			m_eboSize = other.m_eboSize;
			other.m_id = 0;
			return *this;
		}

		/// <summary>
		/// Determines whether this instance is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_id != 0; }

		///// <summary>
		///// Gets the element buffer object.
		///// </summary>
		///// <returns></returns>
		//const ElementBufferObject * getElementBufferObject() const { return m_ebo; }

		/// <summary>
		/// Returns the size of the attached element buffer object.
		/// </summary>
		/// <returns></returns>
		size_t eboSize() const { return m_eboSize; }

		/// <summary>
		/// Binds this VAO.
		/// </summary>
		void bind() const
		{
			assert(isValid());
			glBindVertexArray(m_id);
		}

		/// <summary>
		/// Unbinds this VAO.
		/// </summary>
		void unbind() const
		{
			glBindVertexArray(0);
		}
	};
}