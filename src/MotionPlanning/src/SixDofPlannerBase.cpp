#include <MotionPlanning/SixDofPlannerBase.h>

namespace MotionPlanning
{
	SixDofPlannerBase::Configuration SixDofPlannerBase::randomConfiguration()
	{
		float dofs[6];
		for (size_t cpt = 0; cpt<6; ++cpt)
		{
			dofs[cpt] = m_uniformRandom(m_intervals[cpt].first, m_intervals[cpt].second);
		}
		return Configuration{ Math::makeVector(dofs[0], dofs[1], dofs[2]), Math::makeVector(dofs[3], dofs[4], dofs[5]) };
	}

	float SixDofPlannerBase::distanceToObstacles(const Configuration & configuration)
	{
		m_object.setTranslation(configuration.m_translation);
		m_object.setOrientation(configuration.m_orientation);
		return m_collisionManager->computeDistance();
	}

	bool SixDofPlannerBase::doCollide(const Configuration & configuration)
	{
		m_object.setTranslation(configuration.m_translation);
		//m_object.setOrientation(toQuaternion(configuration.m_eulerAngles));
		m_object.setOrientation(configuration.m_orientation);
		//m_object.setOrientation(configuration.m_eulerAngles[0], configuration.m_eulerAngles[1], configuration.m_eulerAngles[2]);
		return m_collisionManager->doCollide();
	}

	float SixDofPlannerBase::configurationDistance(const Configuration & c1, const Configuration & c2) const
	{
		Math::Vector3f point = Math::makeVector(m_maxDistance, 0.f, 0.f);
		Math::Vector3f startPt = c1.m_orientation.rotate(point); // +c1.m_translation;
		Math::Vector3f endPt = c2.m_orientation.rotate(point); // +c2.m_translation;
		return std::max((startPt - endPt).norm(), (c1.m_translation - c2.m_translation).norm());
		//return (startPt - endPt).norm() + (c1.m_translation - c2.m_translation).norm();
	}

	bool SixDofPlannerBase::doCollideRecursive(const Math::Vector3f & startPosition, const Math::Quaternion<float>& startQ, const Math::Vector3f & endPosition, const Math::Quaternion<float>& endQ, float dq)
	{
		//float distance = orientationDistance(startQ, endQ) + (endPosition - startPosition).norm();
		float distance = configurationDistance(Configuration(startPosition, startQ), Configuration(endPosition, endQ));
		Math::Vector3f middlePosition = (startPosition + endPosition) / 2.0f;
		Math::Quaternion<float> middleOrientation = Math::Quaternion<float>::slerp(startQ, endQ, 0.5f);

		if (distance < dq)
		{
			m_object.setTranslation(middlePosition);
			m_object.setOrientation(middleOrientation);
			return m_collisionManager->doCollide();
		}

		return doCollideRecursive(startPosition, startQ, middlePosition, middleOrientation, dq) ||
			doCollideRecursive(middlePosition, middleOrientation, endPosition, endQ, dq);
	}

	bool SixDofPlannerBase::doCollide(const Configuration & start, const Configuration & end, float dq)
	{
		return doCollideRecursive(start.m_translation, start.m_orientation, end.m_translation, end.m_orientation, dq);
	}

	void SixDofPlannerBase::optimize(::std::vector<Configuration>& toOptimize, float dq)
	{
		// Step 1 : we try to remove 
		for (size_t cpt = 0; (cpt + 2) < toOptimize.size(); ++cpt)
		{
			if (!doCollide(toOptimize[cpt], toOptimize[cpt + 2], dq))
			{
				toOptimize.erase(toOptimize.begin() + cpt + 1);
				cpt--;
			}
		}
		// Step 2 : we randomly move the points along the trajectory to try to optimize the path
		for (size_t attempt = 0; attempt < 1000; ++attempt)
		{
			for (size_t cpt = 1; cpt < toOptimize.size() - 1; ++cpt)
			{
				float value = m_uniformRandom(0.0f, 1.0f);
				Configuration intermediate = toOptimize[cpt - 1].interpolate(toOptimize[cpt], value);
				bool collision = doCollide(intermediate, toOptimize[cpt + 1], dq);
				if (!collision)
				{
					toOptimize[cpt] = intermediate;
				}
			}
		}
	}
}