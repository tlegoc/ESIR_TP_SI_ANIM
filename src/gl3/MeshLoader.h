#pragma once

#include <filesystem>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <gl3/Mesh.h>
#include <gl3/TextureServer.h>
#include <gl3/PhongMaterial.h>
#include <exception>
#include <vector>
#include <System/SearchPaths.h>

namespace gl3
{
	/// <summary>
	/// A mesh loader based on Assimp library.
	/// </summary>
	class MeshLoader
	{
		System::SearchPaths m_geometryPaths;
		System::SearchPaths m_texturePaths;
		std::vector<PhongMaterial*> m_materials;
		std::vector<std::pair<Mesh*, PhongMaterial*>> m_meshes;

	public:
		using iterator = std::vector<std::pair<Mesh*, PhongMaterial*>>::iterator;
		using const_iterator = std::vector<std::pair<Mesh*, PhongMaterial*>>::const_iterator;

		/// <summary>
		/// Returns an iterator to the first mesh.
		/// </summary>
		/// <returns></returns>
		iterator begin() { return m_meshes.begin(); }

		/// <summary>
		/// Returns an iterator to past the end of the mesh list.
		/// </summary>
		/// <returns></returns>
		iterator end() { return m_meshes.end(); }

		/// <summary>
		/// Returns an iterator to the first mesh.
		/// </summary>
		/// <returns></returns>
		const_iterator begin() const { return m_meshes.begin(); }

		/// <summary>
		/// Returns an iterator to past the end of the mesh list.
		/// </summary>
		/// <returns></returns>
		const_iterator end() const { return m_meshes.end(); }

		/// <summary>
		/// Initializes a new instance of the <see cref="MeshLoader"/> class.
		/// </summary>
		/// <param name="geometryPaths">The geometries search paths.</param>
		/// <param name="texturePaths">The textures search paths.</param>
		MeshLoader(const System::SearchPaths & geometryPaths = System::SearchPaths(), const System::SearchPaths & texturePaths = System::SearchPaths())
			: m_geometryPaths(geometryPaths), m_texturePaths(texturePaths)
		{}

		/// <summary>
		/// Adds a search path for the geometries.
		/// </summary>
		/// <param name="path">The path.</param>
		void addGeometryPath(const std::filesystem::path & path)
		{
			m_geometryPaths.push(path);
		}

		/// <summary>
		/// Adds a search path for the textures.
		/// </summary>
		/// <param name="path">The path.</param>
		void addTexturePath(const std::filesystem::path & path)
		{
			m_texturePaths.push(path);
		}

		/// <summary>
		/// Loads the specified filename.
		/// </summary>
		/// <param name="filename">The filename.</param>
		/// <returns> An iterator range containing the loaded meshes. </returns>
		std::pair<const_iterator, const_iterator> load(std::filesystem::path filename)
		{
			filename = m_geometryPaths.findFile(filename);
			if (!std::filesystem::exists(filename))
			{
				std::cerr << "MeshLoader: could not find file " << filename.string() << std::endl;
				throw std::ios_base::failure(std::string("MeshLoader: could not find file ") + filename.string());
			}
			{
				std::filesystem::path tmp = filename;
				tmp.remove_filename();
				m_texturePaths.push(tmp);
			}
			std::cout << "MeshLodaer: loading file " << filename.string() << "..." << std::flush;
			Assimp::Importer importer;
			const aiScene * scene = importer.ReadFile(filename.string(),
				aiProcess_JoinIdenticalVertices |
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_PreTransformVertices |
				aiProcess_ImproveCacheLocality |
				aiProcess_RemoveRedundantMaterials |
				//aiProcess_FixInfacingNormals |
				//aiProcess_SortByPType |
				aiProcess_FindDegenerates |
				aiProcess_FindInvalidData |
				aiProcess_TransformUVCoords |
				aiProcess_CalcTangentSpace |
				aiProcess_OptimizeMeshes |
				aiProcess_OptimizeGraph
			);
			if (!scene)
			{
				std::cout << "NOT OK" << std::endl << "MeshLoader: Assimp could not load the scene "<<filename.string() << std::endl;
				m_texturePaths.pop();
				throw std::ios_base::failure("gl3::Loader: Assimp could not load file " + filename.string());
			}
			std::cout << "OK" << std::endl;

			// We save the starting points of the materials associated with this file
			size_t materialOffset = m_materials.size();
			size_t meshOffset = m_meshes.size();

			// We process the materials
			for (size_t cpt = 0; cpt < scene->mNumMaterials; ++cpt)
			{
				m_materials.push_back(convert(*scene->mMaterials[cpt]));
			}
			// We process the geometries
			for (size_t cpt = 0; cpt < scene->mNumMeshes; ++cpt)
			{
				m_meshes.push_back(convert(*scene->mMeshes[cpt], materialOffset));
			}
			m_texturePaths.pop();

			return { m_meshes.begin() + meshOffset, m_meshes.end() };
		}

