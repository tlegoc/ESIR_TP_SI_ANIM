#pragma once

#include <GL/glew.h>
#include <gl3/GlmTypeTraits.h>
#include <gl3/proxy/AssociateTextureWithTextureUnit.h>

namespace gl3
{
	/// <summary>
	/// A class interfacing with a texture buffer object. This object can be used to provide
	/// large one dimensional arrays to a fragment shader. The provided generic type should be
	/// float, glm::vec2, glm::vec3 or glm::vec4.
	/// </summary>
	template <class Type>
	class TextureBufferObject
	{
		static_assert(GlmTypeTraits<Type>::isCompatible() && (GlmTypeTraits<Type>::isVector() || GlmTypeTraits<Type>::isScalar()), "You provided a data type incompatible with the plateform, should be float, glm::vec2; glm::vec3 or glm::vec4");

		GLuint m_tboId;
		GLuint m_textureId;
		size_t m_size;

	public:
		/// <summary>
		/// Creates a texture buffer object that contains provided data
		/// </summary>
		/// <param name="values">The values to transfer to the texture buffer object.</param>
		/// <param name="update">The update frequency.</param>
		TextureBufferObject(const std::vector<Type> & values, BufferUpdate update = BufferUpdate::staticDraw)
		{
			// 1 - We generate the buffer
			glGenBuffers(1, &m_tboId); 
			glBindBuffer(GL_TEXTURE_BUFFER, m_tboId);
			glBufferData(GL_TEXTURE_BUFFER, values.size() * sizeof(Type), values.data(), update);
			// 2 - We generate the texture
			glGenTextures(1, &m_textureId);
			glBindTexture(GL_TEXTURE_BUFFER, m_textureId);
			glTexBuffer(GL_TEXTURE_BUFFER_EXT, GlmTypeTraits<Type>::glTextureFormat(), m_tboId);
			// 3 - We finish everything
			glBindBuffer(GL_TEXTURE_BUFFER, 0);
			glBindTexture(GL_TEXTURE_BUFFER, 0);
			// We store the size
			m_size = values.size();
		}

		/// <summary>
		/// Finalizes an instance of the <see cref="TextureBufferObject"/> class.
		/// </summary>
		~TextureBufferObject()
		{
			if (m_textureId != 0) { glDeleteTextures(1, &m_textureId); }
			if (m_tboId != 0) { glDeleteBuffers(1, &m_tboId); }
		}

		/// <summary>
		/// Forbids the default copy constructor
		/// </summary>
		/// <param name="">The .</param>
		TextureBufferObject(const TextureBufferObject &) = delete;

		/// <summary>
		/// Forbids the default assignment operator
		/// </summary>
		/// <param name="">The .</param>
		/// <returns></returns>
		TextureBufferObject & operator = (const TextureBufferObject &) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="other">The other.</param>
		TextureBufferObject(TextureBufferObject && other)
			: m_tboId(other.m_tboId), m_textureId(other.m_textureId), m_size(other.m_size)
		{
			other.m_tboId = 0; other.m_textureId = 0;
		}

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="other">The other.</param>
		/// <returns></returns>
		TextureBufferObject & operator= (TextureBufferObject && other)
		{
			if (m_textureId != 0) { glDeleteTextures(1, &m_textureId); }
			if (m_tboId != 0) { glDeleteBuffers(1, &m_tboId); }
			m_tboId = other.m_tboId;
			m_textureId = other.m_textureId;
			m_size = other.m_size;
			other.m_tboId = 0;
			other.m_textureId = 0;
		}

		/// <summary>
		/// Determines whether this instance is valid.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		bool isValid() const { return m_textureId != 0; }

		/// <summary>
		/// Updates the content of the texture buffer.
		/// </summary>
		/// <param name="values">The values to transfer to the texture buffer object.</param>
		/// <param name="offset">The offset at which the writing in the targeted buffer will begin.</param>
		void update(const std::vector<Type> & values, size_t offset = 0)
		{
			assert(isValid());
			assert(values.size() + offset <= m_size);
			glBindBuffer(GL_TEXTURE_BUFFER, m_tboId);
			glBufferSubData(GL_TEXTURE_BUFFER, sizeof(Type)*offset, values.size(), values.data());
		}

		/// <summary>
		/// Returns the size of the texture buffer object
		/// </summary>
		/// <returns></returns>
		size_t size() const { return m_size; }

		/// <summary>
		/// Binds this texture buffer object.
		/// </summary>
		void bind()
		{
			assert(isValid());
			glBindTexture(GL_TEXTURE_BUFFER, m_textureId);
		}

		/// <summary>
		/// Unbinds this texture buffer object.
		/// </summary>
		void unbind()
		{
			assert(isValid());
			glBindTexture(GL_TEXTURE_BUFFER, 0);
		}

		/// <summary>
		/// Associates this texture buffer object with the provided texture unit.
		/// </summary>
		/// <param name="unitNumber">The unit number.</param>
		void associateWithTextureUnit(GLuint unitNumber)
		{
			glActiveTexture(GL_TEXTURE0 + unitNumber);
			bind();
		}

		/// <summary>
		/// Returns a proxy for the association with a texture unit.
		/// </summary>
		/// <returns></returns>
		proxy::AssociateTextureWithTextureUnit associateWithTextureUnitProxy()
		{
			return [this](GLuint unitNumber) { associateWithTextureUnit(unitNumber); };
		}
	};
}