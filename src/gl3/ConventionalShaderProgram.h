#pragma once

#include <gl3/ShaderProgram.h>
#include <gl3/ShaderConfig.h>
#include <gl3/proxy/AttribPointer.h>

namespace gl3
{
	/// <summary>
	/// A shader program that respects the conventions used in ShaderConfig. It can be used to simplify communication 
	/// between the application and the shader program.
	/// This shader program uses texture units 0, 1 and 2 for texture association.
	/// </summary>
	/// <seealso cref="ShaderProgram" />
	class ConventionalShaderProgram : public ShaderProgram
	{
		GLint inPosition, inNormal, inTangent, inBitangent, inTextureCoordinate;
		GLint uniModelMatrix, uniViewMatrix, uniProjectionMatrix, uniNormalMatrix;
		GLint uniEmissiveColor, uniAmbientColor, uniDiffuseColor, uniSpecularColor, uniShininess;
		GLint uniDiffuseTexture, uniSpecularTexture, uniNormalTexture;

		/// <summary>
		/// Initialises this instance.
		/// </summary>
		void initialise()
		{
			inPosition = getAttributeLocation(ShaderConfig::inPositionString());
			inNormal = getAttributeLocation(ShaderConfig::inNormalString());
			inTangent = getAttributeLocation(ShaderConfig::inTangentString());
			inBitangent = getAttributeLocation(ShaderConfig::inBitagentString());
			inTextureCoordinate = getAttributeLocation(ShaderConfig::inTextureCoordinatesString());

			uniModelMatrix = getUniformLocation(ShaderConfig::uniModelMatrixString());
			uniViewMatrix = getUniformLocation(ShaderConfig::uniViewMatrixString());
			uniProjectionMatrix = getUniformLocation(ShaderConfig::uniProjectionMatrixString());
			uniNormalMatrix = getUniformLocation(ShaderConfig::uniNormalMatrixString());

			uniEmissiveColor = getUniformLocation(ShaderConfig::uniEmissiveColorString());
			uniAmbientColor = getUniformLocation(ShaderConfig::uniAmbientColorString());
			uniDiffuseColor = getUniformLocation(ShaderConfig::uniDiffuseColorString());
			uniSpecularColor = getUniformLocation(ShaderConfig::uniSpecularColorString());
			uniShininess = getUniformLocation(ShaderConfig::uniShininessString());

			uniDiffuseTexture = getUniformLocation(ShaderConfig::uniDiffuseTextureString());
			uniSpecularTexture = getUniformLocation(ShaderConfig::uniSpecularTextureString());
			uniNormalTexture = getUniformLocation(ShaderConfig::uniNormalTextureString());
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ConventionalShaderProgram"/> class.
		/// </summary>
		/// <param name="vertex">The vertex shader.</param>
		/// <param name="fragment">The fragment shader.</param>
		ConventionalShaderProgram(const Shader & vertex, const Shader & fragment)
			: ShaderProgram(vertex, fragment)
		{
			initialise();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ConventionalShaderProgram"/> class.
		/// </summary>
		/// <param name="vertex">The vertex shader.</param>
		/// <param name="geometry">The geometry shader.</param>
		/// <param name="fragment">The fragment shader.</param>
		ConventionalShaderProgram(const Shader & vertex, const Shader & geometry, const Shader & fragment)
			: ShaderProgram(vertex, geometry, fragment)
		{
			initialise();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ConventionalShaderProgram"/> class.
		/// </summary>
		/// <param name="vertexSource">The vertex source.</param>
		/// <param name="fragmentSource">The fragment source.</param>
		ConventionalShaderProgram(const std::filesystem::path & vertexSource, const std::filesystem::path & fragmentSource)
			: ShaderProgram(vertexSource, fragmentSource)
		{
			initialise();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="ConventionalShaderProgram"/> class.
		/// </summary>
		/// <param name="vertexSource">The vertex source.</param>
		/// <param name="geometrySource">The geometry source.</param>
		/// <param name="fragmentSource">The fragment source.</param>
		ConventionalShaderProgram(const std::filesystem::path & vertexSource, const std::filesystem::path & geometrySource, const std::filesystem::path & fragmentSource)
			: ShaderProgram(vertexSource, geometrySource, fragmentSource)
		{
			initialise();
		}

		/// <summary>
		/// Sets the material associated with this shader. Note that this method verifies the capabilities of the shader before
		/// trying to set any uniform.
		/// </summary>
		/// <param name="material">The material.</param>
		void setMaterial(const PhongMaterial & material) const
		{
			// The material properties
			if (uniEmissiveColor!=-1) { setUniform(uniEmissiveColor, material.getEmissive()); }
			if (uniAmbientColor != -1) { setUniform(uniAmbientColor, material.getAmbient()); }
			if (uniDiffuseColor != -1) { setUniform(uniDiffuseColor, material.getDiffuse()); }
			if (uniSpecularColor != -1) { setUniform(uniSpecularColor, material.getSpecular()); }
			if (uniShininess != -1) { setUniform(uniShininess, material.getShininess()); }
			// The textures
			if (uniDiffuseTexture != -1 && material.getDiffuseTexture() != nullptr)
			{
				material.getDiffuseTexture()->associateWithTextureUnit(0);
				setUniform(uniDiffuseTexture, 0);
			}
			if (uniSpecularTexture != -1 && material.getSpecularTexture() != nullptr)
			{
				material.getSpecularTexture()->associateWithTextureUnit(1);
				setUniform(uniSpecularTexture, 1);
			}
			if (uniNormalTexture != -1 && material.getNormalTexture() != nullptr)
			{
				material.getSpecularTexture()->associateWithTextureUnit(2);
				setUniform(uniNormalTexture, 2);
			}
		}

		/// <summary>
		/// Sets the mesh as an input of the shader.
		/// </summary>
		/// <param name="mesh">The mesh.</param>
		void setMesh(const Mesh & mesh)
		{
			if (hasInPosition())
			{
				setPosition(*mesh.getVerticesVbo());
			}
			if (hasInNormal())
			{
				setNormal(*mesh.getNormalsVbo());
			}
			if (hasInTangent())
			{
				setTangent(*mesh.getTagentsVbo());
			}
			if (hasInBitangent())
			{
				setBitangent(*mesh.getBitangentsVbo());
			}
			if (hasInTextureCoordinate())
			{
				setTextureCoordinates(*mesh.getTextureCoordinatesVbo());
			}
		}

		/// <summary>
		/// Sets the transforms associated with this shader. Note that the shader verifies the capabilities of the
		/// shader before setting transforms.
		/// </summary>
		/// <param name="projection">The projection.</param>
		/// <param name="view">The view.</param>
		/// <param name="model">The model.</param>
		void setTransforms(const glm::mat4 & projection, const glm::mat4 & view, const glm::mat4 & model)
		{
			if (uniProjectionMatrix != -1) { setUniform(uniProjectionMatrix, projection); }
			if (uniViewMatrix != -1) { setUniform(uniViewMatrix, view); }
			if (uniModelMatrix != -1) { setUniform(uniModelMatrix, model); }
			if (uniNormalMatrix != -1)
			{
				glm::mat3 normal = glm::transpose(inverse(glm::mat3(view) * glm::mat3(model)));
				setUniform(uniModelMatrix, normal);
			}
		}

		/// <summary>
		/// Determines whether the shader has in position attribute.
		/// </summary>
		/// <returns>
		///   <c>true</c> if [has in position]; otherwise, <c>false</c>.
		/// </returns>
		bool hasInPosition() const { return inPosition != -1; }

		/// <summary>
		/// Binds the buffer to the position attribute
		/// </summary>
		/// <param name="vertices">The vertices.</param>
		void setPosition(const VertexBufferObject & vertices) const
		{ 
			assert(hasInPosition()); 
			vertices.attribPointer(inPosition);
		}

		/// <summary>
		/// Binds a buffer to the position attribute
		/// </summary>
		/// <param name="vertices">The vertices.</param>
		void setPosition(const proxy::AttribPointer & attribPointer)
		{
			assert(hasInPosition());
			attribPointer.attribPointer(inPosition);
		}

		/// <summary>
		/// Determines whether the shader has in normal attribute.
		/// </summary>
		/// <returns>
		///   <c>true</c> if [has in normal]; otherwise, <c>false</c>.
		/// </returns>
		bool hasInNormal() const { return inNormal != -1; }

		/// <summary>
		/// Binds the buffer to the normal attribute
		/// </summary>
		/// <param name="normals">The normals.</param>
		void setNormal(const VertexBufferObject & normals)
		{
			assert(hasInNormal());
			normals.attribPointer(inNormal);
		}

		/// <summary>
		/// Binds a buffer to the normal attribute
		/// </summary>
		/// <param name="fnc">The attribPointer function.</param>
		void setNormal(const proxy::AttribPointer & fnc)
		{
			assert(hasInNormal());
			fnc.attribPointer(inNormal);
		}

		/// <summary>
		/// Determines whether the shader has in tangent attribute.
		/// </summary>
		/// <returns>
		///   <c>true</c> if [has in tangent]; otherwise, <c>false</c>.
		/// </returns>
		bool hasInTangent() const { return inTangent != -1; }

		/// <summary>
		/// Binds the buffer to the tangent attribute.
		/// </summary>
		/// <param name="tangents">The tangents.</param>
		void setTangent(const VertexBufferObject & tangents)
		{
			assert(hasInTangent());
			tangents.attribPointer(inTangent);
		}

		/// <summary>
		/// Binds a buffer to the tangent attribute.
		/// </summary>
		/// <param name="fnc">The attribPointer proxy.</param>
		void setTangent(const proxy::AttribPointer & fnc)
		{
			assert(hasInTangent());
			fnc.attribPointer(inTangent);
		}

		/// <summary>
		/// Determines whether the shader has in bitangent attribute.
		/// </summary>
		/// <returns>
		///   <c>true</c> if [has in bitangent]; otherwise, <c>false</c>.
		/// </returns>
		bool hasInBitangent() const { inBitangent != -1; }

		/// <summary>
		/// Binds the buffer to the bitangent attribute.
		/// </summary>
		/// <param name="bitangents">The bitangents.</param>
		void setBitangent(const VertexBufferObject & bitangents)
		{
			assert(hasInBitangent());
			bitangents.attribPointer(inBitangent);
		}

		/// <summary>
		/// Binds a buffer to the bitangent attribute.
		/// </summary>
		/// <param name="bitangents">The attribPointer proxy.</param>
		void setBitangent(const proxy::AttribPointer & fnc)
		{
			assert(hasInBitangent());
			fnc.attribPointer(inBitangent);
		}

		/// <summary>
		/// Determines whether he shader has in texture coordinate.
		/// </summary>
		/// <returns>
		///   <c>true</c> if [has in texture coordinate]; otherwise, <c>false</c>.
		/// </returns>
		bool hasInTextureCoordinate() const { inTextureCoordinate != -1; }

		/// <summary>
		/// Binds the buffer to the texture coordinates attribute.
		/// </summary>
		/// <param name="textureCoordinates">The texture coordinates.</param>
		void setTextureCoordinates(const VertexBufferObject & textureCoordinates)
		{
			assert(hasInTextureCoordinate());
			textureCoordinates.attribPointer(inTextureCoordinate);
		}

		/// <summary>
		/// Binds a buffer to the texture coordinates attribute.
		/// </summary>
		/// <param name="textureCoordinates">The attrib pointer proxy.</param>
		void setTextureCoordinates(const proxy::AttribPointer & fnc)
		{
			assert(hasInTextureCoordinate());
			fnc.attribPointer(inTextureCoordinate);
		}
	};
}