	private:
		static glm::vec3 convert(const aiColor3D & color)
		{
			return glm::vec3(color.r, color.g, color.b);
		}

		static std::string convert(const aiString & str)
		{
			return str.C_Str();
		}

		static glm::vec3 convert(const aiVector3D & v)
		{
			return glm::vec3(v.x, v.y, v.z);
		}

		static glm::vec2 convert(const aiVector2D & v)
		{
			return glm::vec2(v.x, v.y);
		}

		PhongMaterial * convert(const aiMaterial & material)
		{
			PhongMaterial * result = new PhongMaterial;
			aiColor3D color;
			aiString name;
			float value = 1.0;
			// Usual Phong properties
			if (material.Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
			{
				result->setDiffuse(convert(color));
			}
			if(material.Get(AI_MATKEY_COLOR_SPECULAR, color)==AI_SUCCESS)
				result->setSpecular(convert(color));
			if(material.Get(AI_MATKEY_COLOR_AMBIENT, color)==AI_SUCCESS)
				result->setAmbient(convert(color));
			if(material.Get(AI_MATKEY_COLOR_EMISSIVE, color)==AI_SUCCESS)
				result->setEmissive(convert(color));
			if (material.Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
			{
				result->setShininess(value);
			}
			else
			{
				result->setShininess(1.0f);
			}
			// Texture loading
			material.Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
			if (name.length != 0)
			{
				try
				{
					std::filesystem::path path = m_texturePaths.findFile(convert(name));
					Texture2D * diffuseTexture = TextureServer::getSingleton()->loadTexture(path, 4, true);
					result->setDiffuseTexture(diffuseTexture);
				}
				catch (const std::exception & e)
				{
					std::cerr << "MeshLoader: problem while loading diffuse texture, setting default white texture" << std::endl;
					std::cerr << "\tError" << e.what() << std::endl;
				}
			}
			material.Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), name);
			if (name.length != 0)
			{
				try
				{
					std::filesystem::path path = m_texturePaths.findFile(convert(name));
					Texture2D * specularTexture = TextureServer::getSingleton()->loadTexture(path, 4, true);
					result->setSpecularTexture(specularTexture);
				}
				catch (const std::exception & e)
				{
					std::cerr << "MeshLoader: problem while loading specular texture, setting default white texture" << std::endl;
					std::cerr << "\tError" << e.what() << std::endl;
				}
			}
			material.Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), name);
			if (name.length != 0)
			{
				try
				{
					std::filesystem::path path = m_texturePaths.findFile(convert(name));
					Texture2D * normalTexture = TextureServer::getSingleton()->loadTexture(path, 4, true);
					result->setNormalTexture(normalTexture);
				}
				catch (const std::exception & e)
				{
					std::cerr << "MeshLoader: problem while loading normals texture, setting to default normal texture" << std::endl;
					std::cerr << "\tError" << e.what() << std::endl;
				}
			}
			return result;
		}

		std::pair<Mesh *, PhongMaterial *> convert(const aiMesh & mesh, size_t materialOffset)
		{
			Mesh * result = new Mesh;
			PhongMaterial * material = m_materials[materialOffset + mesh.mMaterialIndex];
			// We extract the structure of the mesh
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> tangents;
			std::vector<glm::vec3> bitangents;
			std::vector<glm::vec2> textureCoordinates;
			vertices.reserve(mesh.mNumVertices);
			normals.reserve(mesh.mNumVertices);
			tangents.reserve(mesh.mNumVertices);
			bitangents.reserve(mesh.mNumVertices);
			textureCoordinates.reserve(mesh.mNumVertices);
			for (size_t cpt = 0; cpt < mesh.mNumVertices; ++cpt)
			{
				vertices.push_back(convert(mesh.mVertices[cpt]));
				if (mesh.HasNormals()) { normals.push_back(convert(mesh.mNormals[cpt])); }
				if (mesh.HasTangentsAndBitangents())
				{
					tangents.push_back(convert(mesh.mTangents[cpt]));
					bitangents.push_back(convert(mesh.mBitangents[cpt]));
				}
				if (mesh.HasTextureCoords(0)) { textureCoordinates.push_back(glm::vec2(mesh.mTextureCoords[0][cpt][0], mesh.mTextureCoords[0][cpt][1])); }
			}
			// We setup the mesh
			result->setVertices(vertices);
			if (normals.size() > 0) { result->setNormals(normals); }
			if (tangents.size() > 0) { result->setTagents(tangents); }
			if (bitangents.size() > 0) { result->setBitagents(bitangents); }
			if (textureCoordinates.size() > 0) { result->setTextureCoodinates(textureCoordinates); }

			std::vector<GLuint> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (int cpt = 0; cpt < mesh.mNumFaces; ++cpt)
			{
				const aiFace & face = mesh.mFaces[cpt];
				if (face.mNumIndices != 3) { continue; }
				for (size_t i = 0; i < 3; ++i)
				{
					indices.push_back(face.mIndices[i]);
				}
			}
			if (indices.size() > 0) { result->setIndices(indices); }

			return { result, material };
		}
	};
}