#pragma once

#include <map>
#include <filesystem>
#include <tuple>
#include <gl3/Texture2D.h>

namespace gl3
{
	/// <summary>
	/// A class handling textures loaded into memory. It avoids the duplication of the textures. This is a singleton, please see getSingleton method.
	/// </summary>
	class TextureServer
	{
		std::map<std::tuple<std::filesystem::path, size_t, bool>, Texture2D> m_textures;

		/// <summary>
		/// Prevents a default instance of the <see cref="TextureServer"/> class from being created.
		/// </summary>
		TextureServer() {}

		TextureServer(const TextureServer &) = delete;
		TextureServer & operator= (const TextureServer &) = delete;

	public:
		/// <summary>
		/// Gets the singleton.
		/// </summary>
		/// <returns></returns>
		static TextureServer * getSingleton()
		{
			static TextureServer server;
			return &server;
		}

		/// <summary>
		/// Loads a texture if necessary and returns a pointer to .
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <param name="nbChannels">The nb channels.</param>
		/// <param name="generateMipmap">if set to <c>true</c> [generate mipmap].</param>
		/// <returns></returns>
		Texture2D * loadTexture(const std::filesystem::path & filename, size_t nbChannels = 4, bool generateMipmap = false)
		{
			auto found = m_textures.find({ filename, nbChannels, generateMipmap });
			if (found != m_textures.end()) { return &found->second; }
			m_textures[{filename, nbChannels, generateMipmap}] = Texture2D(filename, nbChannels, generateMipmap);
			return &m_textures.find({ filename, nbChannels, generateMipmap })->second;
		}
	};
}