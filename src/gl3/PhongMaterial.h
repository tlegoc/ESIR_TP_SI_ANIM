#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <gl3/Texture2D.h>

namespace gl3
{
	/// <summary>
	/// Description of a Phong material with emissive, diffuse and specular colors.
	/// </summary>
	class PhongMaterial
	{
	protected:
		glm::vec3 m_emissive;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		GLfloat m_shininess;
		Texture2D * m_diffuseTexture;
		Texture2D * m_specularTexture;
		Texture2D * m_normalTexture;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PhongMaterial"/> class.
		/// </summary>
		PhongMaterial(bool useDefaultTextures=true)
			: m_emissive(0), m_ambient(0), m_diffuse(0), m_specular(0), m_shininess(1.0), m_diffuseTexture(nullptr), m_specularTexture(nullptr), m_normalTexture(nullptr)
		{
			if (useDefaultTextures)
			{
				m_diffuseTexture = TextureServer::getSingleton()->loadTexture(Config::defaultWhiteTexture());
				m_specularTexture = TextureServer::getSingleton()->loadTexture(Config::defaultWhiteTexture());
				m_normalTexture = TextureServer::getSingleton()->loadTexture(Config::defaultNormalMap());
			}
		}

		/// <summary>
		/// Gets the emissive color.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & getEmissive() const { return m_emissive; }

		/// <summary>
		/// Sets the emissive color.
		/// </summary>
		/// <param name="emissive">The emissive.</param>
		void setEmissive(const glm::vec3 & emissive) { m_emissive = emissive; }
		
		/// <summary>
		/// Gets the ambient color.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & getAmbient() const { return m_ambient; }

		/// <summary>
		/// Sets the ambient color.
		/// </summary>
		/// <param name="ambient">The ambient color.</param>
		void setAmbient(const glm::vec3 & ambient) { m_ambient = ambient; }

		/// <summary>
		/// Gets the diffuse color.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & getDiffuse() const { return m_diffuse; }

		/// <summary>
		/// Sets the diffuse color.
		/// </summary>
		/// <param name="diffuse">The diffuse.</param>
		void setDiffuse(const glm::vec3 & diffuse) { m_diffuse = diffuse; }

		/// <summary>
		/// Gets the specular color.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & getSpecular() const { return m_specular; }

		/// <summary>
		/// Sets the specular color.
		/// </summary>
		/// <param name="specular">The specular.</param>
		void setSpecular(const glm::vec3 & specular) { m_specular = specular; }

		/// <summary>
		/// Gets the shininess.
		/// </summary>
		/// <returns></returns>
		GLfloat getShininess() const { return m_shininess; }

		/// <summary>
		/// Sets the shininess.
		/// </summary>
		/// <param name="shininess">The shininess.</param>
		void setShininess(GLfloat shininess) { m_shininess = shininess; }

		/// <summary>
		/// Gets the diffuse texture.
		/// </summary>
		/// <returns></returns>
		Texture2D * getDiffuseTexture() const { return m_diffuseTexture; }

		/// <summary>
		/// Sets the diffuse texture.
		/// </summary>
		/// <param name="texture">The texture.</param>
		void setDiffuseTexture(Texture2D * texture) { m_diffuseTexture = texture; }

		/// <summary>
		/// Gets the specular texture.
		/// </summary>
		/// <returns></returns>
		Texture2D * getSpecularTexture() const { return m_specularTexture; }

		/// <summary>
		/// Sets the specular texture.
		/// </summary>
		/// <param name="texture">The texture.</param>
		void setSpecularTexture(Texture2D * texture) { m_specularTexture = texture; }

		/// <summary>
		/// Gets the normal texture.
		/// </summary>
		/// <returns></returns>
		Texture2D * getNormalTexture() const { return m_normalTexture; }

		/// <summary>
		/// Sets the normal texture.
		/// </summary>
		/// <param name="texture">The texture.</param>
		void setNormalTexture(Texture2D * texture) { m_normalTexture = texture; }
	};
}