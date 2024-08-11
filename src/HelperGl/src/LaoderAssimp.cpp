#include <HelperGl/LoaderAssimp.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace HelperGl
{
	static Color convert(const aiColor3D & color)
	{
		return Color(color.r, color.g, color.b);
	}

	static std::pair<std::string, Material *> convert(const aiMaterial & material, const std::string & texturePath)
	{
		Material * result = new Material();
		aiColor3D color;
		aiString name;
		float value;
		material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
		result->setDiffuse(convert(color));
		material.Get(AI_MATKEY_COLOR_SPECULAR, color);
		result->setSpecular(convert(color));
		material.Get(AI_MATKEY_COLOR_AMBIENT, color);
		result->setAmbiant(convert(color));
		//result->setAmbient(convert(color));
		material.Get(AI_MATKEY_COLOR_EMISSIVE, color);
		result->setEmission(convert(color));
		//result->setEmissive(convert(color));
		material.Get(AI_MATKEY_SHININESS, value);
		result->setShininess(value);
		material.Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name); // We only consider the diffuse texture...
		if (name.length != 0)
		{
			//result->setTextureFile(texturePath + '\\' + name.C_Str());
			result->setTexture(texturePath + '\\' + name.C_Str());
		}

		material.Get(AI_MATKEY_NAME, name);
		//result->setName(name.C_Str());
		return std::make_pair(std::string(name.C_Str()), result);
	}

	static Math::Vector3f convert(const aiVector3D & v)
	{
		return Math::makeVector(v.x, v.y, v.z);
	}

	static Mesh * convert(const aiMesh & mesh, const std::vector<Material*> & materials)
	{
		Mesh * result = new Mesh();
		Material * material = materials[mesh.mMaterialIndex];
		result->setMaterial(material);
		//Math::Vector3f * normals;
		
		std::vector<Math::Vector3f> vertices;
		for (size_t cpt = 0; cpt < mesh.mNumVertices; ++cpt)
		{
			vertices.push_back(convert(mesh.mVertices[cpt]));
			//result->addVertex(convert(mesh.mVertices[cpt]));
		}
		result->setVertices(vertices);

		if (mesh.HasNormals())
		{
			//normals = new Math::Vector3f[mesh.mNumVertices];
			std::vector<Math::Vector3f> normals;
			for (size_t cpt = 0; cpt < mesh.mNumVertices; ++cpt)
			{
				normals.push_back(convert(mesh.mNormals[cpt]));
				//result->addNormal(convert(mesh.mNormals[cpt]));
			}
			result->setVerticesNormals(normals);
		}

		if (mesh.HasTextureCoords(0))
		{
			std::vector<Math::Vector2f> texCoord;
			for (size_t cpt = 0; cpt < mesh.mNumVertices; ++cpt)
			{
				texCoord.push_back(Math::makeVector(mesh.mTextureCoords[0][cpt][0], mesh.mTextureCoords[0][cpt][1]));
				//result->addTextureCoordinates(Math::makeVector(mesh.mTextureCoords[0][cpt][0], mesh.mTextureCoords[0][cpt][1]));
			}
			result->setTextureCoordinates(texCoord);
		}

		std::vector<Mesh::Face> faces;
		for (size_t cpt = 0; cpt < mesh.mNumFaces; ++cpt)
		{
			//Geometry::VertexDescriptor descriptor[3];
			Mesh::Face tmpFace;
			const aiFace & face = mesh.mFaces[cpt];
			if (face.mNumIndices != 3) { continue; }
			for (size_t i = 0; i < 3; ++i)
			{
				tmpFace.m_verticesIndexes[i] = face.mIndices[i];
				//descriptor[i].vertex = face.mIndices[i];
				if (mesh.HasTextureCoords(0)) { tmpFace.m_textureIndexes[i] = face.mIndices[i]; }
				//if (mesh.HasTextureCoords(0)) { descriptor[i].uvCoordinates = face.mIndices[i]; }
				//if (mesh.HasNormals()) { descriptor[i].normal = face.mIndices[i]; }
			}
			//result->addTriangle(descriptor[0], descriptor[1], descriptor[2], material);
			faces.push_back(tmpFace);
		}
		result->setFaces(faces);

		return result;
	}



	LoaderAssimp::LoaderAssimp(const ::std::filesystem::path & filename, const ::std::filesystem::path & texturePath)
	{
		::std::cout << "Loading and preprocessing file: " << filename << " ... " << std::endl;
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
			//aiProcess_FindDegenerates |
			//aiProcess_FindInvalidData |
			aiProcess_TransformUVCoords |
			aiProcess_CalcTangentSpace
		);
		if (!scene)
		{
			std::cout << "ERROR" << std::endl;
			return;
		}
		else
		{
			std::cout << "OK" << std::endl;
		}
		// We process the materials
		for (size_t cpt = 0; cpt < scene->mNumMaterials; ++cpt)
		{
			m_materials.push_back(convert(*scene->mMaterials[cpt], texturePath.string()).second);
		}
		// We process the geometries
		for (size_t cpt = 0; cpt < scene->mNumMeshes; ++cpt)
		{
			m_meshes.push_back(convert(*scene->mMeshes[cpt], m_materials));
		}
	}

}