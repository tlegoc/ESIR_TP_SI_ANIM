#ifndef _HelperGL_VBO_H
#define _HelperGL_VBO_H

#include <GL/glew.h>
#include <vector>
#include <Math/Vectorf.h>
#include <cassert>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	VBO
	///
	/// \brief	Simple helper class facilitating the use of Vertex Buffer Objects for standard 
	/// 		geometries i.e. vertices with optional normals and optional 2D texture coordinates.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	01/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class VBO
	{
	public:
		/// \brief	Type of data contained in the float buffer
		typedef enum { VERTEX3D=1, 
					   VERTEX_NORMAL=2, 
					   TEXTURE2D_COORDINATES=4
					 } DataType ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::pair<size_t*, size_t> & VBO::getDataTypeSizes() const
		///
		/// \brief	Gets data type sizes. Warning if DataType enum is changed, this method MUST be updated !!!!
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	02/12/2015
		///
		/// \return	The data type sizes.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::pair<const size_t *, size_t> & getDataTypeSizes() const;


	protected:
		GLfloat *	 m_floatBuffer ;
		size_t		 m_floatBufferSize ;
		size_t		 m_floatBufferEntrySize ;
		GLuint		 m_floatBufferId ;
		bool		 m_floatBufferRegistered ;
		bool	     m_floatBufferModified ;
		unsigned int m_dataType ;

		GLuint * m_indexBuffer ;
		size_t   m_indexBufferSize ;
		GLuint   m_indexBufferId ;
		bool	 m_indexBufferRegistered ;
		bool     m_indexBufferModified ;

		::std::vector<GLenum> m_clientStateStack ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	size_t VBO::getEntrySize() const
		///
		/// \brief	Gets the float buffer entry size.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/12/2015
		///
		/// \return	The entry size.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		size_t getEntrySize() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	size_t VBO::getEntryOffset(unsigned int dataType) const
		///
		/// \brief	Gets the offset of an element in the buffer entry.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/12/2015
		///
		/// \param	dataType	Type of the data.
		///
		/// \return	The entry offset.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		size_t getEntryOffset(unsigned int dataType) const;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	VBO::VBO(size_t floatBufferSize, size_t indexBufferSize, unsigned int dataType = VERTEX3D)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/12/2015
		///
		/// \param	floatBufferSize	Size of the float buffer (number of entries).
		/// \param	indexBufferSize	Size of the index buffer (number of indexes).
		/// \param	dataType	   	(optional) type of the data in a float buffer entry.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VBO(size_t floatBufferSize, size_t indexBufferSize, unsigned int dataType = VERTEX3D);

		void set(DataType type, size_t index, const Math::Vector3f & value);

		void set(DataType type, size_t index, const Math::Vector2f & value);

		void set(size_t index, GLuint indexInFloatBuffer);

		void registerFloatBuffer(GLenum usage = GL_STATIC_DRAW);

		void registerIndexBuffer(GLenum usage = GL_STATIC_DRAW);

		void registerBuffers(GLenum floatUsage = GL_STATIC_DRAW, GLenum indexUsage = GL_STATIC_DRAW);

		void updateFloatBuffer();

		void updateIndexBuffer();

		void updateBuffers();

		void draw(GLenum primitive = GL_TRIANGLES);
	} ;
}

#endif

