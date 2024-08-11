#pragma once

#include <vector>
#include <glm/matrix.hpp>

namespace gl3
{
	/// <summary>
	/// A view frustum with associated transformations
	/// </summary>
	class ViewFrustum
	{
		std::vector<glm::vec3> m_halfUnitCube;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_inverseProjectionView;

		void updateMatrices()
		{
			m_inverseProjectionView = glm::inverse(m_projectionMatrix*m_viewMatrix);
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ViewFrustum"/> class.
		/// </summary>
		ViewFrustum()
		{
			for (int x = -1; x <= 1; x += 2)
			{
				for (int y = -1; y <= 1; y += 2)
				{
					for (int z = 0; z <= 1; z++)
					{
						m_halfUnitCube.push_back(glm::vec4(x, y, z, 1.0f));
					}
				}
			}
		}

		/// <summary>
		/// Sets the projection matrix.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix.</param>
		void setProjectionMatrix(const glm::mat4 & projectionMatrix)
		{
			m_projectionMatrix = projectionMatrix;
			updateMatrices();
		}

		/// <summary>
		/// Sets the view matrix (world to local coordinate system).
		/// </summary>
		/// <param name="viewMatrix">The view matrix.</param>
		void setViewMatrix(const glm::mat4 & viewMatrix)
		{
			m_viewMatrix = viewMatrix;
			updateMatrices();
		}

		/// <summary>
		/// Sets the projection and view matrices.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix.</param>
		/// <param name="viewMatrix">The view matrix.</param>
		void setProjectionAndViewMatrices(const glm::mat4 & projectionMatrix, const glm::mat4 & viewMatrix)
		{
			m_projectionMatrix = projectionMatrix;
			m_viewMatrix = viewMatrix;
			updateMatrices();
		}

		/// <summary>
		/// Gets the vertices of the view frustum in world coordinate system.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix.</param>
		/// <param name="viewMatrix">The view matrix (world to local coordinate system).</param>
		/// <returns></returns>
		std::vector<glm::vec3> getInWorldCoordinateSystem() const
		{
			std::vector<glm::vec3> result(m_halfUnitCube);
			for (auto it = result.begin(), end = result.end(); it != end; ++it)
			{
				glm::vec4 tmp = m_inverseProjectionView * glm::vec4((*it), 1.0);
				(*it) = glm::vec3(tmp) / glm::vec3(tmp.w, tmp.w, tmp.w);
			}
			return std::move(result);
		}

		/// <summary>
		/// Gets the world coordinates of a projected position.
		/// </summary>
		/// <param name="position">The position.</param>
		/// <returns></returns>
		glm::vec3 projectedToWorld(const glm::vec3 & position) const
		{
			return m_inverseProjectionView * glm::vec4(position, 1.0f);
		}

		/// <summary>
		/// Gets the frustum vertices the in projected coordinate system (canonical form used in the graphics card).
		/// </summary>
		/// <returns></returns>
		const std::vector<glm::vec3> getInProjectedCoordinateSystem() const
		{
			return m_halfUnitCube;
		}
	};
}