#pragma once

#include <GL/glew.h>
#include <gl3/PackedVertexBufferObject.h>

namespace gl3
{
	/// <summary>
	/// A 2D quad covering the whole render window as well as a whole texture. Useful for post processing for instance.
	/// This is a singleton, see the <see cref="ScreenQuad2d::getSingleton"> getSingleton </see> method.
	/// The <see cref="ScreenQuad2D::render"> render </see> methods renders the quad
	/// </summary>
	class ScreenQuad2D
	{
		static const std::vector<std::tuple<glm::vec3, glm::vec2>> & getVertices()
		{
			static std::vector<std::tuple<glm::vec3, glm::vec2>> quadVertices =
			{
				// (positions, texCoords)
				{ glm::vec3(-1.0f,  1.0f, 0.1f), glm::vec2(0.0f, 1.0f) },
				{ glm::vec3(-1.0f, -1.0f, 0.1f), glm::vec2(0.0f, 0.0f) },
				{ glm::vec3(1.0f,  1.0f, 0.1f), glm::vec2(1.0f, 1.0f) },
				{ glm::vec3(1.0f, -1.0f, 0.1f), glm::vec2(1.0f, 0.0f) },
			};
			return quadVertices;
		}

		static const PackedVertexBufferObject<glm::vec3, glm::vec2> & getVbo()
		{
			static PackedVertexBufferObject<glm::vec3, glm::vec2> vbo(getVertices());
			return vbo;
		}

		/// <summary>
		/// Default constructor.
		/// </summary>
		ScreenQuad2D()
		{}

	public:
		/// <summary>
		/// Gets the singleton of ScreenQuad2d.
		/// </summary>
		/// <returns> The unique instance of this class. </returns>
		static const ScreenQuad2D * getSingleton()
		{
			static ScreenQuad2D instance;
			return &instance;
		}

		/// <summary>
		/// Renders the quad.
		/// </summary>
		/// <param name="position2dAttribute">The position2d attribute.</param>
		/// <param name="textureCoordinatesAttribute">The texture coordinates attribute.</param>
		void render(GLint position2dAttribute, GLint textureCoordinatesAttribute) const
		{
			getVbo().bind();
			getVbo().attribPointer<0>(position2dAttribute);
			getVbo().attribPointer<1>(textureCoordinatesAttribute);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, getVbo().size());
			getVbo().unbind();
		}
	};
}