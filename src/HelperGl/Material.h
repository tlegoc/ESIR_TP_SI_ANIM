#ifndef _HelperGl_Material_H
#define _HelperGl_Material_H

#include <HelperGl/Color.h>
#include <HelperGl/TextureServer.h>
#include <iostream>

namespace HelperGl
{
	class Material
	{
	private:
		Color m_diffuse ;
		Color m_specular ;
		Color m_ambiant ;
		float m_shininess ;
		Color m_emission ;
		GLuint m_textureId ;

	public:
		Material()
			: m_shininess(1.0f), m_textureId(0)
		{}

		void setDiffuse(const Color & diffuse)
		{
			m_diffuse = diffuse;
		}

		const Color & getDiffuse() const
		{
			return m_diffuse ;
		}

		void setSpecular(const Color & specular)
		{
			m_specular = specular ; 
		}

		const Color & getSpecular() const
		{
			return m_specular ;
		}

		void setShininess(float shininess)
		{
			m_shininess = shininess ;
		}

		float getShininess() const
		{
			return m_shininess ;
		}

		void setAmbiant(const Color & ambiant) 
		{
			m_ambiant = ambiant ;
		}

		const Color & getAmbiant() const
		{
			return m_ambiant ;
		}

		void setEmission(const Color & emission)
		{
			m_emission = emission ;
		}

		const Color & getEmission() const
		{
			return m_emission ;
		}	

		GLuint getTextureId() const
		{
			return m_textureId;
		}

		bool hasTexture() const
		{
			return m_textureId != 0 ;
		}

		void setTexture(const std::string & textureFile)
		{
			m_textureId = TextureServer::getSingleton()->loadTexture(textureFile);
		}
	};

	inline ::std::ostream & operator<<(::std::ostream & out, const Material & material)
	{
		out<<"{"<<"diffuse: "<<material.getDiffuse()<<" specular: "<<material.getSpecular()<<" shininess: "<<material.getShininess()<<" ambiant: "<<material.getAmbiant() ;
		if(material.hasTexture())
		{
			out<<" textureId: "<<material.getTextureId() ;
		}
		out<<"}";
		return out ;
	}
}

#endif