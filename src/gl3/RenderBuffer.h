#pragma once

#include <GL/glew.h>
#include <gl3/Texture2D.h>

namespace gl3
{
	/// <summary>
	/// A render buffer (can be a depth buffer, a stencil buffer, a frame buffer etc...)
	/// </summary>
	class RenderBuffer
	{
		GLuint m_id;
		GLuint m_width, m_height;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RenderBuffer"/> class.
		/// </summary>
		/// <param name="width">The width of the buffer.</param>
		/// <param name="height">The height of the buffer.</param>
		/// <param name="storage">The storage of the buffer.</param>
		RenderBuffer(GLuint width, GLuint height, TextureInternalFormat storage)
		{
			glGenRenderbuffers(1, &m_id);
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
			glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(storage), width, height);
			m_width = width;
			m_height = height;
		}

		/// <summary>
		/// Creates an invalid buffer (see isValid()).
		/// </summary>
		RenderBuffer()
			: m_id(0)
		{}

		/// <summary>
		/// Finalizes an instance of the <see cref="RenderBuffer"/> class.
		/// </summary>
		~RenderBuffer()
		{
			if (m_id != 0) { glDeleteRenderbuffers(1, &m_id); }
		}

		RenderBuffer(const RenderBuffer &) = delete;
		RenderBuffer & operator=(const RenderBuffer &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		RenderBuffer(RenderBuffer && other)
		{
			m_id = other.m_id;
			m_width = other.m_width;
			m_height = other.m_height;
			other.m_id = 0;
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		RenderBuffer & operator= (RenderBuffer && other)
		{
			if (m_id != 0) { glDeleteRenderbuffers(1, &m_id); }
			m_id = other.m_id;
			m_width = other.m_width;
			m_height = other.m_height;
			other.m_id = 0;
			return (*this);
		}

		/// <summary>
		/// Determines whether this instance is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_id != 0; }

		/// <summary>
		/// Gets the identifier.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { assert(isValid() && "RenderBuffer::getId called on an invalid RenderBuffer"); return m_id; }

		/// <summary>
		/// Gets the width.
		/// </summary>
		/// <returns></returns>
		GLuint getWidth() const { assert(isValid() && "RenderBuffer::getWidth called on an invalid RenderBuffer"); return m_width; }

		/// <summary>
		/// Gets the height.
		/// </summary>
		/// <returns></returns>
		GLuint getHeight() const { assert(isValid() && "RenderBuffer::getHeight called on an invalid RenderBuffer"); return m_height; }
	};
}