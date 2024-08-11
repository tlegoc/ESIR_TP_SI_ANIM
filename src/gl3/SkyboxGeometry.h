#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <gl3/VertexBufferObject.h>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// A skybox geometry which is a cube with faces oriented toward the inside. This is a singleton.
	/// </summary>
	class SkyboxGeometry
	{
		static const std::vector<glm::vec3> & getVertices()
		{
			static std::vector<glm::vec3> vertices =
			{
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f, -1.0f),

				glm::vec3(-1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),

				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),

				glm::vec3(-1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),

				glm::vec3(-1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f, -1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(1.0f,  1.0f,  1.0f),
				glm::vec3(-1.0f,  1.0f,  1.0f),
				glm::vec3(-1.0f,  1.0f, -1.0f),

				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f,  1.0f),
				glm::vec3(1.0f, -1.0f,  1.0f)
			};
			return vertices;
		}

		static const VertexBufferObject & getVbo()
		{
			static VertexBufferObject vbo(getVertices());
			return vbo;
		}

		SkyboxGeometry()
		{}

	public:
		/// <summary>
		/// Gets the instance of SkyboxGeometry.
		/// </summary>
		/// <returns></returns>
		static SkyboxGeometry * getSingleton()
		{
			static SkyboxGeometry skybox;
			return &skybox;
		}

		/// <summary>
		/// Renders the skybox using the provided attribute to attach the vertices VBO
		/// </summary>
		/// <param name="positionAttribute">The position attribute of the vertex shader.</param>
		void render(GLint positionAttribute)
		{
			getVbo().bind();
			getVbo().attribPointer(positionAttribute);
			glDrawArrays(GL_TRIANGLES, 0, getVbo().size());
			getVbo().unbind();
		}
	};
}