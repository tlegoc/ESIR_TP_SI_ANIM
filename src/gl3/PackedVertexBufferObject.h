#pragma once

#include <tuple>
#include <gl3/VertexBufferObject.h>
#include <gl3/proxy/AttribPointer.h>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// A class simplifying the use of vertex buffers containing more than one information per vertex 
	/// (for instance position, normal, texture coordinates...).
	/// The provided template types are the type of each informations associated with a vertex.
	/// The information is packed in a tuple in which each per-vertex information has an index. This 
	/// index is the index in the list of provided template parameters.
	/// </summary>
	template <typename FirstType, typename... OtherTypes>
	class PackedVertexBufferObject 
	{
	public:
		/// <summary> The type of the data stored in the buffer. It is a tuple if the number of provided types is more than one, the provided type otherwise </summary> 
		using Data = typename std::conditional<sizeof...(OtherTypes)==0, FirstType, std::tuple<FirstType, OtherTypes...>>::type;

	protected:
		GLuint m_id;
		GLuint m_size;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="InterlacedInterlacedVertexBufferObject"/> class based on the provided buffer.
		/// </summary>
		/// <param name="buffer">The buffer.</param>
		/// <param name="usage">The usage of the buffer.</param>
		PackedVertexBufferObject(const std::vector<Data> & buffer, BufferUpdate usage = BufferUpdate::staticDraw)
		{
			// Création of the vertex buffer object
			glGenBuffers(1, &m_id);
			// Initialisation of the vertex buffer object
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Data)*buffer.size(), buffer.data(), static_cast<GLenum>(usage));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_size = buffer.size();
		}

		/// <summary>
		/// Default constructor. The instance is not valid!!!!
		/// </summary>
		PackedVertexBufferObject()
			: m_id(0), m_size(0)
		{}

		/// <summary>
		/// Finalizes an instance of the <see cref="PackedVertexBufferObject"/> class.
		/// </summary>
		~PackedVertexBufferObject()
		{
			if (m_id != 0) { glDeleteBuffers(1, &m_id); }
		}

		/// <summary>
		/// Forbids the use of the copy constructor.
		/// </summary>
		PackedVertexBufferObject(const PackedVertexBufferObject &) = delete;

		/// <summary>
		/// Forbids the use of the default assignment operator.
		/// </summary>
		PackedVertexBufferObject & operator=(const PackedVertexBufferObject&) = delete;

		/// <summary>
		/// A move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		PackedVertexBufferObject(PackedVertexBufferObject && other)
			: m_id(other.m_id), m_size(other.m_size)
		{
			other.m_id = 0;
		}

		/// <summary>
		/// A move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		PackedVertexBufferObject & operator= (PackedVertexBufferObject && other)
		{
			m_id = other.m_id;
			m_size = other.m_size;
			other.m_id = 0;
			return (*this);
		}

		/// <summary>
		/// Determines whether this buffer is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this buffer is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_id != 0; }

		/// <summary>
		/// Returns the size of the vbo.
		/// </summary>
		/// <returns></returns>
		size_t size() const { return m_size; }

		/// <summary>
		/// Gets the identifier of the buffer.
		/// </summary>
		/// <returns> The identifier of the buffer </returns>
		GLuint getId() const { return m_id; }

		/// <summary>
		/// Binds this buffer.
		/// </summary>
		void bind() const
		{
			assert(isValid() && "PackedVertexBufferObject::bind called on an invalid packed vertex buffer object");
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		/// <summary>
		/// Unbinds this buffer.
		/// </summary>
		void unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		/// <summary>
		/// Associates the element of index struct index in the vertex buffer object with an attribute of a shader.
		/// Warning, the buffer is bound after calling this method.
		/// </summary>
		/// <param name="shaderAttributeIndex">Index of the shader attribute.</param>
		/// <param name="nbInstances">The number of instances before chaning the value.</param>
		template <size_t structIndex = 0>
		void attribPointer(GLint shaderAttributeIndex, GLint nbInstances=0) const
		{
			static_assert(structIndex < 1 + sizeof...(OtherTypes), "PackedVertexBufferObject::attribPointer: Index out of range");

			assert(isValid() && "PackedVertexBufferObject::attribPointer called on an invalid packed vertex buffer object");

			// Binds this buffer
			bind();

			// We compute the offset of the value for a buffer that does not contains tuples
			size_t valueOffset = 0;
			GLenum glType = GLenum(gl3::GlmTypeTraits<FirstType>::glType()); // The scalar data type
			size_t nbSlots = gl3::GlmTypeTraits<FirstType>::columns(); // The number of slots occupied by the data type (1 per column)
			size_t slotSize = gl3::GlmTypeTraits<FirstType>::rows(); // The size of the data in the slot

			// If the buffer contains tuples, we handle it
			if constexpr (sizeof...(OtherTypes) > 0)
			{
				using Type = typename std::remove_reference<std::remove_const<decltype(std::get<structIndex>(Data()))>::type>::type;
				Data tmp;
				auto * value = &std::get<structIndex>(tmp);
				valueOffset = ((char*)value) - ((char*)&tmp);
				using typeTraits = gl3::GlmTypeTraits<Type>;
				nbSlots = typeTraits::columns();
				slotSize = typeTraits::rows();
				glType = GLenum(typeTraits::glType());
			}

			// We associate the buffer with the input of the shader program
			for (size_t cpt = 0; cpt < nbSlots; ++cpt)
			{
				glVertexAttribPointer(shaderAttributeIndex+cpt, slotSize, glType, GL_FALSE, sizeof(Data), (void*)(valueOffset+slotSize*sizeof(float)*cpt));
				glEnableVertexAttribArray(shaderAttributeIndex+cpt);
				if (nbInstances > 0) { glVertexAttribDivisor(shaderAttributeIndex+cpt, nbInstances); }
			}
		}


		/// <summary>
		/// Returns a proxy enabling to bind the packed element identified by structIndex to a vertex attribute.
		/// </summary>
		/// <param name="nbInstances">The number of instances before updating this attribute.</param>
		/// <returns></returns>
		template <size_t structIndex = 0>
		proxy::AttribPointer attribPointerProxy(GLint nbInstances=0) const
		{
			return proxy::AttribPointer([this, nbInstances](GLint index) { attribPointer<structIndex>(index, nbInstances); });
		}

		/// <summary>
		/// Updates the content of the buffer.
		/// </summary>
		/// <param name="buffer">The buffer.</param>
		void update(const std::vector<Data> & buffer) const
		{
			assert(isValid() && "PackedVertexBufferObject::update called on an invalid packed vertex buffer object");
			assert(buffer.size() == m_dataSize);
			bind();
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Data)*buffer.size(), buffer.data());
			unbind();
		}

		/// <summary>
		/// Updates the content of the buffer.
		/// </summary>
		/// <param name="buffer">The buffer.</param>
		/// <param name="startIndex">The start index in the vertex buffer object.</param>
		void update(const std::vector<Data> & buffer, size_t startIndex) const
		{
			assert(isValid() && "PackedVertexBufferObject::update called on an invalid packed vertex buffer object");
			assert(startIndex+buffer.size() <= m_dataSize && && "PackedVertexBufferObject::update called with a too big buffer");
			bind();
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(T)*startIndex, sizeof(Data)*buffer.size(), buffer.data());
			unbind();
		}

	};
}