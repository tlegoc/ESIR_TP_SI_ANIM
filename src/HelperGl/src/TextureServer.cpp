#include <HelperGl/TextureServer.h>

namespace HelperGl
{


	TextureServer * TextureServer::getSingleton()
	{
		static TextureServer server ;
		return &server ;
	}

	GLuint TextureServer::loadTexture( std::string const & filename )
	{
		// If the texture as already been loaded, we return its identifier
		auto f = m_loadedTextures.find(filename) ;
		if(f!=m_loadedTextures.end())
		{
			return f->second;
		}
		// Otherwise, we load the texture
		GLuint result = SOIL_load_OGL_texture(filename.c_str(), 3, 0, SOIL_FLAG_POWER_OF_TWO) ;
		if(result==0)
		{
			::std::cerr<<"TextureServer: failed to load texture "+filename<<::std::endl ;
		}
		else
		{
			m_loadedTextures[filename] = result ;
			::std::cout<<"TextureServer: loaded texture "<<filename<<::std::endl ;
		}
		return result ;
	}

}