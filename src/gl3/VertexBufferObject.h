#pragma once

#include <vector>
#include <type_traits>
#include <GL/GL.h>
#include <GL/glew.h>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gl3/GlmTypeTraits.h>
#include <gl3/proxy/AttribPointer.h>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// A very simple implementation of a vertex buffer object that can contains scalars or vectors up to dimension 4.
	/// Please, see <see cref="gl3::PackedVertexBufferObject"> PackedVertexBufferObject </see> for a more versatile implementation.
	/// </summary>
	class VertexBufferObject
	{
	private:
		GLuint m_id;
		GLenum m_scalarType;
		size_t m_vectorSize;
		size_t m_dataSize;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VertexBufferObject"/> class.
		/// </summary>
		/// <param name="buffer">The buffer.</param>
		/// <param name="usage">The usage.</param>
		template <class T>
		VertexBufferObject(const std::vector<T> & buffer, BufferUpdate usage = BufferUpdate::staticDraw)
		{
			static_assert(GlmTypeTraits<T>::isCompatible() && (GlmTypeTraits<T>::isScalar() || GlmTypeTraits<T>::isVector()), "Invalid type provided in VertexBufferObject constructor");
			// Création of the vertex buffer object
			glGenBuffers(1, &m_id);
			// Initialisation of the vertex buffer object
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(T)*buffer.size(), buffer.data(),static_cast<GLenum>(usage));
			m_scalarType = GLenum(GlmTypeTraits<T>::glType());
			m_vectorSize = GlmTypeTraits<T>::size();
			m_dataSize = buffer.size();
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="VertexBufferObject"/> class (the buffer is invalid)
		/// </summary>
		VertexBufferObject() : m_id(0) {}

		~VertexBufferObject()
		{
			if (m_id != 0) { glDeleteBuffers(1, &m_id); }
		}

		VertexBufferObject(const VertexBufferObject &) = delete;
		VertexBufferObject & operator = (const VertexBufferObject &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other buffer.</param>
		VertexBufferObject(VertexBufferObject && other)
		{
			m_id = other.m_id;
			m_scalarType = other.m_scalarType;
			m_vectorSize = other.m_vectorSize;
			m_dataSize = other.m_dataSize;
			other.m_id = 0;
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		VertexBufferObject & operator= (VertexBufferObject && other)
		{
			if (m_id != 0) { glDeleteBuffers(1, &m_id); }
			m_id = other.m_id;
			m_scalarType = other.m_scalarType;
			m_vectorSize = other.m_vectorSize;
			m_dataSize = other.m_dataSize;
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
		/// Gets the identifier of the VBO.
		/// </summary>
		/// <returns></returns>
		GLuint getId()  const { assert(isValid()); return m_id; }

		/// <summary>
		/// Gets the scalar type as a GLenum (GL_FLOAT for instance).
		/// </summary>
		/// <returns></returns>
		GLenum glScalarType() const { assert(isValid()); return m_scalarType; }

		/// <summary>
		/// Gets the vector size.
		/// </summary>
		/// <returns></returns>
		unsigned int vectorDimension() const { assert(isValid()); return m_vectorSize; }

		/// <summary>
		/// Returns the number of elements in the 
		/// </summary>
		/// <returns></returns>
		size_t size() const { assert(isValid()); return m_dataSize; }

		/// <summary>
		/// Updates the content of the buffer.
		/// </summary>
		/// <param name="buffer">The buffer.</param>
		template <class T>
		void update(const std::vector<T> & buffer) const
		{
			static_assert(GlmTypeTraits<T>::isCompatible() && (GlmTypeTraits<T>::isScalar() || GlmTypeTraits<T>::isVector()), "Invalid type provided in VertexBufferObject constructor");
			assert(isValid());
			assert(m_scalarType == GLenum(GlmTypeTraits<T>::glType()));
			assert(m_vectorSize == GlmTypeTraits<T>::size());
			assert(buffer.size() == m_dataSize);
			bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(T)*buffer.size(), buffer.data());
			unbind();
		}

		/// <summary>
		/// Binds this buffer.
		/// </summary>
		void bind() const
		{
			assert(isValid());
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		/// <summary>
		/// Unbinds this buffer.
		/// </summary>
		void unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		/// <summary>
		/// Associates this vertex buffer object with the attribute of a shader.
		/// Warning, the buffer is bound after calling this method.
		/// </summary>
		/// <param name="shaderAttributeIndex">Index of the shader attribute.</param>
		/// <param name="nbInstances">The number of instances before updating this attribute.</param>
		void attribPointer(GLint shaderAttributeIndex, GLuint nbInstances = 0) const
		{
			bind();
			// We associate the buffer with the input of the shader program
			glVertexAttribPointer(shaderAttributeIndex, vectorDimension(), glScalarType(), GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(shaderAttributeIndex);
			if(nbInstances>0) { glVertexAttribDivisor(shaderAttributeIndex, nbInstances); }
		}

		/// <summary>
		/// Returns a proxy enabling to bind the packed element structIndex to a vertex attribute.
		/// </summary>
		/// <param name="nbInstances">The number of instances before updating the attribute.</param>
		/// <returns></returns>
		proxy::AttribPointer attribPointerProxy(GLuint nbInstances = 0) const
		{
			return proxy::AttribPointer([this](GLint index) { attribPointer(index); });
		}
	};
}