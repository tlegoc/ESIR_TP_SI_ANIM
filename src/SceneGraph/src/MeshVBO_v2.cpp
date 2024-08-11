#include <SceneGraph/MeshVBO_v2.h>
#include <HelperGl/PhongShader.h>
#include <HelperGl/LightServer.h>
#include <HelperGl/Draw.h>

namespace SceneGraph
{


	MeshVBO_v2::MeshVBO_v2( const HelperGl::Mesh * mesh ) : m_mesh(mesh)
	{
		// We compute a map associating indexes to vertices identified by their vertex index and texture coordinate index
		::std::map<::std::pair<size_t,size_t>, size_t> verticesIndex ;
		for(auto it = m_mesh->getFaces().begin() ; it!=m_mesh->getFaces().end() ; ++it)
		{
			const HelperGl::Mesh::Face & face = *it ;
			for(int cpt=0 ; cpt<3 ; ++cpt)
			{
				verticesIndex.insert(::std::make_pair(::std::make_pair(face.m_verticesIndexes[cpt], face.m_textureIndexes[cpt]), verticesIndex.size())) ;
			}
		}
		// We create the VBO 
		unsigned int vboAttributes = HelperGl::VBO::VERTEX3D ;
		if(m_mesh->hasVerticesNormals()) { vboAttributes |= HelperGl::VBO::VERTEX_NORMAL ; }
		if(m_mesh->hasTextureCoordinates()) { vboAttributes |= HelperGl::VBO::TEXTURE2D_COORDINATES ; }
		m_vbo = new HelperGl::VBO(verticesIndex.size(), m_mesh->getFaces().size()*3, vboAttributes);
		// We add necessary information into the VBO : : the vertices and associated data
		{
			unsigned int index =  0;
			for(auto it=verticesIndex.begin() ; it!=verticesIndex.end() ; ++it)
			{
				const Math::Vector3f & vertex = mesh->getVertices()[it->first.first] ;
				m_vbo->set(HelperGl::VBO::VERTEX3D, index, vertex) ;
				if(m_mesh->hasVerticesNormals())
				{
					const Math::Vector3f & normal = mesh->getVerticesNormals()[it->first.first] ;
					m_vbo->set(HelperGl::VBO::VERTEX_NORMAL, index, normal) ;
				}
				if(m_mesh->hasTextureCoordinates())
				{
					const Math::Vector2f & texture = mesh->getTextureCoordinates()[it->first.second] ;
					m_vbo->set(HelperGl::VBO::TEXTURE2D_COORDINATES, index, texture) ;
				}
				++index ;
			}
		}
		// We add necessary information into the VBO : the indexes
		{
			unsigned int index = 0 ;
			for(auto it = m_mesh->getFaces().begin() ; it!=m_mesh->getFaces().end() ; ++it)
			{
				const HelperGl::Mesh::Face & face = *it ;
				for(int cpt=0 ; cpt<3 ; ++cpt)
				{
					GLuint vertexIndex = verticesIndex[::std::make_pair(face.m_verticesIndexes[cpt], face.m_textureIndexes[cpt])] ;
					m_vbo->set(index, vertexIndex) ;
					++index ;
				}
			}
		}
	}

	void MeshVBO_v2::draw()
	{
		// if the mesh has normals associated with vertices, we activate the Phong shader
		if(m_mesh->hasVerticesNormals())
		{
			HelperGl::PhongShader::getSingleton()->enable(HelperGl::LightServer::getSingleton()->getLightMask(), m_mesh->getMaterial()->hasTexture()) ;
		}

		// We bind the material
		HelperGl::Draw::draw(*m_mesh->getMaterial()) ;
		// We draw the VBO
		m_vbo->draw(GL_TRIANGLES) ;

		if(m_mesh->hasVerticesNormals())
		{
			HelperGl::PhongShader::getSingleton()->disable() ;
		}
	}

}