#include <HelperGl/VBO.h>

namespace HelperGl
{


	const ::std::pair<const size_t *, size_t> & VBO::getDataTypeSizes() const
	{
		// This array contains the sizes associated to the types defined in DataType
		// Warning if DataType enum is changed, this table MUST be updated !!!!
		static const size_t sizes[] = { 3, 3, 2 } ;
		static const ::std::pair<const size_t *, size_t> result(sizes, sizeof(sizes)/sizeof(size_t));
		return result ;
	}

	size_t VBO::getEntrySize() const
	{
		const ::std::pair<const size_t*, size_t> & sizes = getDataTypeSizes();
		size_t result = 0 ;
		for(size_t cpt=0 ; cpt<sizes.second ; ++cpt)
		{
			if(((m_dataType>>cpt) & 1)==1)
			{
				result+=sizes.first[cpt] ;
			}
		}
		return result ;
	}

	size_t VBO::getEntryOffset( unsigned int dataType ) const
	{
		const ::std::pair<const size_t*, size_t> & sizes = getDataTypeSizes();
		size_t result = 0 ;
		for(size_t cpt=0 ; cpt<sizes.second ; ++cpt)
		{
			if(((dataType>>cpt) & 1) == 1)
			{
				return result;
			}
			if(((m_dataType>>cpt) & 1) == 1)
			{
				result+=sizes.first[cpt] ;
			}
		}
		// We should never reach this point...
		assert(false) ;
		return result ;
	}

	VBO::VBO( size_t floatBufferSize, size_t indexBufferSize, unsigned int dataType /*= VERTEX3D*/ ) : m_floatBufferSize(floatBufferSize), m_indexBufferSize(indexBufferSize), m_dataType(dataType),
		m_floatBufferRegistered(false), m_floatBufferModified(false),
		m_indexBufferRegistered(false), m_indexBufferModified(false)
	{
		m_floatBufferEntrySize = getEntrySize() ;
		m_floatBuffer = new GLfloat[m_floatBufferSize * m_floatBufferEntrySize] ;
		m_indexBuffer = new GLuint[m_indexBufferSize] ;
	}

	void VBO::set( DataType type, size_t index, const Math::Vector3f & value )
	{
		//assert(getEntrySize()==3) ;
		assert((type & m_dataType)!=0) ;
		assert(index<m_floatBufferSize) ;
		GLfloat * base = m_floatBuffer + getEntrySize() * index + getEntryOffset(type) ;
		::std::copy(value.getBuffer(), value.getBuffer()+3, base) ;
		m_floatBufferModified = true ;
	}

	void VBO::set( size_t index, GLuint indexInFloatBuffer )
	{
		m_indexBuffer[index] = indexInFloatBuffer ;
		m_indexBufferModified = true ;
	}

	void VBO::set( DataType type, size_t index, const Math::Vector2f & value )
	{
		//assert(getEntrySize(type)==2) ;
		assert((type & m_dataType)!=0) ;
		assert(index<m_floatBufferSize) ;
		GLfloat * base = m_floatBuffer + getEntrySize() * index + getEntryOffset(type) ;
		::std::copy(value.getBuffer(), value.getBuffer()+2, base) ;
		m_floatBufferModified = true ;
	}

	void VBO::registerFloatBuffer( GLenum usage /*= GL_STATIC_DRAW*/ )
	{
		if(m_floatBufferRegistered) { return ; }
		// Loads the float buffer
		glGenBuffers(1, &m_floatBufferId) ; 
		glBindBuffer(GL_ARRAY_BUFFER, m_floatBufferId);
		glBufferData(GL_ARRAY_BUFFER, m_floatBufferSize*m_floatBufferEntrySize*sizeof(GLfloat), m_floatBuffer, usage) ;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_floatBufferRegistered = true ;
		m_floatBufferModified = false ;
	}

	void VBO::registerIndexBuffer( GLenum usage /*= GL_STATIC_DRAW*/ )
	{
		if(m_indexBufferRegistered) { return ; }
		// Loads the index buffer
		glGenBuffers(1, &m_indexBufferId) ;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId) ;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferSize*sizeof(GLuint), m_indexBuffer, usage) ;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) ;
		m_indexBufferRegistered = true ;
		m_indexBufferModified = false ;
	}

	void VBO::registerBuffers( GLenum floatUsage /*= GL_STATIC_DRAW*/, GLenum indexUsage /*= GL_STATIC_DRAW*/ )
	{
		registerFloatBuffer(floatUsage);
		registerIndexBuffer(indexUsage);
	}

	void VBO::updateFloatBuffer()
	{
		if(!m_floatBufferModified) { return ; }
		glBindBuffer(GL_ARRAY_BUFFER, m_floatBufferId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_floatBufferSize*m_floatBufferEntrySize*sizeof(GLfloat), m_floatBuffer) ;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_floatBufferModified = false ;
	}

	void VBO::updateIndexBuffer()
	{
		if(!m_indexBufferModified) { return ; }
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId) ;
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indexBufferSize*sizeof(GLuint), m_indexBuffer) ;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_indexBufferModified = false ;
	}

	void VBO::updateBuffers()
	{
		if(!m_floatBufferRegistered) { registerFloatBuffer() ;}
		else { updateFloatBuffer() ; }

		if(!m_indexBufferRegistered) { registerIndexBuffer() ; }
		else { updateIndexBuffer() ; }
	}

	void VBO::draw( GLenum primitive /*= GL_TRIANGLES*/ )
	{
		// Updates buffers if necessary
		updateBuffers() ; 
		// We bind the float buffer
		GLfloat * base = NULL ;
		glBindBuffer(GL_ARRAY_BUFFER, m_floatBufferId) ;
		if(m_dataType & VERTEX3D)
		{
			glVertexPointer(3, GL_FLOAT, m_floatBufferEntrySize*sizeof(GLfloat), base+getEntryOffset(VERTEX3D)) ;
			glEnableClientState(GL_VERTEX_ARRAY) ;
			m_clientStateStack.push_back(GL_VERTEX_ARRAY) ;
		}
		if(m_dataType & VERTEX_NORMAL)
		{
			glNormalPointer(GL_FLOAT, m_floatBufferEntrySize*sizeof(GLfloat), base+getEntryOffset(VERTEX_NORMAL)) ;
			glEnableClientState(GL_NORMAL_ARRAY) ;
			m_clientStateStack.push_back(GL_NORMAL_ARRAY) ;
		}
		if(m_dataType & TEXTURE2D_COORDINATES)
		{
			glTexCoordPointer(2, GL_FLOAT, m_floatBufferEntrySize*sizeof(GLfloat), base+getEntryOffset(TEXTURE2D_COORDINATES)) ;
			glEnableClientState(GL_TEXTURE_COORD_ARRAY) ;
			m_clientStateStack.push_back(GL_TEXTURE_COORD_ARRAY) ;
		}
		// We bind the index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId) ;
		// We draw
		glDrawElements(GL_TRIANGLES, m_indexBufferSize, GL_UNSIGNED_INT, 0) ;
		// We disable client states
		while(!m_clientStateStack.empty())
		{
			GLenum target = m_clientStateStack.back() ;
			m_clientStateStack.pop_back() ;
			glDisableClientState(target) ;
		}
	}

}