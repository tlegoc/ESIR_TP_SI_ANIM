#pragma once

#include <GL/glew.h>
#include <gl3/Texture2D.h>
#include <gl3/RenderBuffer.h>
#include <cassert>

namespace gl3
{
	/// <summary>
	/// A frame buffer object. The default behavior is to connect the fragment shader output X to the COLOR_ATTACHMENTX (for simplification reasons)
	/// </summary>
	class FrameBufferObject
	{
		GLuint m_id;
		GLuint m_width, m_height;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FrameBufferObject"/> class from the buffers and their attachments in the frame buffer.
		/// </summary>
		/// <param name="textures">The textures associated with their attachment.</param>
		/// <param name="renderBuffers">The render buffers associated with their attachment.</param>
		FrameBufferObject(const std::vector<std::pair<FboAttachement, Texture2D*>> & textures, const std::vector<std::pair<FboAttachement, RenderBuffer*>> & renderBuffers)
		{
			assert((textures.size() > 0 || renderBuffers.size() > 0) && "FrameBufferObject::FrameBufferObject called with no attachment provvided");

			// The buffer attachments
			GLenum bufferAttachment[] = { GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE };

			// We create the frame buffer object and bind it
			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);

			// We get the dimensions of one existing buffer in order to verify the size consistency over all buffers
			if (textures.size() > 0)
			{
				m_width = textures[0].second->getWidth();
				m_height = textures[0].second->getHeight();
			}
			else
			{
				m_width = renderBuffers[0].second->getWidth();
				m_height = renderBuffers[0].second->getHeight();
			}

			for (auto it = textures.begin(), end = textures.end(); it != end; ++it)
			{
				const auto & current = *it;
				FboAttachement attachment = current.first;
				Texture2D * texture = current.second;
				assert(m_width == current.second->getWidth() && m_height == current.second->getHeight() && "FrameBufferObject::FrameBufferObject called with textures / render buffers of different sizes");
				glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(attachment), GL_TEXTURE_2D, texture->getId(), 0);
				if (static_cast<GLenum>(attachment) >= GL_COLOR_ATTACHMENT0 && static_cast<GLenum>(attachment) < GL_COLOR_ATTACHMENT0 + 8)
				{
					bufferAttachment[static_cast<GLenum>(attachment) - GL_COLOR_ATTACHMENT0] = static_cast<GLenum>(attachment);
				}
			}

			for (auto it = renderBuffers.begin(), end = renderBuffers.end(); it != end; ++it)
			{
				const auto & current = *it;
				assert(m_width == current.second->getWidth() && m_height == current.second->getHeight() && "FrameBufferObject::FrameBufferObject called with textures / render buffers of different sizes");
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(current.first), GL_RENDERBUFFER, current.second->getId());
				FboAttachement attachment = current.first;
				if (static_cast<GLenum>(attachment) >= GL_COLOR_ATTACHMENT0 && static_cast<GLenum>(attachment) < GL_COLOR_ATTACHMENT0 + 8)
				{
					bufferAttachment[static_cast<GLenum>(attachment) - GL_COLOR_ATTACHMENT0] = static_cast<GLenum>(attachment);
				}
			}

			glDrawBuffers(8, bufferAttachment);
			
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNSUPPORTED)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_UNSUPPORTED this format is not supported by your graphics card" << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNDEFINED)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_UNDEFINED the specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist." << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT at least one of the framebuffer attachment points are framebuffer incomplete" << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT the framebuffer does not have at least one image attached to it." << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER. " << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			{
				std::cerr << "FrameBufferObject: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES." << std::endl;
			}

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cerr << "FrameBufferObject: initialization failed" << std::endl;
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				throw std::runtime_error("FrameBufferObject: could not initialize the frame buffer.");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/// <summary>
		/// Creates an invalid buffer object
		/// </summary>
		FrameBufferObject()
			: m_id(0)
		{}

		/// <summary>
		/// Finalizes an instance of the <see cref="FrameBufferObject"/> class.
		/// </summary>
		~FrameBufferObject()
		{
			if(m_id!=0) glDeleteFramebuffers(1, &m_id);
		}

		/// <summary>
		/// Forbids copy constructor
		/// </summary>
		FrameBufferObject(const FrameBufferObject &) = delete;

		/// <summary>
		/// Forbids default assignment
		/// </summary>
		FrameBufferObject & operator= (const FrameBufferObject &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		FrameBufferObject(FrameBufferObject && other)
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
		FrameBufferObject & operator = (FrameBufferObject && other)
		{
			if (m_id != 0) glDeleteFramebuffers(1, &m_id);
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
		GLuint getId() const { assert(isValid() && "FrameBufferObject::getId called on an uninitialized frame buffer object"); return m_id; }

		/// <summary>
		/// Gets the width.
		/// </summary>
		/// <returns></returns>
		GLuint getWidth() const { assert(isValid() && "FrameBufferObject::getWidth called on an uninitialized frame buffer object"); return m_width; }

		/// <summary>
		/// Gets the height.
		/// </summary>
		/// <returns></returns>
		GLuint getHeight() const { assert(isValid() && "FrameBufferObject::getHeight called on an uninitialized frame buffer object");  return m_height; }

		/// <summary>
		/// Binds this instance.
		/// </summary>
		void bind() const
		{
			assert(isValid() && "FrameBufferObject::bind called on an uninitialized frame buffer object");
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		/// <summary>
		/// Unbinds this instance.
		/// </summary>
		void unbind() const
		{
			assert(isValid() && "FrameBufferObject::unbind called on an uninitialized frame buffer object");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	};
}