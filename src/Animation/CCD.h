#pragma once

#include <Animation/KinematicChain.h>

class CCD
{
public:
	Animation::KinematicChain* m_chain;
	Animation::KinematicChain::Node* m_end_effector;

	CCD(Animation::KinematicChain* chain, Animation::KinematicChain::Node* end_effector) : m_chain(chain), m_end_effector(end_effector)
	{
		
	}

	bool solve(Math::Vector3f target, Math::Vector3f offset, float maxAngularVariation)
	{
		float dist = std::numeric_limits<float>::max();
		float previous_dist = 0.0f;

		int iter = 0;
		int max_iter = 600;
		while (dist >= 0.001f && abs(dist-previous_dist) >= 0.0001f && iter < max_iter)
		{
			iter++;
			convergeTowards(target, offset, maxAngularVariation);
			
			auto deltaP = target - m_end_effector->getGlobalTransformation() * offset;
			previous_dist = dist;
			dist = deltaP.norm();
		}
		std::cout << iter << std::endl;

		if (dist >= 0.001f) return false;

		return true;
	}

	void convergeTowards(Math::Vector3f target, Math::Vector3f offset, float maxAngularVariation)
	{
		std::vector<Animation::KinematicChain::DegreeOfFreedom> dof;
		m_end_effector->collectDegreesOfFreedom(dof);

		for (int i = dof.size()-1; i >= 0; i--)
		{
			Math::Vector3f deltaP = target - m_end_effector->getGlobalTransformation() * Math::makeVector(0, 0, 0);

			float dthetai = m_chain->derivate(m_end_effector, offset, dof[i], 0.01f).inv() * deltaP;
			dof[i] = dof[i] + Math::makeInterval(-maxAngularVariation, maxAngularVariation).clamp(dthetai);
		}
	}
};