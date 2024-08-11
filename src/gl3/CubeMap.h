#pragma once

#include <GL/glew.h>
#include <gl3/enums.h>
#include <SOIL/SOIL.h>
#include <unordered_map>
#include <cassert>
#include <gl3/proxy/AssociateTextureWithTextureUnit.h>

namespace gl3
{
	/// <summary>
	/// A cube map texture
	/// </summary>
	class CubeMap
	{
		GLuint m_id;

		static bool exists(const std::unordered_map<CubeMapPosition, std::filesystem::path> & textureFiles, CubeMapPosition entry)
		{
			return textureFiles.find(entry) != textureFiles.end();
		}

	public:
		/// <summary>
		/// Default constructor. Warning, the cube map is not valid.
		/// </summary>
		CubeMap()
			: m_id(0)
		{}

		/// <summary>
		/// Constructs a cube map of the provided internal format
		/// </summary>
		/// <param name="width">The width of a face texture.</param>
		/// <param name="height">The height of a face texture.</param>
		/// <param name="format">The internal format of the cube map texture.</param>
		CubeMap(GLuint width, GLuint height, TextureInternalFormat format)
		{
			glGenTextures(1, &m_id);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
			for (GLenum it = GL_TEXTURE_CUBE_MAP_POSITIVE_X, end = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z + 1; it != end; ++it)
			{
				std::pair<GLenum, GLenum> baseTypes = textureInternalFormatBaseType(format);
				glTexImage2D(it, 0, static_cast<GLenum>(format), width, height, 0, baseTypes.first, baseTypes.second, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureFilterTarget::magFilter), static_cast<GLenum>(TextureFilterMode::nearest));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureFilterTarget::minFilter), static_cast<GLenum>(TextureFilterMode::nearest));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapS), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapT), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapR), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		/// <summary>
		///Constructs  a cube map texture based on texture files.
		/// </summary>
		/// <param name="files">The files names.</param>
		CubeMap(const std::unordered_map<CubeMapPosition, std::filesystem::path> & files)
		{
			assert(exists(files, CubeMapPosition::front) && "CubeMap::CubeMap texture file does not exist");
			assert(exists(files, CubeMapPosition::back) && "CubeMap::CubeMap texture file does not exist");
			assert(exists(files, CubeMapPosition::left) && "CubeMap::CubeMap texture file does not exist");
			assert(exists(files, CubeMapPosition::right) && "CubeMap::CubeMap texture file does not exist");
			assert(exists(files, CubeMapPosition::top) && "CubeMap::CubeMap texture file does not exist");
			assert(exists(files, CubeMapPosition::bottom) && "CubeMap::CubeMap texture file does not exist");
			assert(files.size() == 6 && "CubeMap::CubeMap you should provide 6 textures!");
			std::string fileXpositive = files.find(CubeMapPosition::xPositive)->second.string();
			std::string fileXnegative = files.find(CubeMapPosition::xNegative)->second.string();
			std::string fileYpositive = files.find(CubeMapPosition::yPositive)->second.string();
			std::string fileYnegative = files.find(CubeMapPosition::yNegative)->second.string();
			std::string fileZpositive = files.find(CubeMapPosition::zPositive)->second.string();
			std::string fileZnegative = files.find(CubeMapPosition::zNegative)->second.string();

			m_id = SOIL_load_OGL_cubemap(
				fileXpositive.c_str(),
				fileXnegative.c_str(),
				fileYpositive.c_str(),
				fileYnegative.c_str(),
				fileZpositive.c_str(),
				fileZnegative.c_str(),
				SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureFilterTarget::magFilter), static_cast<GLenum>(TextureFilterMode::linear));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureFilterTarget::minFilter), static_cast<GLenum>(TextureFilterMode::linearMipmapLinear));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapS), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapT), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, static_cast<GLenum>(TextureWrappingTarget::wrapR), static_cast<GLenum>(TextureWrappingMode::clampToEdge));
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref="CubeMap"/> class.
		/// </summary>
		~CubeMap()
		{
			if (m_id != 0) { glDeleteTextures(1, &m_id); }
		}

		/// <summary>
		/// Forbids the copy constructor.
		/// </summary>
		/// <param name="">The .</param>
		CubeMap(const CubeMap &) = delete;

		/// <summary>
		/// Forbids the assignment operator.
		/// </summary>
		/// <param name="">The .</param>
		/// <returns></returns>
		CubeMap & operator= (const CubeMap &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		CubeMap(CubeMap && other)
			: m_id(other.m_id)
		{
			other.m_id = 0;
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		CubeMap & operator= (CubeMap && other)
		{
			m_id = other.m_id;
			other.m_id = 0;
			return *this;
		}

		/// <summary>
		/// Determines whether the cube map is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this texture is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_id != 0; }

		/// <summary>
		/// Binds this cube map.
		/// </summary>
		void bind() const
		{
			assert(isValid() && "CubeMap::bind called but the cube map is not initialized");
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
		}

		/// <summary>
		/// Unbinds this cube map.
		/// </summary>
		void unbind() const
		{
			assert(isValid() && "CubeMap::unbind called but the cube map is not initialized");
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
		}

		/// <summary>
		/// Gets the identifier of the cube map texture.
		/// </summary>
		/// <returns></returns>
		GLuint getId() const { return m_id; }

		/// <summary>
		/// Associates this cube map texture to a texture unit.
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
	};
}