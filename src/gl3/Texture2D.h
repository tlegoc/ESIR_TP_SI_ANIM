#pragma once

#include <GL/glew.h>
#include <filesystem>
#include <exception>
#include <SOIL/SOIL.h>
#include <cassert>
#include <gl3/enums.h>
#include <gl3/proxy/AssociateTextureWithTextureUnit.h>

namespace gl3
{
	/// <summary>
	/// A two dimensional texture class.
	/// </summary>
	class Texture2D
	{
		GLuint m_id;
		GLuint m_sizeX, m_sizeY;
		TextureInternalFormat m_internalFormat;

	public:
		/// <summary>
		/// Generates a new empty texture of size width x height, with internal representation in internalFormat.
		/// The wrapping is set to clampToEdge and the filtering is set to nearest.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="internalFormat">The internal format.</param>
		Texture2D(GLuint width, GLuint height, TextureInternalFormat internalFormat)
		{
			m_internalFormat = internalFormat;
			glGenTextures(1, &m_id);
			glBindTexture(GL_TEXTURE_2D, m_id);
			// Generation of the texture
			//glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, nullptr);
			//if (internalFormat != TextureInternalFormat::depth && internalFormat != TextureInternalFormat::depth16 && internalFormat!=TextureInternalFormat::depth32)
			//{
			//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			//}
			//else
			//{
			//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_FLOAT, nullptr);
			//}
			std::pair<GLenum, GLenum> internalFormatInfo = textureInternalFormatBaseType(internalFormat);
			glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(internalFormat), width, height, 0, static_cast<GLenum>(internalFormatInfo.first), static_cast<GLenum>(internalFormatInfo.second), nullptr);
			// We set the default texture options
			glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(TextureWrappingTarget::wrapS), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(TextureWrappingTarget::wrapT), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(TextureFilterMode::nearest));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(TextureFilterMode::nearest));
			// We unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
			// We store the dimensions of the texture
			m_sizeX = width;
			m_sizeY = height;
		}

		/// <summary>
		/// Generates a new texture from an image file. The clamping is set to repeat by default.
		/// The interpolation is set to linear, except if generateMipmaps is true. In that case, the nim filter
		/// if set to linearMipmapLinear (it should be the best quality).
		/// </summary>
		/// <param name="textureFile">The texture file.</param>
		/// <param name="channels">The number of channels in the texture (1 = red, 2 = red/green, 3 = red/green/blue, 4 = red/green/blue/alpha).</param>
		/// <param name="generateMipmaps">if set to <c>true</c> generate mipmaps.</param>
		Texture2D(std::filesystem::path textureFile, size_t channels = 4, bool generateMipmaps = false)
		{
			assert(channels <= 4 && channels > 0);
			switch (channels)
			{
			case 1:
				m_internalFormat = TextureInternalFormat::red;
				break;
			case 2:
				m_internalFormat = TextureInternalFormat::rg;
				break;
			case 3:
				m_internalFormat = TextureInternalFormat::rgb;
				break;
			case 4:
				m_internalFormat = TextureInternalFormat::rgba;
				break;
			}
			if (!std::filesystem::exists(textureFile))
			{
				std::cerr << "Texture2D: could not find texture " << textureFile.string() << std::endl;
				throw std::ios_base::failure("Texture2D: " + std::string("File ") + textureFile.string() + " not found.");
			}
			// With SOIL
			size_t flag = SOIL_FLAG_POWER_OF_TWO;
			if (generateMipmaps) { flag = flag | SOIL_FLAG_MIPMAPS; }
			m_id = SOIL_load_OGL_texture(textureFile.string().c_str(), channels, 0, flag);
			if (m_id == 0)
			{
				std::cerr << "Texture2D: SOIL could not load texture:" << textureFile.string() << std::endl;
				throw std::ios_base::failure("Texture2D: SOIL library is unable to load file " + textureFile.string());
			}
			// With SOIL
			bind();
			set(TextureWrappingTarget::wrapS, TextureWrappingMode::repeat);
			set(TextureWrappingTarget::wrapT, TextureWrappingMode::repeat);
			if (!generateMipmaps)
			{
				set(TextureFilterTarget::minFilter, TextureFilterMode::linear);
			}
			else
			{
				set(TextureFilterTarget::minFilter, TextureFilterMode::linearMipmapLinear); // Best quality
			}
			set(TextureFilterTarget::magFilter, TextureFilterMode::linear);
			// Retrieves the size of the texture
			GLint sizeX, sizeY;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sizeX);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &sizeY);
			m_sizeX = sizeX;
			m_sizeY = sizeY;

			unbind();
		}

		/// <summary>
		/// Default constructor that creates an invalid texture.
		/// </summary>
		Texture2D()
			: m_id(0)
		{}

		/// <summary>
		/// Finalizes an instance of the <see cref="Texture2D"/> class.
		/// </summary>
		~Texture2D()
		{
			if(m_id!=0) glDeleteTextures(1, &m_id);
		}

		/// <summary>
		/// Forbids the copy constructor.
		/// </summary>
		/// <param name="">The .</param>
		Texture2D(const Texture2D &) = delete;

		/// <summary>
		/// Forbids the assignment operator.
		/// </summary>
		/// <param name="">The .</param>
		/// <returns></returns>
		Texture2D & operator = (const Texture2D &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other texture.</param>
		Texture2D(Texture2D && other)
		{
			m_id = other.m_id;
			m_sizeX = other.m_sizeX;
			m_sizeY = other.m_sizeY;
			m_internalFormat = other.m_internalFormat;
			other.m_id = 0;
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other texture.</param>
		/// <returns></returns>
		Texture2D & operator = (Texture2D && other)
		{
			if (m_id != 0) glDeleteTextures(1, &m_id);
			m_id = other.m_id;
			m_sizeX = other.m_sizeX;
			m_sizeY = other.m_sizeY;
			m_internalFormat = other.m_internalFormat;
			other.m_id = 0;
			return *this;
		}

		/// <summary>
		/// Determines whether this texture is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this texture is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_id != 0; }

		/// <summary>
		/// Sets the wrapping mode of the texture.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="mode">The mode.</param>
		void set(TextureWrappingTarget target, TextureWrappingMode mode)
		{
			assert(isValid());
			bind();
			glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(target), static_cast<GLenum>(mode));
			unbind();
		}

		/// <summary>
		/// Sets the filter mode of the texture.
		/// </summary>
		/// <param name="target">The target.</param>
		/// <param name="mode">The mode.</param>
		void set(TextureFilterTarget target, TextureFilterMode mode)
		{
			assert(isValid());
			bind();
			glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(target), static_cast<GLenum>(mode));
			unbind();
		}


		/// <summary>
		/// Binds this texture.
		/// </summary>
		void bind() const
		{
			assert(isValid());
			glBindTexture(GL_TEXTURE_2D, m_id);
		}

		/// <summary>
		/// Unbinds this texture.
		/// </summary>
		void unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/// <summary>
		/// Gets the identifier of the texture.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { return m_id; }

		/// <summary>
		/// Associates this texture to a texture unit.
		/// </summary>
		/// <param name="unitNumber">The texture unit number.</param>
		void associateWithTextureUnit(GLuint unitNumber) const
		{
			glActiveTexture(GL_TEXTURE0 + unitNumber);
			bind();
		}

		/// <summary>
		/// Returns a proxy enabling to associate this texture with a provided texture unit.
		/// </summary>
		/// <returns></returns>
		proxy::AssociateTextureWithTextureUnit associateWithTextureUnitProxy() const
		{
			return [this](GLuint unit) { associateWithTextureUnit(unit); };
		}

		/// <summary>
		/// Returns the width of the texture
		/// </summary>
		/// <returns></returns>
		GLuint getWidth() const { return m_sizeX; }

		/// <summary>
		/// Returns the height of this texture.
		/// </summary>
		/// <returns></returns>
		GLuint getHeight() const { return m_sizeY; }
	};
}
