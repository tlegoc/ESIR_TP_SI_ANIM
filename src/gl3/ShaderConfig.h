#pragma once

#include <string>

namespace gl3
{
	/// <summary>
	/// Default names and configuration for the shaders (see shader text file Convention.txt in shader directory)
	/// </summary>
	class ShaderConfig
	{
	public:
		// ------
		// Inputs
		// ------

		/// <summary>
		/// Gets the vertex position string.
		/// </summary>
		/// <returns></returns>
		inline static std::string inPositionString() { return "in_position"; }
		/// <summary>
		/// Gets the vertex normal string.
		/// </summary>
		/// <returns></returns>
		inline static std::string inNormalString() { return "in_normal"; }
		/// <summary>
		/// Gets the tangent string.
		/// </summary>
		/// <returns></returns>
		inline static std::string inTangentString() { return "in_tangent"; }
		/// <summary>
		/// Gets the bitagent string.
		/// </summary>
		/// <returns></returns>
		inline static std::string inBitagentString() { return "in_bitangent"; }
		/// <summary>
		/// Gets the texture coordinates string.
		/// </summary>
		/// <returns></returns>
		inline static std::string inTextureCoordinatesString() { return "in_textureCoordinates"; }

		// --------
		// Uniforms
		// --------

		/// <summary>
		/// Gets the model matrix string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniModelMatrixString() { return "uni_mat_model"; }
		/// <summary>
		/// Gets the view matrix string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniViewMatrixString() { return "uni_mat_view"; }
		/// <summary>
		/// Gets the projection matrix string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniProjectionMatrixString() { return "uni_mat_projection"; }
		/// <summary>
		/// Gets the normal matrix string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniNormalMatrixString() { return "uni_mat_normal"; }

		/// <summary>
		/// Gets the emissive material component string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniEmissiveColorString() { return "uni_material.emissiveColor"; }
		/// <summary>
		/// Gets the ambient color material component string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniAmbientColorString() { return "uni_material.ambientColor"; }
		/// <summary>
		/// Gets the diffuse color material component string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniDiffuseColorString() { return "uni_material.diffuseColor"; }
		/// <summary>
		/// Gets the specular color material component string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniSpecularColorString() { return "uni_material.specularColor"; }
		/// <summary>
		/// Gets the shininess material component string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniShininessString() { return "uni_material.shininess"; }

		/// <summary>
		/// Gets the diffuse texture string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniDiffuseTextureString() { return "uni_diffuseTexture"; }
		/// <summary>
		/// Gets the specular texture string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniSpecularTextureString() { return "uni_specularTexture"; }
		/// <summary>
		/// Gets the normal texture string.
		/// </summary>
		/// <returns></returns>
		inline static std::string uniNormalTextureString() { return "uni_normalTexture"; }
	};
}
