#include <HelperGl/Loader3ds.h>

namespace HelperGl
{
	void Loader3ds::loadMaterial( Lib3dsMaterial * material, const ::std::filesystem::path & texturePath )
	{
		Material currentMaterial ;
		Color color ;
		color.set(material->diffuse);
		currentMaterial.setDiffuse(color);
		color.set(material->specular);
		currentMaterial.setSpecular(color);
		color.set(material->ambient);
		currentMaterial.setAmbiant(color);
		currentMaterial.setShininess(material->shininess);
		::std::string textureName = material->texture1_map.name;
		if(textureName!="")
		{
			currentMaterial.setTexture((texturePath/textureName).string());
		}
		::std::string materialName = material->name;
		m_materials[materialName] = new Material(currentMaterial);
		::std::cout<<"Loader3ds: created material "<<materialName<<" shading: "<<material->shading<<::std::endl ;
	}

	void Loader3ds::loadMaterials( const ::std::filesystem::path & texturePath )
	{
		for(Lib3dsMaterial * material = m_file->materials ; material != NULL ; material = material->next)
		{
			loadMaterial(material, texturePath);
		}
	}

	void Loader3ds::loadMesh( Lib3dsMesh * mesh )
	{
		::std::string meshName = mesh->name;
		::std::vector<Math::Vector3f> vertices ;
		::std::vector<Math::Vector2f> texture ;
		// 1 - Converts vertices
		for(unsigned int cpt=0 ; cpt<mesh->points ; ++cpt)
		{
			Lib3dsPoint pt = mesh->pointL[cpt] ;
			Math::Vector3f v = Math::makeVector(pt.pos[0], pt.pos[1], pt.pos[2]) ;
			vertices.push_back(v);
		}
		// 2 - Converts texture coordinates
		for(unsigned int cpt=0 ; cpt<mesh->texels ; ++cpt)
		{
			Math::Vector2f t = Math::makeVector(mesh->texelL[cpt][0], mesh->texelL[cpt][1]);
			texture.push_back(t);
		}
		// 3 - Converts faces
		::std::map<Material*, ::std::vector<Mesh::Face>> faces ;
		for(unsigned int cpt=0 ; cpt<mesh->faces ; ++cpt)
		{
			Lib3dsFace tmp = mesh->faceL[cpt] ;
			Mesh::Face result ;
			std::copy(tmp.points, tmp.points+3, result.m_verticesIndexes) ;
			std::copy(tmp.points, tmp.points+3, result.m_textureIndexes) ;
			result.m_normal = Math::Vector3f(tmp.normal) ;
			faces[m_materials[tmp.material]].push_back(result) ;
		}
		// 4 - Creates one mesh per material
		for(auto it=faces.begin() ; it!=faces.end() ; ++it)
		{
			Mesh * result = new Mesh ;
			result->setName(meshName) ;
			result->setVertices(vertices) ;
			result->setTextureCoordinates(texture) ;
			result->setMaterial(it->first) ;
			result->setFaces(it->second) ;
			result->clean(); // We clean the mesh
			m_meshes.push_back(result) ;
			::std::cout<<"Loader3ds: Created mesh "<<result->getName()<<" with "<<result->getFaces().size()<<" faces and "<<texture.size()<<" texture coordinates"<<::std::endl ;
		}
	}

	Loader3ds::Loader3ds( const ::std::filesystem::path & filename, const ::std::filesystem::path & texturePath )
	{
		::std::cout<<"Loader3ds: loading file "<<filename<<::std::endl; 
		m_file = lib3ds_file_load(filename.string().c_str()) ;
		if(m_file==NULL)
		{
			::std::cerr<<"Loader3ds: unable to load file "+filename.string()<<::std::endl ;
			return ;
		}
		loadMaterials(texturePath) ;
		loadMeshes();
		::std::cout<<"Loader3ds: OK"<<::std::endl ;
	}

	const ::std::vector<Mesh*> & Loader3ds::getMeshes() const
	{
		return m_meshes ;
	}

	::std::vector<Material*> Loader3ds::getMaterials() const
	{
		::std::vector<Material*> result ;
		::std::transform(m_materials.begin(), m_materials.end(), ::std::back_inserter(result), [](::std::pair<::std::string, Material*> const & m) -> Material* { return m.second ; } );
		return result;
	}

}