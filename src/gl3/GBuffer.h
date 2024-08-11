#pragma once

#include <gl3/FrameBuffer.h>

namespace gl3
{
	/// <summary>
	/// A frame buffer having the characteristics of a gBuffer
	/// Color attachment 0 : positions (usually in world coordinates)  [RGB32F]
	/// Color attachment 1 : normals (usually in world coordinates) [RGB32F]
	/// Color attachment 2 : diffuse color [RGBA]
	/// Color attachment 3 : specular color [RGBA]
	/// Color attachment 4 : z value [R32F]
	/// depth attachment : render buffer de type z buffer 
	/// </summary>
	/// <seealso cref="FrameBuffer" />
	class GBuffer : public FrameBuffer
	{
		const Texture2D * m_positions;
		const Texture2D * m_normals;
		const Texture2D * m_diffuse;
		const Texture2D * m_specular;
		const Texture2D * m_depth;

		static std::vector<std::tuple<FboAttachement, FboAttachmentType, TextureInternalFormat>> configuration()
		{
			std::vector<std::tuple<FboAttachement, FboAttachmentType, TextureInternalFormat>> result;
			// Texture for positions
			result.push_back({ FboAttachement::colorAttachment0, FboAttachmentType::texture, TextureInternalFormat::rgb32f });
			// Texture for normals
			result.push_back({ FboAttachement::colorAttachment1, FboAttachmentType::texture, TextureInternalFormat::rgb32f });
			// Diffuse color
			result.push_back({ FboAttachement::colorAttachment2, FboAttachmentType::texture, TextureInternalFormat::rgba });
			// Specular color
			result.push_back({ FboAttachement::colorAttachment3, FboAttachmentType::texture, TextureInternalFormat::rgba });
			// Depth
			result.push_back({ FboAttachement::colorAttachment4, FboAttachmentType::texture, TextureInternalFormat::r32f });
			// z buffer
			result.push_back({ FboAttachement::depthAttachment, FboAttachmentType::renderBuffer, TextureInternalFormat::depth });
			return result;
		}

		void updateAttachments()
		{
			m_positions = getTexture(FboAttachement::colorAttachment0);
			m_normals = getTexture(FboAttachement::colorAttachment1);
			m_diffuse = getTexture(FboAttachement::colorAttachment2);
			m_specular = getTexture(FboAttachement::colorAttachment3);
			m_depth = getTexture(FboAttachement::colorAttachment4);
		}

	public:
		/// <summary>
		/// Default constructor that creates an invalid g-buffer
		/// </summary>
		GBuffer()
			: m_positions(nullptr), m_normals(nullptr), m_diffuse(nullptr), m_specular(nullptr)
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="GBuffer"/> class.
		/// </summary>
		/// <param name="width">The width of the screen.</param>
		/// <param name="height">The height of the screen.</param>
		GBuffer(size_t width, size_t height)
			: FrameBuffer(configuration(), width, height)
		{
			updateAttachments();
		}

		/// <summary>
		/// Move constructor.
		/// </summary>
		GBuffer(GBuffer && other)
			: FrameBuffer(std::move(other))
		{
			updateAttachments();
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <returns></returns>
		virtual FrameBuffer & operator= (GBuffer && other)
		{
			FrameBuffer::operator=(std::move(other));
			if (isValid()) { updateAttachments(); }
			return (*this);
		}

		/// <summary>
		/// Resizes the gBuffer. 
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		virtual void resize(size_t width, size_t height) override
		{
			FrameBuffer::resize(width, height);
			updateAttachments();
		}

		/// <summary>
		/// Gets the positions texture
		/// </summary>
		/// <returns></returns>
		const Texture2D * getPositions() const { return m_positions; }

		/// <summary>
		/// Gets the normals texture.
		/// </summary>
		/// <returns></returns>
		const Texture2D * getNormals() const { return m_normals; }

		/// <summary>
		/// Gets the diffuse texture.
		/// </summary>
		/// <returns></returns>
		const Texture2D * getDiffuse() const { return m_diffuse; }

		/// <summary>
		/// Gets the specular texture.
		/// </summary>
		/// <returns></returns>
		const Texture2D * getSpecular() const { return m_specular; }

		/// <summary>
		/// Gets the depth texture (warning, it has to be written).
		/// </summary>
		/// <returns></returns>
		const Texture2D * getDepth() const { return m_depth; }
	};
}