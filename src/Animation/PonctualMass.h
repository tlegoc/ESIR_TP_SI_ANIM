#ifndef _Animation_PonctualMass_H
#define _Animation_PonctualMass_H

#include <Math/Vectorf.h>

namespace Animation
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	PonctualMass
	///
	/// \brief	Ponctual mass with position, speed and forces
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	24/03/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class PonctualMass
	{
	public:
		/// \brief	The position of the particle.
		Math::Vector3f m_position;
		/// \brief	The speed of the particle.
		Math::Vector3f m_speed;
		/// \brief	The forces applied to the particle.
		Math::Vector3f m_forces;
		/// \brief	The mass of the particle.
		float m_mass;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	PonctualMass::PonctualMass(float mass,
		/// 	const Math::Vector3f & position = Math::Vector3f(0.f),
		/// 	const Math::Vector3f & speed = Math::Vector3f(0.f),
		/// 	const Math::Vector3f & forces = Math::Vector3f(0.f))
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	24/03/2016
		///
		/// \param	mass		The mass.
		/// \param	position	(optional) the position.
		/// \param	speed   	(optional) the speed.
		/// \param	forces  	(optional) the applied forces.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PonctualMass(float mass, const Math::Vector3f & position = Math::Vector3f(0.f), const Math::Vector3f & speed = Math::Vector3f(0.f), const Math::Vector3f & forces = Math::Vector3f(0.f))
			: m_position(position), m_speed(speed), m_forces(forces), m_mass(mass)
		{}
	};
}

#endif
