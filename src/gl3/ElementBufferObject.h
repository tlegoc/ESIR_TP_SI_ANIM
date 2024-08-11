#pragma once

#include <vector>
#include <GL/GL.h>
#include <GL/glew.h>
#include <gl3/VertexBufferObject.h>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// An element buffer object.
	/// </summary>
	class ElementBufferObject
	{
		GLuint m_id;
		size_t m_size;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ElementBufferObject"/> class.
		/// </summary>
		/// <param name="indices">The indices.</param>
		/// <param name="usage">The usage.</param>
		ElementBufferObject(const std::vector<GLuint> & indices, BufferUpdate usage = BufferUpdate::staticDraw)
		{
			// Creation of the element buffer object
			glGenBuffers(1, &m_id);
			// Initialization of the element buffer object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), static_cast<GLenum>(usage));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_size = indices.size();
		}

		/// <summary>
		/// Default constructor. The buffer is invalid.
		/// </summary>
		ElementBufferObject() {}

		~ElementBufferObject()
		{
			if (m_id != 0) { glDeleteBuffers(1, &m_id); }
		}

		ElementBufferObject(const ElementBufferObject &) = delete;
		ElementBufferObject & operator= (const ElementBufferObject &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		ElementBufferObject(ElementBufferObject && other)
		{
			m_id = other.m_id;
			m_size = other.m_size;
			other.m_id = 0; 
		}

		/// <summary>
		/// Move assignment.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		ElementBufferObject & operator= (ElementBufferObject && other)
		{
			if (m_id != 0) { glDeleteBuffers(1, &m_id); }
			m_id = other.m_id;
			m_size = other.m_size;
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

		/// <summary>
		/// Gets the identifier of the buffer.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { assert(isValid() && "ElementBufferObject::getId called but the EBO is not initialized"); return m_id; }

		/// <summary>
		/// Returns the type of the elements in the buffer (GL_UNSIGNED_INT).
		/// </summary>
		/// <returns></returns>
		GLenum glElementType() const { assert(isValid() && "ElementBufferObject::glElementType called but the EBO is not initialized"); return GL_UNSIGNED_INT; }

		/// <summary>
		/// Returns the number of elements in the buffer.
		/// </summary>
		/// <returns></returns>
		size_t size() const { assert(isValid() && "ElementBufferObject::size called but the EBO is not initialized");  return m_size; }

		/// <summary>
		/// Binds this buffer.
		/// </summary>
		void bind() const
		{
			assert(isValid() && "ElementBufferObject::bind called but the EBO is not initialized");
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}

		/// <summary>
		/// Unbinds this buffer.
		/// </summary>
		void unbind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};
}