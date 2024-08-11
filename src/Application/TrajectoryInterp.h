#pragma once

#include <Application/HermitInterp.h>
#include <vector>

class TrajectoryInterp {

public:
	std::vector<HermitInterp> m_interps;

	glm::vec3 m_currentPoint;
	glm::vec3 m_currentSpeed;
	bool m_canAddInterpolations = false;

	void addPoint(glm::vec3 point, glm::vec3 speed)
	{
		// The first point is not added as an interpolation cause we're missing some data
		if (m_canAddInterpolations) {
			m_interps.push_back(HermitInterp(
				m_currentPoint,
				point,
				m_currentSpeed,
				speed
			));
		}

		m_currentPoint = point;
		m_currentSpeed = speed;
		m_canAddInterpolations = true;
	}

	Math::Vector3f evaluate(float t)
	{
		if (m_interps.empty()) {
			throw std::runtime_error("Trajectory is not built yet");
		}

		float _t = std::clamp(t, 0.0f, m_interps.size() * 1.0f);

		return m_interps[std::floor(_t)].evaluate(fmod(_t, 1.0f));
	}

	Math::Vector3f evaluateLooped(float t)
	{
		return evaluate(fmod(t, m_interps.size() * 1.0f));
	}

	Math::Vector3f speed(float t)
	{
		if (m_interps.empty()) {
			throw std::runtime_error("Trajectory is not built yet");
		}

		float _t = std::clamp(t, 0.0f, m_interps.size() * 1.0f);

		return m_interps[std::floor(_t)].speed(fmod(_t, 1.0f));
	}

	Math::Vector3f speedLooped(float t)
	{
		return speed(fmod(t, m_interps.size() * 1.0f));
	}

	Math::Vector3f getEulerRotation(float t)
	{
		if (m_interps.empty()) {
			throw std::runtime_error("Trajectory is not built yet");
		}

		float _t = std::clamp(t, 0.0f, m_interps.size() * 1.0f);

		return m_interps[std::floor(_t)].getEulerRotation(fmod(_t, 1.0f));
	}

	Math::Vector3f getEulerRotationLooped(float t)
	{
		return getEulerRotation(fmod(t, m_interps.size() * 1.0f));
	}
};