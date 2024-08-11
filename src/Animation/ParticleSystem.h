#ifndef _Animation_ParticleSystem_H
#define _Animation_ParticleSystem_H

#include <Animation/Particle.h>
#include <Math/Sampler.h>
#include <vector>
#include <functional>
#include <iterator>
#include <tbb/parallel_for_each.h>
#include <algorithm>

namespace Animation
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	ParticleSystem
	///
	/// \brief	A particle system with a programmable behavior.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	18/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ParticleSystem
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \typedef	::std::back_insert_iterator<::std::vector<Particle>> ParticleInserter
		///
		/// \brief	Defines an alias representing the particle inserter used by particle emitters.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef ::std::back_insert_iterator<::std::vector<Particle>> ParticleInserter ;

	protected:
		/// \brief	The particles.
		::std::vector<Particle> m_particles ;
		/// \brief The particle budget.
		unsigned int m_budget ;
		
		/// \brief	The modifiers applied to particles.
		::std::vector<::std::function<void (float dt)>> m_modifiers;

		/// \brief Death functions used to control life and death of the particles.
		::std::vector<::std::function<void (::std::vector<size_t> &)>> m_deathQualifier ;

		/// \brief The referenced particles emitters
		::std::vector<::std::function<bool (ParticleInserter inserter, size_t productionLimit, float dt)>> m_emitters ;

	public:
		ParticleSystem(unsigned int budget)
			: m_budget(budget)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<Particle> ParticleSystem::getParticles() const
		///
		/// \brief	Gets the particles (read only).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/12/2015
		///
		/// \return	The particles.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<Particle> & getParticles() const
		{
			return m_particles ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class ParticleModifier> void ParticleSystem::addModifier(const ParticleModifier & modifier)
		///
		/// \brief	Adds a modifier. A modifier MUST be a functor which type is : void (Particle &, float)
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \tparam	ParticleModifier	Type of the particle modifier.
		/// \param	modifier	The modifier. void (const Particle & particle, float dt)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class ParticleModifier>
		void addModifier(ParticleModifier modifier)
		{
			auto func = [this, modifier](float dt) 
				{ 
					for(auto it=m_particles.begin() ; it!=m_particles.end() ; ++it)
					{
						modifier(*it, dt) ;
					}
				} ;
			//auto func = [this, modifier](float dt) 
			//{ 
			//	auto & refModifier = modifier ;
			//	auto subFunction = [&dt, refModifier](Particle & particle) { refModifier(particle, dt); } ;
			//	::tbb::parallel_for_each(m_particles.begin(), m_particles.end(), subFunction) ;
			//} ; 
			m_modifiers.push_back(func) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class DeathFunction> void ParticleSystem::addDeathFunction(DeathFunction deathFunction)
		///
		/// \brief	Adds a death function to the particle system. deathFunction MUST be a functor which type
		/// 		is: bool (const Particle & particle)
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \tparam	DeathFunction 	Type of the death function.
		/// \param	deathFunction	The deathFunction : bool (const Particle & particle)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class DeathFunction>
		void addDeathFunction(DeathFunction deathFunction)
		{
			auto func = [this, deathFunction] (::std::vector<size_t> & deadVector) -> void
				{
					for(auto it=m_particles.begin() ; it!=m_particles.end() ; ++it)
					{
						bool dead = deathFunction(::std::cref(*it)) ;
						if(dead) { deadVector.push_back(it-m_particles.begin()) ; }
					}
				} ;
			m_deathQualifier.push_back(func) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class EmitterFunction> void ParticleSystem::addEmitter(EmitterFunction emitter)
		///
		/// \brief	Adds an emitter to the particle system. 
		/// 		
		/// 		EmitterFunction is a functor which prototype is: 
		/// 		bool (ParticleInserter inserter, size_t productionLimit, float dt)
		/// 		If this function returns false, no more particles will be emitted by this emitter. In this
		/// 		case, the emitter is immediately removed from the system. 
		/// 		Parameter inserter: a back insert iterator used to produce particles.
		/// 		Parameter productionLimit: the maximum number of particles that can be generated, if 
		/// 		too much particles are produced, the surplus will be deleted. 
		/// 		Parameter dt: the elapsed time since last call.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \tparam	EmitterFunction	Type of the emitter function.
		/// \param	emitter	The emitter function : bool (ParticleInserter inserter, size_t productionLimit, float dt)
		////////////////////////////////////////////////////////////////////////////////////////////////////		
		template <class EmitterFunction>
		void addEmitter(EmitterFunction emitter)
		{
			m_emitters.push_back(emitter) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	size_t ParticleSystem::budget() const
		///
		/// \brief	Gets the particle budget.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/02/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		size_t budget() const
		{
			return m_budget ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void ParticleSystem::update(float dt)
		///
		/// \brief	Updates the particle system.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \param	dt	The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void update(float dt)
		{
			// Evolution of the particles
			for(auto it=m_modifiers.begin(); it!=m_modifiers.end() ; ++it)
			{
				(*it)(dt) ;
			}
			// Life and death
			::std::vector<size_t> deathVector ;
			for(auto it=m_deathQualifier.begin() ; it!=m_deathQualifier.end() ; ++it)
			{
				(*it)(deathVector) ; // Note that death vector is sorted in increasing order (consequence of the algorithm)
				for(auto itDeath = deathVector.rbegin() ; itDeath!=deathVector.rend() ; ++itDeath)
				{
					m_particles[*itDeath] = m_particles.back() ; 
					m_particles.pop_back() ;
				}
				deathVector.erase(deathVector.begin(), deathVector.end()) ;
			}
			// Emission
			for(auto it=m_emitters.begin() ; it!=m_emitters.end() ; ++it)
			{
				(*it)(::std::back_inserter(m_particles), m_budget-m_particles.size(), dt) ;
				if(m_particles.size()>m_budget) // We enforce the budget
				{
					m_particles.erase(m_particles.begin()+m_budget, m_particles.end()) ;
				}
			}
		}

		// -----------------------------------------------------------------------
		// Some modifiers (compatibles with the addModifier method)
		// -----------------------------------------------------------------------


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void ParticleSystem::modifierResetForce(Particle & particle, float dt)
		///
		/// \brief	Modifier that resets forces applied on a particle.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param [in,out]	particle	The particle.
		/// \param	dt					The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void modifierResetForce(Particle & particle, float dt)
		{
			particle.m_forces = Math::makeVector(0.0f,0.0f,0.0f) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void ParticleSystem::modifierLifeTime(Particle & particle, float dt)
		///
		/// \brief	Modifier that increases the life time.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param [in,out]	particle	The particle.
		/// \param	dt					The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void modifierLifeTime(Particle & particle, float dt)
		{
			particle.m_lifeTime+=dt ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void ParticleSystem::modifierIntegrator(Particle & particle, float dt)
		///
		/// \brief	Modifier that integrates force and speed to produce particle movements.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param [in,out]	particle	The particle.
		/// \param	dt					The dt.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static inline void modifierIntegrator(Particle & particle, float dt)
		{
			particle.m_speed = particle.m_speed + particle.m_forces*dt/particle.m_mass ;
			particle.m_position = particle.m_position + particle.m_speed*dt ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	ModiferColorLifeTime
		///
		/// \brief	Color modifier class. Given the lifetime of a particle, this modifier interpolates
		/// 		between two colors (color when born, color when dying).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class ModifierColorLifeTime
		{
		protected:
			HelperGl::Color m_startColor ;
			HelperGl::Color m_endColor ;

		public:
			ModifierColorLifeTime(HelperGl::Color const & start, HelperGl::Color const & end)
				: m_startColor(start), m_endColor(end)
			{}

			void operator() (Particle & particle, float dt) const
			{
				float timePercent = ::std::min(particle.m_lifeTime/particle.m_lifeTimeLimit, 1.0f) ;
				particle.m_color = HelperGl::lerp(m_startColor, m_endColor, timePercent) ;
			}
		};

		// -------------------------------------------------------------------------------
		// Some death functions compatibles with the addDeathFunction method
		// -------------------------------------------------------------------------------


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static bool ParticleSystem::deathLifeTime(const Particle & particle)
		///
		/// \brief	Provoques the death of a particle if its life time reaches its end.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	particle	The particle.
		///
		/// \return	true if it succeeds, false if it fails.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static bool deathLifeTime(const Particle & particle)
		{
			return particle.m_lifeTime >= particle.m_lifeTimeLimit ;
		}

		// ---------------------------
		// Some emitters
		// ---------------------------	

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	RateEmitterBase
		///
		/// \brief	Helper class for emitters with emission rate.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class RateEmitterBase
		{
		protected:
			float	m_emissionRate ;
			float	m_dateFraction ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	size_t RateEmitterBase::numberOfParticles(float dt)
			///
			/// \brief	Returns the number of particles that should be emitted at this time.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	dt	The dt.
			///
			/// \return	The total number of particles that should be emitted.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			size_t numberOfParticles(float dt)
			{
				m_dateFraction += dt ;
				size_t number = (size_t)(m_dateFraction * m_emissionRate) ;
				m_dateFraction -= (float)number/(float)m_emissionRate ;
				return number ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	RateEmitterBase::RateEmitterBase(float emissionRate)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	emissionRate	The emission rate (number of particles per second).
			////////////////////////////////////////////////////////////////////////////////////////////////////
			RateEmitterBase(float emissionRate)
				: m_emissionRate(emissionRate)
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	LimitedEmitterBase
		///
		/// \brief	Helper class for emitter with a limited number of particles.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class LimitedEmitterBase
		{
		protected:
			unsigned int m_remainingParticles ;

			unsigned int remainingParticles() const
			{
				return m_remainingParticles ;
			}

			void removeEmitted(unsigned int number) 
			{
				m_remainingParticles -= ::std::min(m_remainingParticles, number) ;
			}

			bool isEmpty() const
			{
				return m_remainingParticles==0 ;
			}

			LimitedEmitterBase(unsigned int particleNumber)
				: m_remainingParticles(particleNumber)
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	BallFlowEmitter
		///
		/// \brief	Ball flow emitter.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class BallFlowEmitter : RateEmitterBase
		{
		protected:
			Math::Vector3f m_center ;
			float m_radius ;
			Math::Interval<float> m_speed ;
			Math::Interval<float> m_lifeTime ;

		public:
			BallFlowEmitter(Math::Vector3f const & center, float radius, float emissionRate,
						    Math::Interval<float> const & speed, Math::Interval<float> const & lifeTime)
				: RateEmitterBase(emissionRate), m_center(center), m_radius(radius), m_speed(speed), m_lifeTime(lifeTime)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	template <class ParticleInitializer> bool BallFlowEmitter::emit(ParticleInserter inserter,
			/// 	size_t productionLimit, float dt)
			///
			/// \brief	Emits a particle located inside a ball.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	18/12/2015
			///
			/// \tparam	ParticleInitializer	Type of the particle initializer. This is a functor of type :
			/// 							Particle (Math::Vector3 const & position)
			/// 							This functor should initialize the position of the particle with
			/// 							the provided one and decide of the values of the remaining 
			/// 							parameters.
			/// \param	inserter	   	The inserter in the particle structure.
			/// \param	productionLimit	The production limit.
			/// \param	dt			   	The dt.
			///
			/// \return	true to always produce particles.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			bool operator() (ParticleInserter inserter, size_t productionLimit, float dt)
			{
				unsigned int toEmit = ::std::min(productionLimit, numberOfParticles(dt)) ;
				for(unsigned int cpt=0 ; cpt<toEmit ; ++cpt)
				{
					Math::Vector3f direction = Math::Sampler::sphere() ;
					Math::Vector3f position = m_center+direction*m_radius ;
					Math::Vector3f speed = direction*m_speed.random() ;
					Particle current ; //= ParticleInitializer(position) ;
					current.m_position = position ;
					current.m_speed = speed ;
					current.m_lifeTimeLimit = m_lifeTime.random();
					current.m_color = HelperGl::Color(1.0f,1.0f,1.0f);
					(*inserter) = current ;
					++inserter; 
				}
				return true ;
			}
		};
	};
}

#endif