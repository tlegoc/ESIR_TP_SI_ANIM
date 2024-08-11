#ifndef _HelperGl_TextureServer_H
#define _HelperGl_TextureServer_H

#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <map>
#include <string>
#include <iostream>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	TextureServer
	///
	/// \brief	A texture server that loads texture files and registers them in OpenGL. This server ensures
	/// 		the unicity of a texture (i.e. if a texture is loaded several times in the server, this texture
	/// 		will be loaded once and shared between the materials).
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	08/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class TextureServer
	{
	private:
		/// \brief	The loaded textures (filename, openGl identifier).
		::std::map<std::string, GLuint> m_loadedTextures ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	TextureServer::TextureServer()
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		TextureServer()
		{}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static TextureServer * TextureServer::getSingleton()
		///
		/// \brief	Gets the texture server.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \return	The texture server.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static TextureServer * getSingleton();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLuint TextureServer::loadTexture(std::string const & filename)
		///
		/// \brief	Loads a texture.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \param	filename	Filename of the texture.
		///
		/// \return	The texture identifier (0 if a problem occurred).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLuint loadTexture(std::string const & filename);
	};
}

#endif
