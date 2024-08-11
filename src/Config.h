#ifndef _Config_H
#define _Config_H

#include <GL/freeglut.h>
#include <System/Path.h>
#include <filesystem>
#include <iostream>

namespace Config
{
	/// \brief	Full pathname of the executable file.
	inline std::filesystem::path executablePath() { return System::Path::executable(); }

	/// \brief	Full pathname of the data directory.
	inline ::std::filesystem::path dataPath() 
	{
		char * path = getenv("AnimRendu_DATA");
		if (path == nullptr)
		{
			std::cerr << "Error when retrieving AnimRendu_DATA environment variable, switching to relative path" << std::endl;
			return executablePath() / ".." / ".." / ".." / "data";
		}
		return ::std::string(path); 
	}

	/// \brief	The Phong vertex shader file.
	inline std::filesystem::path PhongVertexShaderFile() { return dataPath() / "Shaders" / "Phong" / "Phong.vert"; }

	/// \brief	The Phong fragment shader file.
	inline std::filesystem::path PhongFragmentShaderFile() { return dataPath() / "Shaders" / "Phong" / "Phong.frag"; }

	/// <summary>
	/// Path to a default white texture
	/// </summary>
	/// <returns></returns>
	inline std::filesystem::path defaultWhiteTexture() { return dataPath() / "textures" / "default" / "white.png"; }

	/// <summary>
	/// Path to a default black texture.
	/// </summary>
	/// <returns></returns>
	inline std::filesystem::path defaultBlackTexture() { return dataPath() / "textures" / "default" / "black.png"; }

	/// <summary>
	/// Path to a default normal map (flat one that should keep the normal orientation).
	/// </summary>
	/// <returns></returns>
	inline std::filesystem::path defaultNormalMap() { return dataPath() / "textures" / "default" / "normals.png"; }

	/// \brief	The default openGL window width.
	static unsigned int defaultWindowWidth = 400 ;

	/// \brief	The default openGL window height.
	static unsigned int defaultWindowHeight = 400 ;

	/// \brief	The default near plane distance.
	static GLfloat defaultNearPlane = 0.01f ;

	/// \brief	The default far plane distance.
	static GLfloat defaultFarPlane = 1000.0f ;

	/// \brief	The default fovy.
	static GLfloat defaultFovy = 65.0f ;
}

#endif
