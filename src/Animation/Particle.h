#ifndef _Animation_Particle_H
#define _Animation_Particle_H

#include <limits>
#include <Animation/PonctualMass.h>
#include <Math/Vectorf.h>
#include <HelperGl/Color.h>

namespace Animation
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Particle
	///
	/// \brief	Particle class.
	/// 
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Particle : public PonctualMass
	{
	public:
		/// \brief	The color of the particle.
		HelperGl::Color m_color ;
		/// \brief	The time elapsed since the birth of the particle.
		float m_lifeTime ;
		/// \brief	The life time limit of the particle.
		float m_lifeTimeLimit ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Particle::Particle(Math::Vector3f const & position=Math::makeVector(0.0f,0.0f,0.0f),
		/// 	float const & mass=1.0, HelperGl::Color const & color = HelperGl::Color(),
		/// 	float lifeTimeLimit=::std::numeric_limits<float>::max()) : m_position(position),
		/// 	m_speed(Math::makeVector(0.0f,0.0f,0.0f)), m_forces(Math::makeVector(0.0f,0.0f,0.0f)),
		/// 	m_mass(mass), m_color(color), m_lifeTime(0.0), m_lifeTimeLimit(lifeTimeLimit)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	03/02/2016
		///
		/// \param	position	 	(optional) the initial position.
		/// \param	mass		 	(optional) the mass.
		/// \param	color		 	(optional) the color.
		/// \param	lifeTimeLimit	(optional) the life time limit.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Particle(Math::Vector3f const & position=Math::makeVector(0.0f,0.0f,0.0f), float const & mass=1.0, HelperGl::Color const & color = HelperGl::Color(), float lifeTimeLimit=::std::numeric_limits<float>::max())
			: PonctualMass(mass, position), m_color(color), m_lifeTime(0.0), m_lifeTimeLimit(lifeTimeLimit)
		{}
	};
}

#endif