#ifndef _HelperGl_Buffer_H
#define _HelperGl_Buffer_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Math/Vectorf.h>
#include <cassert>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Buffer
	///
	/// \brief	A buffer that can be sent to the graphics card (base of VBO).
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	06/12/2015
	///
	/// \tparam	Type	Type of the elements stored in the buffer. Warning, this type must be OpenGL
	/// 				compliant (Math::Vector2f, Math::Vector3f, unsigned int etc...).
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Type>
	class Buffer
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	Type * iterator
		///
		/// \brief	Defines an alias representing the iterator.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef Type * iterator ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	const Type * const_iterator
		///
		/// \brief	Defines an alias representing the constant iterator.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef const Type * const_iterator ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	enum
		///
		/// \brief	Defines an alias representing the buffer type.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef enum {ArrayBuffer = GL_ARRAY_BUFFER, ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER} BufferType ;

	protected:
		Type * m_buffer ;
		size_t m_size ;
		GLuint m_bufferId ;
		BufferType m_type ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::construct( const Buffer &other )
		///
		/// \brief	Constructs this object as a copy of the provided one.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/02/2016
		///
		/// \param	other	The other.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void construct( const Buffer &other ) 
		{
			m_size = other.m_size ;
			m_type = other.m_type ;
			m_buffer = new Type[m_size] ;
			::std::copy(other.begin(), other.end(), m_buffer) ;
			if(other.isRegistered())
			{
				registerBuffer() ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::destroy()
		///
		/// \brief	Destroys this object.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void destroy() 
		{
			if(isRegistered())
			{
				unregisterBuffer() ;
			}
			delete[] m_buffer ;
		}

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Buffer::Buffer(size_t size, BufferType type)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		///
		/// \param	size	The size of the buffer in terms of number of elements.
		/// \param	type	The buffer type.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Buffer(size_t size, BufferType type)
			: m_buffer(new Type[size]), m_size(size), m_bufferId(0), m_type(type)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Buffer::Buffer(const Buffer & other)
		///
		/// \brief	Copy constructor. This constructor creates a new buffer which is a copy of the provided
		/// 		one. This new buffer is automatically registered is the provided one is.
		/// 
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		///
		/// \param	other	The other.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Buffer(const Buffer & other)
		{
			construct(other);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Buffer::~Buffer()
		///
		/// \brief	Destructor. Destroy data associated to the buffer and unregister the buffer is needed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		~Buffer()
		{
			destroy();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Buffer & Buffer::operator=(const Buffer & other)
		///
		/// \brief	Assignment operator. Using thios operator is equivalent to destroying the current buffer
		/// 		and creating a new one with the copy constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		///
		/// \param	other	The other.
		///
		/// \return	A shallow copy of this object.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Buffer & operator=(const Buffer & other)
		{
			if(this!=&other)
			{
				destroy() ;
				construct(other) ;
			}
			return *this ;
		}

		iterator begin()
		{
			return m_buffer ;
		}

		iterator end() 
		{
			return m_buffer + m_size ;
		}

		const_iterator begin() const
		{
			return m_buffer ;
		}

		const_iterator end() const
		{
			return m_buffer + m_size ;
		}

		Type & operator[](size_t index)
		{
			assert(index<m_size) ;
			return m_buffer[index] ;
		}

		const Type & operator[](size_t index) const
		{
   		        assert(index<m_size) ;
			return m_buffer[index] ;
		}

		size_t size() const
		{
			return m_size ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::registerBuffer(GLenum usage=GL_STATIC_DRAW)
		///
		/// \brief	Registers the buffer to openGL (also transfers data to the graphic card) with 
		/// 		provided usage.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		///
		/// \param	usage	(optional) the usage; Should be GL_DYNAMIC_DRAW or GL_DYNAMIC_DRAW
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void registerBuffer(GLenum usage=GL_STATIC_DRAW)
		{
			assert(!isRegistered()) ;
			glGenBuffers(1, &m_bufferId) ;
			glBindBuffer(m_type, m_bufferId) ;
			glBufferData(m_type, m_size*sizeof(Type), m_buffer, usage) ;
			unbindBuffer();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::unregisterBuffer()
		///
		/// \brief	Unregisters the buffer from OpenGL.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void unregisterBuffer()
		{
			assert(isRegistered()) ;
			glDeleteBuffers(1, &m_bufferId) ;
			m_bufferId = 0 ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Buffer::isRegistered() const
		///
		/// \brief	Query if this buffer is registered.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		///
		/// \return	true if registered, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool isRegistered() const
		{
			return m_bufferId!=0 ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::updateBuffer() const
		///
		/// \brief	Updates the buffer on the graphics card side. Note that the buffer must have been
		/// 		registered before calling this method.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void updateBuffer() const
		{
			assert(isRegistered());
			glBindBuffer(m_type, m_bufferId) ;
			glBufferSubData(m_type, 0, m_size*sizeof(Type), m_buffer) ;
			unbindBuffer();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::updateBuffer(unsigned int begin, unsigned int end) const
		///
		/// \brief	Updates the buffer in the interval [begin;end[.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	begin	The begin.
		/// \param	end  	The end.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void updateBuffer(unsigned int begin, unsigned int end) const
		{
			assert(isRegistered());
			assert(end<=m_size) ;
			assert(begin<=end) ;
			glBindBuffer(m_type, m_bufferId) ;
			glBufferSubData(m_type, begin*sizeof(Type), (end-begin)*sizeof(Type), m_buffer+begin) ;
			unbindBuffer();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::bindBuffer() const
		///
		/// \brief	Binds this buffer (you still have to call glVertexAttribPoint or glXXXPointer to map
		/// 		thios buffer to a spcific entry of the vertex shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void bindBuffer() const
		{
			assert(isRegistered()) ;
			glBindBuffer(m_type, m_bufferId) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Buffer::unbindBuffer() const
		///
		/// \brief	Unbinds the buffer.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	07/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void unbindBuffer() const
		{
			glBindBuffer(m_type, 0) ;
		}
	};

	// --------------------------------------------------------------------
	// Predefined types of buffers for vertices, texture coordinates etc...
	// --------------------------------------------------------------------
	
	typedef Buffer<Math::Vector2f> BufferVector2f ;
	typedef Buffer<Math::Vector3f> BufferVector3f ;
	typedef Buffer<Math::Vector4f> BufferVector4f ;

	// --------------------------------------------------------------------
	// Predefined types of buffers for index buffers (triangles, lines...)
	// --------------------------------------------------------------------

	typedef Buffer<GLuint>		   BufferIndex ;
	typedef Buffer<GLuint[2]>	   BufferLines ;
	typedef Buffer<GLuint[3]>	   BufferTriangles ;
}

#endif
