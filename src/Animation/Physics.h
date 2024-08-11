#ifndef _Animation_Physics_H
#define _Animation_Physics_H

#include <Animation/PonctualMass.h>

namespace Animation
{
	namespace Physics
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	WeightForce
		///
		/// \brief	Weight force.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	24/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class WeightForce
		{
		protected:
			/// \brief	The gravity.
			float m_gravity ;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	WeightForce::WeightForce(float gravity = 9.807)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	24/03/2016
			///
			/// \param	gravity	(optional) the gravity (default is earth gravity)
			////////////////////////////////////////////////////////////////////////////////////////////////////
			WeightForce(float gravity = 9.807)
				: m_gravity(gravity)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Vector3f WeightForce::operator() (const PonctualMass & mass) const
			///
			/// \brief	 Computation of the force.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	24/03/2016
			///
			/// \return	The force.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Vector3f operator() (const PonctualMass & mass) const
			{
				return mass.m_forces + Math::makeVector(0.0f,0.0f,-mass.m_mass * m_gravity) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	DampingForce
		///
		/// \brief	Damping force.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	24/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class DampingForce
		{
		protected:
			/// \brief	The damping coefficient.
			float m_dampingCoefficient ;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	DampingForce::DampingForce(float dampingCoefficient)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/02/2016
			///
			/// \param	dampingCoefficient	The dampingCoefficient.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			DampingForce(float dampingCoefficient)
				: m_dampingCoefficient(dampingCoefficient)
			{}

			Math::Vector3f operator() (const PonctualMass & mass) const
			{
				return mass.m_speed*(-m_dampingCoefficient) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	AttractionForce
		///
		/// \brief	Attractor class for ponctual masses.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class AttractionForce
		{
		private:
			/// \brief	The center of the attraction force.
			Math::Vector3f m_center ;
			/// \brief	The extent of the attraction force
			float m_extent ;
			/// \brief	The strength of the attraction force.
			float m_attractionForce ;

		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	AttractionForce::AttractionForce(const Math::Vector3f & center, float extent,
			/// 	float attractionForce)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	24/03/2016
			///
			/// \param	center		   	The center of the attraction force.
			/// \param	extent		   	The extent of the attraction force.
			/// \param	attractionForce	The strength of the attraction force.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			AttractionForce(const Math::Vector3f & center, float extent, float attractionForce)
				: m_center(center), m_extent(extent), m_attractionForce(attractionForce)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Vector3f AttractionForce::operator() (PonctualMass & particle) const
			///
			/// \brief	 Computes the force.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	24/03/2016
			///
			/// \return	The force.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Vector3f operator() (PonctualMass & particle) const
			{
				Math::Vector3f delta = m_center-particle.m_position ;
				float deltaNorm = delta.norm() ;
				if(deltaNorm<m_extent)
				{
					float t = deltaNorm/m_extent ;
					Math::Vector3f force = delta.normalized()*t*m_attractionForce ;
					return force ;
				}
				return Math::Vector3f(0.f) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	SpringForce
		///
		/// \brief	Spring force.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class SpringForce
		{
		protected:
			float m_stiffness ;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	SpringForce::SpringForce(float stiffness)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/02/2016
			///
			/// \param	stiffness	The stiffness of the spring.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			SpringForce(float stiffness)
				: m_stiffness(stiffness)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Vector3f SpringForce::operator() (const PonctualMass & mass1,
			/// 	const PonctualMass & mass2, float length) const
			///
			/// \brief	 Computes the force.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	24/03/2016
			///
			/// \return	The force.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Vector3f operator() (const PonctualMass & mass1, const PonctualMass & mass2, float length) const
			{
				Math::Vector3f deltaPosition = mass2.m_position - mass1.m_position ;
				return deltaPosition*(m_stiffness*(1.0f-length/deltaPosition.norm())) ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	inline ::std::pair<Math::Vector3f, Math::Vector3f> integrateEuler(const PonctualMass & previousMass,
		/// 	const PonctualMass & currentMass, float dt)
		///
		/// \brief	Euler integrator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	24/03/2016
		///
		/// \param	previousMass	The previous state of the mass.
		/// \param	currentMass 	The current state of the mass.
		/// \param	dt				The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		inline ::std::pair<Math::Vector3f, Math::Vector3f> integrateEuler(const PonctualMass & previousMass, const PonctualMass & currentMass, float dt) 
		{
			Math::Vector3f newSpeed = currentMass.m_speed + currentMass.m_forces*(dt/currentMass.m_mass) ;
			Math::Vector3f newPosition = currentMass.m_position + newSpeed*dt;
			return ::std::make_pair(newPosition, newSpeed) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	inline ::std::pair<Math::Vector3f, Math::Vector3f> integrateVerlet(const PonctualMass & previousMass,
		/// 	const PonctualMass & currentMass, float dt)
		///
		/// \brief	Verlet integrator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	24/03/2016
		///
		/// \param	previousMass	The previous state of the mass.
		/// \param	currentMass 	The current state of the mass.
		/// \param	dt				The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		inline ::std::pair<Math::Vector3f, Math::Vector3f> integrateVerlet(const PonctualMass & previousMass, const PonctualMass & currentMass, float dt)
		{
			Math::Vector3f newSpeed = currentMass.m_speed + currentMass.m_forces*(dt/currentMass.m_mass) ;
			Math::Vector3f newPosition = currentMass.m_position*2.0f-previousMass.m_position+currentMass.m_forces*dt*dt/currentMass.m_mass ;
			return ::std::make_pair(newPosition, newSpeed) ;
		}
	}
}

#endif