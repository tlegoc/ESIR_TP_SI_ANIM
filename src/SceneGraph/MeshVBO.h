#ifndef _SceneGraph_MeshVBO_H
#define _SceneGraph_MeshVBO_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SceneGraph/NodeInterface.h>
#include <HelperGl/Draw.h>
#include <HelperGl/Mesh.h>
#include <HelperGl/LightServer.h>
#include <HelperGl/PhongShader.h>
#include <set>


namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	MeshVBO
	///
	/// \brief	An optimized version of a mesh drawer that uses Vertex Buffer Objects for rendering.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	01/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class MeshVBO : public NodeInterface
	{
	protected:
		/// \brief	The rendered mesh.
		const HelperGl::Mesh * m_mesh ;
		/// \brief	Buffer for float data.
		GLfloat * m_floatBuffer ;
		/// \brief	Size of the float buffer.
		size_t m_floatBufferSize ;
		/// \brief	Buffer for index data.
		GLuint * m_indexBuffer ;
		/// \brief	Size of the index buffer.
		size_t m_indexBufferSize ;
		/// \brief	Identifier for the float buffer.
		GLuint m_floatBufferId ;
		/// \brief	Identifier for the index buffer.
		GLuint m_indexBufferId ;
		/// \brief Float buffer entry size
		unsigned int m_bufferEntrySize ;

	public:
		MeshVBO(const HelperGl::Mesh * mesh)
			: m_mesh(mesh)
		{
			// We compute a map associating indexes to couples (vertex index, texture coordinate index) 
			::std::map<::std::pair<size_t,size_t>, size_t> verticesIndex ;
			for(auto it = m_mesh->getFaces().begin() ; it!=m_mesh->getFaces().end() ; ++it)
			{
				const HelperGl::Mesh::Face & face = *it ;
				for(int cpt=0 ; cpt<3 ; ++cpt)
				{
					verticesIndex.insert(::std::make_pair(::std::make_pair(face.m_verticesIndexes[cpt], face.m_textureIndexes[cpt]), verticesIndex.size())) ;
				}
			}
			// Computation of the size of an entry in the buffer
			m_bufferEntrySize = 3 ; // 3D vertex
			if(m_mesh->hasVerticesNormals()) { m_bufferEntrySize += 3 ; } // 3D vertex normal
			if(m_mesh->hasTextureCoordinates()) { m_bufferEntrySize += 2 ; } // 2D texture coordinates
			// Allocation of the buffers
			m_floatBufferSize = m_bufferEntrySize*verticesIndex.size() ;
			m_floatBuffer = new GLfloat[m_floatBufferSize] ;
			m_indexBufferSize = m_mesh->getFaces().size()*3 ;
			m_indexBuffer = new GLuint[m_indexBufferSize] ; 
			// We fill the buffer
			for(auto it=verticesIndex.begin() ; it!=verticesIndex.end() ; ++it)
			{
				GLfloat * baseAddress = m_floatBuffer+it->second*m_bufferEntrySize ;
				const Math::Vector3f & vertex = mesh->getVertices()[it->first.first] ;
				::std::copy(vertex.getBuffer(), vertex.getBuffer()+3, baseAddress) ;
				baseAddress += 3 ;
				if(m_mesh->hasVerticesNormals())
				{
					const Math::Vector3f & vertex = mesh->getVerticesNormals()[it->first.first] ;
					::std::copy(vertex.getBuffer(), vertex.getBuffer()+3, baseAddress) ;
					baseAddress += 3 ;
				}
				if(m_mesh->hasTextureCoordinates())
				{
					const Math::Vector2f & texture = mesh->getTextureCoordinates()[it->first.second] ;
					::std::copy(texture.getBuffer(), texture.getBuffer()+2, baseAddress) ;
					baseAddress += 2 ;
				}
			}
			// We fill the index buffer
			GLuint * currentIndex = m_indexBuffer ;
			for(auto it = m_mesh->getFaces().begin() ; it!=m_mesh->getFaces().end() ; ++it)
			{
				const HelperGl::Mesh::Face & face = *it ;
				for(int cpt=0 ; cpt<3 ; ++cpt)
				{
					GLuint index = verticesIndex[::std::make_pair(face.m_verticesIndexes[cpt], face.m_textureIndexes[cpt])] ;
					(*currentIndex) = index ;
					++currentIndex ;
				}
			}
			// We load the buffer in the graphic card
			glGenBuffers(1, &m_floatBufferId) ; 
			glBindBuffer(GL_ARRAY_BUFFER, m_floatBufferId);
			glBufferData(GL_ARRAY_BUFFER, m_floatBufferSize*sizeof(GLfloat), m_floatBuffer, GL_STATIC_DRAW) ;
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// We load the index buffer in the graphics card
			glGenBuffers(1, &m_indexBufferId) ;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId) ;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferSize*sizeof(GLuint), m_indexBuffer, GL_STATIC_DRAW) ;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) ;
		}

		virtual void draw()
		{
			// We enable Phong shader if vertices have vertex normals
			if(m_mesh->hasVerticesNormals())
			{
				HelperGl::PhongShader::getSingleton()->enable(HelperGl::LightServer::getSingleton()->getLightMask(), m_mesh->getMaterial()->hasTexture()) ;
			}
			// We bind the material
			HelperGl::Draw::draw(*m_mesh->getMaterial()) ;
			// We bind the buffer
			GLfloat * base = NULL ;
			glBindBuffer(GL_ARRAY_BUFFER, m_floatBufferId) ;
			glVertexPointer(3, GL_FLOAT, m_bufferEntrySize*sizeof(GLfloat), base) ;
			base += 3 ;
			if(m_mesh->hasVerticesNormals())
			{
				glNormalPointer(GL_FLOAT, m_bufferEntrySize*sizeof(GLfloat), base) ;
				base += 3 ;
			}
			if(m_mesh->hasTextureCoordinates())
			{
				glTexCoordPointer(2, GL_FLOAT, m_bufferEntrySize*sizeof(GLfloat), base) ;
				base += 2 ;
			}
			// We bind the index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId) ;
			// We draw the geometry
			glEnableClientState(GL_VERTEX_ARRAY) ;
			if(m_mesh->hasVerticesNormals()) { glEnableClientState(GL_NORMAL_ARRAY) ; }
			if(m_mesh->hasTextureCoordinates()) { glEnableClientState(GL_TEXTURE_COORD_ARRAY) ; }
			// Note: If there is an access violation when calling glDrawElements, it is possible that some indexes of the index buffer are out of the buffer...
			glDrawElements(GL_TRIANGLES, m_mesh->getFaces().size()*3, GL_UNSIGNED_INT, 0) ;			
			if(m_mesh->hasTextureCoordinates()) { glDisableClientState(GL_TEXTURE_COORD_ARRAY) ; }
			if(m_mesh->hasVerticesNormals()) { glDisableClientState(GL_NORMAL_ARRAY) ; }
			glDisableClientState(GL_VERTEX_ARRAY) ;

			if(m_mesh->hasVerticesNormals())
			{
				HelperGl::PhongShader::getSingleton()->disable();
			}
		}
	};
}

#endif
