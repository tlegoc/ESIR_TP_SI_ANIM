#pragma once

#include <glm/glm.hpp>

#include "Math/SphericalCoordinates.h"

class HermitInterp
{
public:
	glm::vec3 m_control1;
	glm::vec3 m_control2;
	glm::vec3 m_tangent1;
	glm::vec3 m_tangent2;

	HermitInterp(glm::vec3 control1, glm::vec3 control2, glm::vec3 tangent1, glm::vec3 tangent2)
	{
		m_control1 = control1;
		m_control2 = control2;
		m_tangent1 = tangent1;
		m_tangent2 = tangent2;
	}

	const glm::mat4x4 M = glm::transpose(glm::mat4x4{
		2, -2, 1, 1,
		-3, 3, -2, -1,
		0, 0, 1, 0,
		1, 0, 0, 0
		});

	/**
	 * \brief Compute the position at progression u for the given interpolation
	 * \param u progression along the curves. Clamped between 0 and 1
	 * \return The vector at position u
	 */
	Math::Vector3f evaluate(float u)
	{
		float new_u = glm::clamp(u, 0.0f, 1.0f);

		glm::vec4 u_vector = { powf(new_u, 3), powf(new_u, 2), new_u, 1.0f };
		glm::mat3x4 control = glm::transpose(glm::mat4x3{
			m_control1,
			m_control2,
			m_tangent1,
			m_tangent2
			});

		glm::mat3x4 tmp = M * control;
		glm::vec3 result = u_vector * tmp;

		return Math::makeVector(result[0], result[1], result[2]);
	}

	Math::Vector3f speed(float u)
	{
		float new_u = glm::clamp(u, 0.0f, 1.0f);

		glm::vec4 u_vector = { 3.0f * powf(new_u, 2), 2.0f * powf(new_u, 1), 1.0f, 0.0f };
		glm::mat3x4 control = glm::transpose(glm::mat4x3{
			m_control1,
			m_control2,
			m_tangent1,
			m_tangent2
			});

		glm::mat3x4 tmp = M * control;
		glm::vec3 result = u_vector * tmp;

		return Math::makeVector(result[0], result[1], result[2]);
	}

	Math::Vector3f speedLooped(float u)
	{
		return speed(fmod(u, 1.0f));
	}

	Math::Vector3f getEulerRotation(float u)
	{
		Math::Vector3f s = speed(u).normalized();

		auto sc = Math::SphericalCoordinates(s);

		return Math::makeVector(0.0f, 3.141592653589793238f/2.0f - sc.getPhy(), sc.getTheta());
	}

	Math::Vector3f getEulerRotationLooped(float u)
	{
		return getEulerRotation(fmod(u, 1.0f));
	}

	Math::Vector3f evaluateLooped(float u)
	{
		return evaluate(fmod(u, 1.0f));
	}
};
