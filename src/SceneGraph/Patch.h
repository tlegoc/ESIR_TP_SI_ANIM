#ifndef _SceneGraph_Patch_H
#define _SceneGraph_Patch_H

#include <GL/glew.h>
#include <SceneGraph/NodeInterface.h>
#include <HelperGl/Material.h>
#include <HelperGl/Buffer.h>
#include <HelperGl/PhongShader.h>
#include <HelperGl/LightServer.h>
#include <HelperGl/Draw.h>
#include <tbb/parallel_for.h>
#include <vector>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Patch
	///
	/// \brief	A deformable patch.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	09/03/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Patch : public NodeInterface
	{
	protected:
		HelperGl::Buffer<Math::Vector3f> m_vertices ;
		HelperGl::Buffer<Math::Vector3f> m_normals ;
		HelperGl::Buffer<Math::Vector3f> m_reverseNormals ;
		HelperGl::Buffer<Math::Vector2f> m_textureCoordinates ;
		HelperGl::Buffer<unsigned int> m_indexes ;
		HelperGl::Buffer<unsigned int> m_reverseIndexes ;
		HelperGl::Material m_material ;
		unsigned int m_sizeX ;
		unsigned int m_sizeY ;
		bool m_updateNeeded ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Patch::setupTextureCoordinates()
		///
		/// \brief	Sets up the texture coordinates.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setupTextureCoordinates() 
		{
			float stepX = 1.0f/m_sizeX ;
			float stepY = 1.0f/m_sizeY ;
			HelperGl::Buffer<Math::Vector2f>::iterator output = m_textureCoordinates.begin() ;
			for(unsigned int y=0 ; y<m_sizeY ; y++)
			{
				for(unsigned int x=0 ; x<m_sizeX ; x++)
				{
					Math::Vector2f coordinate = Math::makeVector(stepX*x, stepY*y) ;
					(*output) = coordinate ;
					++output ;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Patch::setupFacesCCW()
		///
		/// \brief	Sets up the faces counter clock wise.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setupFacesCCW() 
		{
			HelperGl::Buffer<unsigned int>::iterator it=m_indexes.begin() ;
			for(unsigned int y=0 ; y<m_sizeY-1 ; y++)
			{
				for(unsigned int x=0 ; x<m_sizeX-1 ; x++)
				{
					// First triangle
					(*it++) = getIndex(x,y) ;
					(*it++) = getIndex(x+1,y) ;
					(*it++) = getIndex(x+1,y+1) ;
					// Second triangle
					(*it++) = getIndex(x,y) ;
					(*it++) = getIndex(x+1,y+1) ;
					(*it++) = getIndex(x,y+1) ;
				}
			}
		}

		void setupFacesCW() 
		{
			HelperGl::Buffer<unsigned int>::iterator it=m_reverseIndexes.begin() ;
			for(unsigned int y=0 ; y<m_sizeY-1 ; y++)
			{
				for(unsigned int x=0 ; x<m_sizeX-1 ; x++)
				{
					// First triangle
					(*it++) = getIndex(x,y) ;
					(*it++) = getIndex(x+1,y+1) ;
					(*it++) = getIndex(x+1,y) ;
					// Second triangle
					(*it++) = getIndex(x,y) ;
					(*it++) = getIndex(x,y+1) ;
					(*it++) = getIndex(x+1,y+1) ;
				}
			}
		}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Patch::Patch(unsigned int sizeX, unsigned int sizeY,
		/// 	const HelperGl::Material & material) 
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		///
		/// \param	sizeX   	Number of vertices on the X axis.
		/// \param	sizeY   	Number of vertices on the y axis.
		/// \param	material	The material.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Patch(unsigned int sizeX, unsigned int sizeY, const HelperGl::Material & material)
			: m_vertices(sizeX*sizeY, HelperGl::Buffer<Math::Vector3f>::ArrayBuffer),
			  m_normals(sizeX*sizeY, HelperGl::Buffer<Math::Vector3f>::ArrayBuffer),
			  m_reverseNormals(sizeX*sizeY, HelperGl::Buffer<Math::Vector3f>::ArrayBuffer),
			  m_textureCoordinates(sizeX*sizeY, HelperGl::Buffer<Math::Vector2f>::ArrayBuffer),
			  m_indexes((sizeX-1)*(sizeY-1)*3*2, HelperGl::Buffer<unsigned int>::ElementArrayBuffer),
			  m_reverseIndexes((sizeX-1)*(sizeY-1)*3*2, HelperGl::Buffer<unsigned int>::ElementArrayBuffer),
			  m_material(material),
			  m_sizeX(sizeX), m_sizeY(sizeY), m_updateNeeded(true)
		{
			setupTextureCoordinates();
			setupFacesCCW();
			setupFacesCW() ;
			m_vertices.registerBuffer(GL_DYNAMIC_DRAW) ;
			m_normals.registerBuffer(GL_DYNAMIC_DRAW) ;
			m_reverseNormals.registerBuffer(GL_DYNAMIC_DRAW) ;
			m_textureCoordinates.registerBuffer() ;
			m_indexes.registerBuffer() ;
			m_reverseIndexes.registerBuffer() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	unsigned int Patch::getIndex(unsigned int x, unsigned int y)
		///
		/// \brief	Gets the index of the vertex located at (x,y).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		///
		/// \param	x	X coodinate.
		/// \param	y	Y coordinate.
		///
		/// \return	The index.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int getIndex(unsigned int x, unsigned int y)
		{
			return y*m_sizeX + x ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	HelperGl::Buffer<Math::Vector3f> & Patch::getVertices()
		///
		/// \brief	Gets the vertices.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		///
		/// \return	The vertices.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		HelperGl::Buffer<Math::Vector3f> & getVertices()
		{
			m_updateNeeded = true ;
			return m_vertices ; 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Patch::updateNormals()
		///
		/// \brief	Updates the normals. Normally, this method is called automatically but in some cases,
		/// 		the user could want to ask for an update.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void updateNormals()
		{
			::std::vector<unsigned int> normalCount(m_sizeX*m_sizeY,0) ;
			::std::vector<Math::Vector3f> normalAccumulator(m_sizeX*m_sizeY,Math::makeVector(0.0f,0.0f,0.0f)) ;
			// 1 - We compute the sum of the normals per vertex 
			HelperGl::Buffer<Math::Vector3f>::const_iterator baseVertex = m_vertices.begin() ;
			HelperGl::Buffer<unsigned int>::const_iterator baseIndex = m_indexes.begin() ;
			for(unsigned int cpt=0 ; cpt<m_indexes.size() ; cpt+=3)
			{
				unsigned int index1 = baseIndex[cpt] ; // *(baseIndex+cpt) ;
				unsigned int index2 = baseIndex[cpt+1] ; //*(baseIndex+cpt+1) ;
				unsigned int index3 = baseIndex[cpt+2] ; //*(baseIndex+cpt+2) ;
				Math::Vector3f normal = ((baseVertex[index2]-baseVertex[index1])^(baseVertex[index3]-baseVertex[index1])).normalized() ; //(((*(baseVertex+index2))-(*(baseVertex+index1)))^((*(baseVertex+index3))-(*(baseVertex+index1)))).normalized() ;
				normalAccumulator[index1] += normal ;
				normalAccumulator[index2] += normal ;
				normalAccumulator[index3] += normal ;
				normalCount[index1]++ ;
				normalCount[index2]++ ;
				normalCount[index3]++ ;
			}
			// 2 - We normalize and write the result in the normal buffer (mono-threaded)
			HelperGl::Buffer<Math::Vector3f>::iterator baseNormals = m_normals.begin(); 
			HelperGl::Buffer<Math::Vector3f>::iterator baseReverseNormals = m_reverseNormals.begin() ;
			//for(unsigned int cpt=0 ; cpt<normalCount.size() ; ++cpt)
			//{
			//	Math::Vector3f normal =(normalAccumulator[cpt]/(float)normalCount[cpt]).normalized() ;
			//	baseNormals[cpt] = normal ;
			//	baseReverseNormals[cpt] = -normal ;			
			//}
			// 2 - We normalize and write the result in the normal buffer (multi-threaded)
			auto subFunction = [&](::tbb::blocked_range<unsigned int> const & range)
			{
				for(unsigned int cpt=range.begin() ; cpt!=range.end() ; ++cpt)
				{
					Math::Vector3f normal =(normalAccumulator[cpt]/(float)normalCount[cpt]).normalized() ;
					baseNormals[cpt] = normal ;
					baseReverseNormals[cpt] = -normal ;
				}
			} ;
			::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, normalCount.size(), 2000), subFunction) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Patch::draw()
		///
		/// \brief	Draws this object.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	09/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void draw()
		{
			if(m_updateNeeded) // We update the normals and send the buffer if a modification occurred.
			{
				updateNormals() ;
				m_vertices.updateBuffer() ;
				m_normals.updateBuffer() ;
			}
			HelperGl::PhongShader::getSingleton()->enable(HelperGl::LightServer::getSingleton()->getLightMask(), m_material.hasTexture()) ;
			HelperGl::Draw::draw(m_material) ;
			m_vertices.bindBuffer() ;
			glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), 0) ;
			glEnableClientState(GL_VERTEX_ARRAY) ;
			m_normals.bindBuffer() ;
			glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), 0) ;
			glEnableClientState(GL_NORMAL_ARRAY) ;
			if(m_material.hasTexture())
			{
				m_textureCoordinates.bindBuffer() ;
				glTexCoordPointer(2,  GL_FLOAT, 2*sizeof(GLfloat), 0) ;
				glEnableClientState(GL_TEXTURE_COORD_ARRAY) ;
			}
			m_indexes.bindBuffer() ;
			glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0) ;

			m_reverseNormals.bindBuffer() ;
			glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), 0) ;
			m_reverseIndexes.bindBuffer() ;
			glDrawElements(GL_TRIANGLES, m_reverseIndexes.size(), GL_UNSIGNED_INT, 0) ;

			glDisableClientState(GL_VERTEX_ARRAY) ;
			glDisableClientState(GL_NORMAL_ARRAY) ;
			if(m_material.hasTexture())
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY) ;
			}
			HelperGl::PhongShader::getSingleton()->disable() ;
		}
	};
}

#endif