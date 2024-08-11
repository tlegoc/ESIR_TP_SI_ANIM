#ifndef _Animation_InverseKinematics_H
#define _Animation_InverseKinematics_H

#include <Math/finite.h>
#include <Animation/KinematicChain.h>
#include <valarray>
#include <algorithm>
#include <cmath>

namespace Animation
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	InverseKinematics
	///
	/// \brief	Inverse kinematics.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	12/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class InverseKinematics
	{
	protected:
		/// \brief	The kinematic chain.
		KinematicChain * m_chain;
		/// \brief	The extremity of the kinematic chain.
		KinematicChain::Node * m_node;
		/// \brief	The degrees of freedom of the kinematic chain.
		::std::vector<KinematicChain::DegreeOfFreedom> m_degreesOfFreedom;

		/// <summary>
		/// Converges the toward.
		/// </summary>
		/// <param name="begin">The begin iterator.</param>
		/// <param name="end">The end iterator.</param>
		/// <param name="target">The target.</param>
		/// <param name="maxDeltaAngle">The maximum angle variation.</param>
		/// <param name="offset">The offset from the extremity of the kinematic chain.</param>
		/// <returns></returns>
		template <class DofIterator>
		double convergeToward(DofIterator begin, DofIterator end, Math::Vector3f const & target, float maxDeltaAngle=::std::numeric_limits<float>::max(), Math::Vector3f const & offset=Math::makeVector(0.0f,0.0f,0.0f))
		{
			double deltaSum = 0.0 ; 
			for(auto it=begin ; it!=end ; ++it)
			{
				KinematicChain::DegreeOfFreedom & dof = *it ;
				Math::Vector3f extremity = m_node->getGlobalTransformation()*offset ;
				Math::Vector3f deltaTarget = target-extremity ;
				Math::Vector3f derivate = m_chain->derivate(m_node, offset, dof, 0.01f) ;
				float deltaAngle = ::Math::Interval<float>(-maxDeltaAngle, maxDeltaAngle).clamp(derivate.inv()*deltaTarget) ;
				if(Math::is_valid(deltaAngle))
				{
					dof = dof + deltaAngle ;
					deltaSum += fabs(deltaAngle) ;
				}
			}
			return (target-m_node->getGlobalTransformation()*offset).norm() ;
		}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	InverseKinematics::InverseKinematics(KinematicChain * chain,
		/// 	KinematicChain::Node * extremity)
		///
		/// \brief	Initializes a new instance of the <see cref="InverseKinematics"/> class.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	chain	 	The kinematic chain.
		/// \param	extremity	The extremity on which inverse kimatics will be computed.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		InverseKinematics(KinematicChain * chain, KinematicChain::Node * extremity)
			: m_node(extremity)
		{
			extremity->collectDegreesOfFreedom(m_degreesOfFreedom) ;
		}

		::std::vector<float> compute(Math::Vector3f const & target, Math::Vector3f const & offset=Math::Vector3f())
		{
			::std::vector<float> result(m_degreesOfFreedom.size()) ;
			// Vector between the extremity (with the offset) and the target
			Math::Vector3f delta = target-m_node->getGlobalTransformation()*offset ;
			::std::transform(m_degreesOfFreedom.begin(), m_degreesOfFreedom.end(), result.begin(),
			    [&](KinematicChain::DegreeOfFreedom & dof) -> float
				{
					Math::Vector3f result = m_chain->derivate(m_node, offset, dof, 0.01f) ;
					float retValue = result.inv()*delta;
					if(!Math::is_valid(retValue)) { return 0.0 ; }
					return retValue ;
				}
			);
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void InverseKinematics::apply(::std::vector<float> const & angleVariation)
		///
		/// \brief	Applies an angle variation to the degrees of fredom.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	angleVariation	The angle variation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void apply(::std::vector<float> const & angleVariation)
		{
			assert(angleVariation.size()==m_degreesOfFreedom.size()) ;
			for(size_t cpt=0 ; cpt<angleVariation.size() ; ++cpt)
			{
				m_degreesOfFreedom[cpt] = m_degreesOfFreedom[cpt] + angleVariation[cpt] ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Vector3f InverseKinematics::extremityPosition(Math::Vector3f const & offset = Math::Vector3f())
		///
		/// \brief	The position of the extremity of the kinematic chain.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	offset	An offset applied to the extremity (in local coordinates).
		///
		/// \return	The position of the extremity.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Vector3f extremityPosition(Math::Vector3f const & offset = Math::Vector3f())
		{
			return m_node->getGlobalTransformation()*offset ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void InverseKinematics::convergeToward(Math::Vector3f const & target,
		/// 	float maxDeltaAngle=::std::numeric_limits<float>::max(),
		/// 	Math::Vector3f const & offset=Math::Vector3f())
		///
		/// \brief	Converge toward the provided target.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	13/02/2016
		///
		/// \param	target		 	Target for the inverse kinematic.
		/// \param	maxDeltaAngle	(optional) the maximum angle variation that can be applied.
		/// \param	offset		 	(optional) the offset from the extremity of the chain.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		double convergeToward(Math::Vector3f const & target, float maxDeltaAngle=::std::numeric_limits<float>::max(), Math::Vector3f const & offset=Math::makeVector(0.0f,0.0f,0.0f))
		{
			convergeToward(m_degreesOfFreedom.begin(), m_degreesOfFreedom.end(), target, maxDeltaAngle*0.5f, offset) ;
			double dist = convergeToward(m_degreesOfFreedom.rbegin(), m_degreesOfFreedom.rend(), target, maxDeltaAngle*0.5f, offset) ;
			return dist ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void InverseKinematics::convergeTo(Math::Vector3f const & target, float epsilon,
		/// 	float maxDeltaAngle=::std::numeric_limits<float>::max(),
		/// 	Math::Vector3f const & offset=Math::Vector3f())
		///
		/// \brief	Converge to.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	13/02/2016
		///
		/// \param	target		 	Target of the inverse kinematic.
		/// \param	epsilon		 	Acceptable distance between extremity and target.
		/// \param	maxDeltaAngle	(optional) the maximum angle variation during one iteration.
		/// \param	offset		 	(optional) the offset from the kinematic chain extremity.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void convergeTo(Math::Vector3f const & target, float epsilon, float maxDeltaAngle=::std::numeric_limits<float>::max(), Math::Vector3f const & offset=Math::Vector3f())
		{
			epsilon *= epsilon ;
			float lastDelta = ::std::numeric_limits<float>::max() ;
			while(true)
			{
				convergeToward(target, maxDeltaAngle, offset) ;
				float newDelta = (target-extremityPosition(offset)).norm2() ;
				if(newDelta <= epsilon) { break ; }
				if(newDelta<lastDelta) { lastDelta = newDelta ; }
				else { break ; } 
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<KinematicChain::DegreeOfFreedom> & InverseKinematics::getDegreesOfFreedom() const
		///
		/// \brief	Gets the degrees of freedom associated with the targeted kinematic chain.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	13/02/2016
		///
		/// \return	The degrees of freedom.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<KinematicChain::DegreeOfFreedom> & getDegreesOfFreedom() const
		{
			return m_degreesOfFreedom ;
		}
	};
}

#endif