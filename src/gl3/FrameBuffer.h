#pragma once

#include <gl3/Texture2D.h>
#include <gl3/RenderBuffer.h>
#include <gl3/FrameBufferObject.h>
#include <unordered_map>
#include <variant>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// A convenient class encapsulating a frame buffer object and all its attachments. It accepts resizing and binding.
	/// </summary>
	class FrameBuffer
	{
	protected:
		std::vector<std::tuple<FboAttachement, FboAttachmentType, TextureInternalFormat>> m_configuration;
		std::unordered_map<FboAttachement, std::variant<RenderBuffer, Texture2D>> m_instances;
		FrameBufferObject m_fbo;
		size_t m_width, m_height;

	public:

		/// <summary>
		/// Default constructor : the frame buffer is empty!!!!
		/// </summary>
		FrameBuffer()
		{}

		/// <summary>
		/// Initializes a new frame buffer.
		/// </summary>
		/// <param name="configuration">The configuration of the frame buffer.</param>
		/// <param name="width">The width of the frame buffer.</param>
		/// <param name="height">The height of the frame buffer.</param>
		FrameBuffer(const std::vector<std::tuple<FboAttachement, FboAttachmentType, TextureInternalFormat>> & configuration, size_t width, size_t height)
			: m_configuration(configuration)
		{
			initialize(width, height);
		}

		/// <summary>
		/// Forbids the copy constructor.
		/// </summary>
		FrameBuffer(const FrameBuffer &) = delete;

		/// <summary>
		/// Forbids the assignment operator.
		/// </summary>
		FrameBuffer & operator= (const FrameBuffer &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		FrameBuffer(FrameBuffer && other)
			: m_configuration(std::move(other.m_configuration)),
			m_instances(std::move(other.m_instances)),
			m_fbo(std::move(other.m_fbo)),
			m_width(other.m_width),
			m_height(other.m_height)
		{}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		virtual FrameBuffer & operator= (FrameBuffer && other)
		{
			m_configuration = std::move(other.m_configuration);
			m_instances = std::move(other.m_instances);
			m_fbo = std::move(other.m_fbo);
			m_width = other.m_width;
			m_height = other.m_height;
			return (*this);
		}

		/// <summary>
		/// Determines whether this frame buffer is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this frame buffer is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_fbo.isValid(); }

		/// <summary>
		/// Resizes the frame buffer. In case you inherit this class, this method is virtual and allows 
		/// you to handle some specific post initializations.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		virtual void resize(size_t width, size_t height)
		{
			assert(isValid() && "FrameBuffer::resize called but the frame buffer is not initialized");
			initialize(width, height);
		}

		/// <summary>
		/// Binds this frame buffer.
		/// </summary>
		void bind() const
		{
			assert(isValid() && "FrameBuffer::bind called but the frame buffer is not initialized");
			m_fbo.bind();
		}

		/// <summary>
		/// Unbinds this frame buffer.
		/// </summary>
		void unbind() const
		{
			assert(isValid() && "FrameBuffer::unbind called but the frame buffer is not initialized");
			m_fbo.unbind();
		}

		/// <summary>
		/// Gets the texture associated with the provided attachment.
		/// </summary>
		/// <param name="attachment">The attachment.</param>
		/// <returns> A pointer to the texture or nullptr if a render buffer is attached or if there is no attachment </returns>
		const Texture2D * getTexture(FboAttachement attachment) const
		{
			assert(isValid() && "FrameBuffer::getTexture called but the frame buffer is not initialized");
			auto it = m_instances.find(attachment);
			if (it != m_instances.end() && std::holds_alternative<Texture2D>(it->second))
			{
				return &std::get<1>(it->second);
			}
			return nullptr;
		}

		/// <summary>
		/// Gets the render buffer associated with the provided attachment.
		/// </summary>
		/// <param name="attachment">The attachment.</param>
		/// <returns> A pointer to the render buffer or nullptr if a texture is attached or if there is no attachment </returns>
		const RenderBuffer * getRenderBuffer(FboAttachement attachment) const
		{
			assert(isValid() && "FrameBuffer::getRenderBuffer called but the frame buffer is not initialized");
			auto it = m_instances.find(attachment);
			if (it != m_instances.end() && std::holds_alternative<RenderBuffer>(it->second))
			{
				return &std::get<0>(it->second);
			}
			return nullptr;
		}

		/// <summary>
		/// Gets the width of the frame buffer.
		/// </summary>
		/// <returns></returns>
		size_t getWidth() const { return m_width; }

		/// <summary>
		/// Gets the height of the frame buffer.
		/// </summary>
		/// <returns></returns>
		size_t getHeight() const { return m_height; }


	private:
		/// <summary>
		/// Initializes the frame buffer and also reinitializes it if usefull
		/// </summary>
		/// <param name="width">The width of the frame buffer.</param>
		/// <param name="height">The height of the frame buffer.</param>
		void initialize(size_t width, size_t height)
		{
			std::vector <std::pair<FboAttachement, Texture2D *>> textures;
			std::vector<std::pair<FboAttachement, RenderBuffer*>> renderBuffers;

			for (auto it = m_configuration.begin(), end = m_configuration.end(); it != end; ++it)
			{
				const std::tuple<FboAttachement, FboAttachmentType, TextureInternalFormat> & current = (*it);
				if (std::get<1>(current) == FboAttachmentType::texture)
				{
					Texture2D texture(width, height, std::get<2>(current));
					m_instances[std::get<0>(current)] = std::move(texture);
					textures.push_back({std::get<0>(current), &std::get<1>(m_instances[std::get<0>(current)]) });
				}
				else
				{
					RenderBuffer renderBuffer(width, height, std::get<2>(current));
					m_instances[std::get<0>(current)] = std::move(renderBuffer);
					renderBuffers.push_back({ std::get<0>(current), &std::get<0>(m_instances[std::get<0>(current)]) });
				}
			}
			m_fbo = FrameBufferObject(textures, renderBuffers);
			m_width = width;
			m_height = height;
		}
	};
}