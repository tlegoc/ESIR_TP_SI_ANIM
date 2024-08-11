#ifndef _Animation_SpringMassSystem_H
#define _Animation_SpringMassSystem_H

#include <Math/Vectorf.h>
#include <Math/Matrix4x4f.h>
#include <Math/Interval.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <cassert>
#include <tbb/parallel_for.h>
#include <tbb/combinable.h>
#include <Utils/History.h>
#include <Animation/PonctualMass.h>

namespace Animation
{
	class SpringMassSystem
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Mass
		///
		/// \brief	A mass.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Mass : public PonctualMass
		{
		public:
			/// \brief	The identifier of the mass.
			int m_id ;
			/// \brief	true if the position of this mass is constrained.
			bool m_isConstrained ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Mass::Mass(const int id, Math::Vector3f const & position,
			/// 	float mass = 1.0f) 
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	id			The identifier of the mass.
			/// \param	position	The position of the mass.
			/// \param	mass		(optional) the mass.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Mass(const int id, Math::Vector3f const & position, float mass = 1.0f)
				: PonctualMass(mass, position), m_id(id), m_isConstrained(false)
			{}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	PatchDescriptor
		///
		/// \brief	Patch descriptor. See the createPatch method.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class PatchDescriptor
		{
		protected:
			friend class SpringMassSystem ;

			/// \brief	Number of masses on the width axis.
			int m_massWidth ;
			/// \brief	Number of masses on the height axis.
			int m_massHeight ;
			/// \brief	Zero-based index of the first mass in the associated SpringMassSystem.
			int m_baseIndex ;
			/// \brief	The instance of SpringMassSystem that created this patch descriptor.
			SpringMassSystem * m_creator ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	PatchDescriptor::PatchDescriptor(int massWidth, int massHeight, int baseIndex)
			///
			/// \brief	Constructor. Only an instance of SpringMassSystem can create an instance of 
			/// 		PatchDescriptor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/02/2016
			///
			/// \param	massWidth 	Number of masses on the width axis.
			/// \param	massHeight	Number of masses on the height axis.
			/// \param	baseIndex 	Zero-based index of the first mass referenced in the SpringMassSystem.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			PatchDescriptor(int massWidth, int massHeight, int baseIndex, SpringMassSystem * creator)
				: m_massWidth(massWidth), m_massHeight(massHeight), m_baseIndex(baseIndex), m_creator(creator)
			{}

		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	PatchDescriptor::PatchDescriptor()
			///
			/// \brief	Default constructor which creates an invalid PatchDescriptor. 
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	18/02/2016
			////////////////////////////////////////////////////////////////////////////////////////////////////
			PatchDescriptor()
				: m_massWidth(0), m_massHeight(0), m_baseIndex(0), m_creator(NULL)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int PatchDescriptor::width() const
			///
			/// \brief	Gets the width.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \return	the width.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int width() const
			{
				assert(m_creator!=NULL) ;
				return m_massWidth ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int PatchDescriptor::height() const
			///
			/// \brief	Gets the height.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \return	The height.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int height() const
			{
				assert(m_creator!=NULL) ;
				return m_massHeight ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int PatchDescriptor::massIndex(int x, int y) const
			///
			/// \brief	Mass index in the spring mass system given its local coordinates.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	x	The x coordinate.
			/// \param	y	The y coordinate.
			///
			/// \return	The identifier of the mass.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int massIndex(int x, int y) const
			{
				assert(m_creator!=NULL) ;
				assert(isValid(x,y)) ;
				return m_baseIndex + y*m_massWidth + x ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	bool PatchDescriptor::isValid(int x, int y) const
			///
			/// \brief	Query if (x,y) coordinates are valid.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	x	The x coordinate.
			/// \param	y	The y coordinate.
			///
			/// \return	true if valid, false if not.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			bool isValid(int x, int y) const
			{
				assert(m_creator!=NULL) ;
				return Math::Interval<int>(0, m_massWidth-1).contains(x) && Math::Interval<int>(0, m_massHeight-1).contains(y) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void PatchDescriptor::constrainPosition(int x, int y)
			///
			/// \brief	Constrain the position of the mass at position (x,y).
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	x	The x coordinate of the mass.
			/// \param	y	The y coordinate of the mass.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void constrainPosition(int x, int y) 
			{
				assert(m_creator!=NULL) ;
				m_creator->constrainPosition(massIndex(x,y)) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void PatchDescriptor::unconstrainPosition(int x, int y)
			///
			/// \brief	Unconstrain position of the mass at position (x,y).
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	x	The x coordinate.
			/// \param	y	The y coordinate.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void unconstrainPosition(int x, int y)
			{
				assert(m_creator!=NULL) ;
				m_creator->unconstrainPosition(massIndex(x,y)) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int PatchDescriptor::getFirstIndex() const
			///
			/// \brief	Gets the first index of the mass belonging to the patch. Note that masses of the patch 
			/// 		are indexed in the interval [getFirstIndex();getLastIndex] in the SpringMassSystem.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \return	The first index.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int getFirstIndex() const
			{
				assert(m_creator!=NULL) ;
				return massIndex(0,0) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	int PatchDescriptor::getLastIndex() const
			///
			/// \brief	Gets the index of the last mass. Note that masses of the patch are indexed in the 
			/// 		interval [getFirstIndex();getLastIndex] in the SpringMassSystem.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \return	The last index.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			int getLastIndex() const
			{
				assert(m_creator!=NULL) ;
				return massIndex(m_massHeight-1, m_massWidth-1) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	SpringMassSystem * PatchDescriptor::getCreator() const
			///
			/// \brief	Gets the SpringMassSystem which created this patch.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \return	null if it fails, else the creator.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			SpringMassSystem * getCreator() const
			{
				assert(m_creator!=NULL) ;
				return m_creator ;
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Link
		///
		/// \brief	Link between masses.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Link
		{
		public:
			/// \brief	The first mass identifier.
			int m_firstMass ;
			/// \brief	The second mass identifier.
			int m_secondMass ;
			/// \brief	The initial length of the link.
			float m_initialLength ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Link::Link(int firstMass, int secondMass,
			/// 	float initialLength) : m_firstMass(::std::min(firstMass, secondMass)),
			/// 	m_secondMass(::std::max(firstMass, secondMass)), m_initialLength(initialLength)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	firstMass	 	The first mass identifier.
			/// \param	secondMass   	The second mass identifier.
			/// \param	initialLength	Initial length of the link.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Link(int firstMass, int secondMass, float initialLength)
				: m_firstMass(::std::min(firstMass, secondMass)), m_secondMass(::std::max(firstMass, secondMass)), m_initialLength(initialLength)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	bool Link::operator< (Link const & link) const
			///
			/// \brief	Less-than comparison operator (lexicographical order)
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	link	The link.
			///
			/// \return	true if the first parameter is less than the second.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			bool operator< (Link const & link) const
			{
				return m_firstMass<link.m_firstMass || (m_firstMass==link.m_firstMass && m_secondMass<link.m_secondMass) ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	bool Link::operator==(Link const & link) const
			///
			/// \brief	Equality operator.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	01/03/2016
			///
			/// \param	link	The link.
			///
			/// \return	true if the parameters are considered equivalent.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			bool operator==(Link const & link) const
			{
				return m_firstMass==link.m_firstMass && m_secondMass==link.m_secondMass ;
			}
		};

	protected:
		/// \brief	The masses.
		Utils::History<::std::vector<Mass> > m_masses ;
		/// \brief	The links.
		::std::vector<Link> m_links ;
		/// \brief	true if links have been modified.
		bool m_linksModified ;
		/// \brief	The internal refresh frequency.
		float m_internalPeriod ;
		/// \brief	The internal clock.
		float m_internalClock ;
		
		/// \brief	The modifiers.
		::std::vector<::std::function<void ()>> m_modifiers ;
		/// \brief	The position constraints functions.
		::std::vector<::std::function<void ()>> m_positionConstraints ;
		/// \brief	The integrator.
		::std::function<void (float dt)> m_integrator ;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	SpringMassSystem::SpringMassSystem()
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		SpringMassSystem()
			: m_linksModified(false), m_internalPeriod(0.001f), m_internalClock(0.0f)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	float SpringMassSystem::getInternalPeriod() const
		///
		/// \brief	Gets internal refresh period.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	19/02/2016
		///
		/// \return	The internal period.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float getInternalPeriod() const
		{
			return m_internalPeriod ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::setInternalPeriod(float value)
		///
		/// \brief	Sets internal refresh period.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	19/02/2016
		///
		/// \param	value	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setInternalPeriod(float value)
		{
			m_internalPeriod = value ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief	Adds a force function.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \tparam	MassForceFunction	Type of the mass force function.
		/// \param	function	The function. Signature : Math::Vector3f (const Mass &amp; mass)
		/// \param	parallel	(optional) True is update must run in parallel, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class MassForceFunction> 
		void addForceFunction(const MassForceFunction & function, bool parallel = false)
		{
			// If a compile error occurs here, your provided function does not have the required signature
			::std::function<Math::Vector3f (const Mass & mass)> verification = function ;
			// Creation of lambda functions
			if(!parallel) // Non parallel version of the modifier
			{
				auto modifier = [this, function]()
				{
					for(auto it=m_masses.current().begin(), end=m_masses.current().end() ; it!=end ; ++it)
					{
						Mass & mass = *it ;
						Math::Vector3f force = function(::std::cref(mass)) ;
						mass.m_forces += force ;
					}
				} ;
				m_modifiers.push_back(modifier) ;
			}
			else // Parallel version of the modifier
			{
				auto modifier = [this, function]()
				{
					auto & refFunction = function ;
					::std::vector<Mass> & masses = m_masses.current() ;
					auto subFunction = [&refFunction, &masses](::tbb::blocked_range<unsigned int> const & range)
					{
						for(unsigned int i=range.begin() ; i<range.end() ; ++i)
						{
							SpringMassSystem::Mass & mass = masses[i] ;
							Math::Vector3f force = refFunction(::std::cref(mass)) ;
							mass.m_forces += force ;
						}
					} ;
					::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, m_masses.current().size(), 2000), subFunction) ;
				} ;
				m_modifiers.push_back(modifier) ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief	Adds a link force function.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \tparam	LinkForceFunction	Type of the link force function.
		/// \param	function	The function. Signature : Math::Vector3f (const Mass &amp; mass1, const
		/// 					Mass &amp; mass2, const Link &amp; link). This function must return the force
		/// 					applied on mass1, the reciprocal force will be automatically applied on mass2.
		/// \param	parallel	(optional) True if update must be done in parallel, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class LinkForceFunction> 
		void addLinkForceFunction(const LinkForceFunction & function, bool parallel=false)
		{
			// If a compile error occurs here, your provided function does not have the required signature
			::std::function<Math::Vector3f (const Mass & mass1, const Mass & mass2, const Link & link)> verification = function ; 
			// Non parallel version
			if(!parallel)
			{
				auto modifier = [this, function]()
				{
					::std::vector<Mass> & currentMasses = m_masses.current() ;
					for(auto it=m_links.begin(), end=m_links.end() ; it!=end ; ++it)
					{
						Mass & mass1 = currentMasses[it->m_firstMass] ;
						Mass & mass2 = currentMasses[it->m_secondMass] ;
						Math::Vector3f force = function(::std::cref(mass1), ::std::cref(mass2), ::std::cref(*it)) ;
						mass1.m_forces += force ;
						mass2.m_forces -= force ;
					}
				} ;
				m_modifiers.push_back(modifier) ;
			}
			else // Parallel version
			{
				auto modifier = [this, function]()
				{
					::tbb::affinity_partitioner ap ;
					auto & refFunction = function ;
					::std::vector<Link> & links = m_links ;
					Utils::History<::std::vector<Mass>> & masses = m_masses ;
					// Thread local storage used to cache and sum computed forces while avoiding read / write conflicts
					::tbb::combinable<::std::vector<Math::Vector3f> > computedForces([&masses]() { return ::std::vector<Math::Vector3f>(masses.current().size(), Math::makeVector(0.0f,0.0f,0.0f)) ; }) ;
					// A lambda function that iterates on the provided range to compute forces
					auto subFunction = [&masses, &refFunction, &links, &computedForces](::tbb::blocked_range<unsigned int> const & range) //unsigned int i)
					{
						::std::vector<Math::Vector3f> & localForces = computedForces.local() ;
						const ::std::vector<SpringMassSystem::Mass> & currentMasses = masses.current() ;
						for(unsigned int i=range.begin() ; i<range.end() ; ++i)
						{
							const SpringMassSystem::Link & link = links[i] ;
							const SpringMassSystem::Mass & mass1 = currentMasses[link.m_firstMass] ;
							const SpringMassSystem::Mass & mass2 = currentMasses[link.m_secondMass] ;
							Math::Vector3f f = refFunction(::std::cref(mass1), ::std::cref(mass2), ::std::cref(link)) ;
							localForces[link.m_firstMass] += f ;
							localForces[link.m_secondMass] -= f ;
						}
					} ;
					// Parallel computation of the forces
					::tbb::parallel_for(::tbb::blocked_range<unsigned int>((unsigned int)0, (unsigned int)m_links.size(),2000), subFunction, ap) ;
					// Combines all results in the force associated with the masses
					computedForces.combine_each([&masses](const ::std::vector<Math::Vector3f> & v)  
					{
						auto & refMasses = masses.current() ;
						auto & refV = v ;
						auto updateForce = [&refMasses, &refV](::tbb::blocked_range<unsigned int> const & range)
						{
							for(unsigned int cpt=range.begin() ; cpt!=range.end() ; ++cpt)
							{
								refMasses[cpt].m_forces += refV[cpt] ;
							}
						} ;
						// Parallel update of the forces
						::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, v.size(), 2000), updateForce) ;
					}
					) ;
				} ;
				m_modifiers.push_back(modifier) ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief	Adds a position constraint.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \tparam	PositionConstraint	Type of the position constraint.
		/// \param	constraint	  	The constraint. Signature : ::std::pair<Math::Vector3f,
		/// 						Math::Vector3> (const Mass &amp; previousMass, const Mass &amp;
		/// 						currentMass). The returned pair must contain (new position, new speed).
		/// \param	parallelUpdate	(optional) true to use parallelism, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class PositionConstraint>
		void addPositionConstraint(const PositionConstraint & constraint, bool parallelUpdate=false)
		{
			// If a compile error occurs here, your provided function does not have the required signature
			::std::function<::std::pair<Math::Vector3f, Math::Vector3f> (const Mass &, const Mass &)> verification = constraint ;
			if(!parallelUpdate) // Mono-threaded update
			{
				auto modifier = [this, constraint]()
				{
					//for(auto it=m_masses.current().begin(), end=m_masses.current().end() ; it!=end ; ++it)
					for(unsigned int cpt=0 ; cpt<m_masses.current().size() ; ++cpt)
					{
						Mass & mass = m_masses.current()[cpt]; 
						Mass & nextMass = m_masses.next()[cpt] ;
						::std::pair<Math::Vector3f, Math::Vector3f> result = constraint(::std::cref(mass), ::std::cref(nextMass)) ;
						nextMass.m_position = result.first ;
						nextMass.m_speed = result.second ;
					}
				} ;
				m_positionConstraints.push_back(modifier) ;
			}
			else // Multi-threaded update
			{
				auto modifier = [this, constraint]()
				{
					static ::tbb::affinity_partitioner ap ;
					Utils::History<::std::vector<Mass>> & masses = m_masses ;
					auto & refConstraint = constraint ;
					auto subFunction = [&masses, &refConstraint](const ::tbb::blocked_range<unsigned int> & range)
					{
						const ::std::vector<SpringMassSystem::Mass> & currentMasses = masses.current() ;
						::std::vector<SpringMassSystem::Mass> & nextMasses = masses.next() ;
						for(unsigned int i=range.begin() ; i<range.end() ; ++i)
						{
							const SpringMassSystem::Mass & mass = currentMasses[i] ;
							SpringMassSystem::Mass & nextMass = nextMasses[i] ;
							::std::pair<Math::Vector3f, Math::Vector3f> result = refConstraint(::std::cref(mass), ::std::cref(nextMass)) ;
							nextMass.m_position = result.first ;
							nextMass.m_speed = result.second ;
						}
					} ;
					::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, m_masses.current().size(), 2000), subFunction, ap) ;
				} ;
				m_positionConstraints.push_back(modifier) ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	int SpringMassSystem::addMass(Math::Vector3f const & position, float mass)
		///
		/// \brief	Adds a mass to the system.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	position	The position of the mass.
		/// \param	mass		Its mass.
		///
		/// \return	The identifier of the added mass.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int addMass(Math::Vector3f const & position, float mass)
		{
			int result = m_masses.current().size() ;
			m_masses.current().push_back(Mass(result, position, mass)) ;
			m_masses.previous().push_back(Mass(result, position, mass)) ;
			m_masses.next().push_back(Mass(result, position, mass)) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::addLink(int mass1, int mass2)
		///
		/// \brief	Adds a link between mass1 and mass2.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	mass1	The identifier of  first mass.
		/// \param	mass2	The identifier of second mass.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void addLink(int mass1, int mass2)
		{
			assert(mass1 < (int)m_masses.current().size()) ;
			assert(mass1 >= 0) ;
			assert(mass2 < (int)m_masses.current().size()) ;
			assert(mass2 >= 0) ;
			float initialLength = (m_masses.current()[mass1].m_position-m_masses.current()[mass2].m_position).norm() ;
			m_links.push_back(Link(mass1, mass2, initialLength)) ;
			m_linksModified = true ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Integrator> void SpringMassSystem::setIntegrator(Integrator integrator,
		/// 	bool parallelUpdate=false)
		///
		/// \brief	Sets the integrator to be used.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	19/02/2016
		///
		/// \tparam	Integrator	Type of the integrator.
		/// \param	integrator	  	The integrator. Signature : ::std::pair<Math::Vector3f, Math::Vector3f> (const Mass &amp; previousMass,
		/// 						const Mass &amp; currentMass, float). The returned pair is (new position, new speed).
		/// \param	parallelUpdate	True to use parallelism, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Integrator>
		void setIntegrator(Integrator integrator, bool parallelUpdate=false)
		{
			// If a compile error occurs here, your provided function does not have the required signature
			::std::function<::std::pair<Math::Vector3f, Math::Vector3f> (const Mass &, const Mass &, float)> verification = integrator ;
			if(!parallelUpdate)
			{
				auto func = [this, integrator](float dt)
				{
					for(unsigned int cpt=0 ; cpt<m_masses.current().size() ; ++cpt)
					{
						Mass & mass = m_masses.current()[cpt] ;
						Mass & previousMass = m_masses.previous()[cpt] ;
						if(!mass.m_isConstrained)
						{
							::std::pair<Math::Vector3f, Math::Vector3f> result = integrator(::std::cref(previousMass), ::std::cref(mass), dt);
							Mass & nextMass = m_masses.next()[cpt] ;
							nextMass.m_position = result.first ;
							nextMass.m_speed = result.second ;
						}
					} 
				} ;
				m_integrator = func ;
			}
			else
			{
				auto func = [this, integrator](float dt)
				{
					static ::tbb::affinity_partitioner ap ;
					// Trick to enable capture in the following lambda function (pb with visual studio 2010).
					Utils::History<::std::vector<Mass>> & masses = m_masses ;
					auto & refIntegrator = integrator ;
					float & refDt = dt ;
					// Lambda function that iterates on a range and computes the integration
					auto subFunction = [&masses, &refIntegrator, &refDt](::tbb::blocked_range<unsigned int> const & range)
					{
						const ::std::vector<SpringMassSystem::Mass> & currentMasses = masses.current() ;
						const ::std::vector<SpringMassSystem::Mass> & previousMasses = masses.previous() ;
						::std::vector<SpringMassSystem::Mass> & nextMasses = masses.next() ;
						for(unsigned int i = range.begin() ; i<range.end() ; ++i)
						{
							const SpringMassSystem::Mass & mass = currentMasses[i] ;
							if(!mass.m_isConstrained)
							{
								const SpringMassSystem::Mass & previousMass = previousMasses[i] ;
								::std::pair<Math::Vector3f, Math::Vector3f> result = refIntegrator(::std::cref(previousMass), ::std::cref(mass), refDt);
								SpringMassSystem::Mass & nextMass = nextMasses[i] ;
								nextMass.m_position = result.first ;
								nextMass.m_speed = result.second ;
							}
						} ;
					} ;
					// We run the previous lambda function in parallel with blocks of 2000 masses
					::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, m_masses.current().size(), 2000), subFunction, ap) ;
				} ;
				m_integrator = func ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool SpringMassSystem::update(float dt)
		///
		/// \brief	Updates the spring mass system.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	dt	The dt.
		///
		/// \return	true if if the system has been updated, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool update(float dt)
		{
			// We update the internal clock
			m_internalClock += dt ;
			bool updated = m_internalClock>m_internalPeriod ;
			// If links have been modified, we sort and ensure the uniqueness of the link between two masses
			updateLinks();
			// We update the system using the internal update frequency
			while(m_internalClock>m_internalPeriod)
			{
				// 1 - We reset forces 
				resetForces();
				// 2 - We call modifiers
				applyModifiers();
				// 3 - We copy the current state to the next state (this ensure a globally consistent state)
				m_masses.next() = m_masses.current() ;
				// 3 - We integrate on the current state and write the results in the next state
				m_integrator(m_internalPeriod) ;
				// 4 - We apply position constraints by using current and next state
				applyPositionConstraints();
				// 5 - We commit the result: next state becomes current state
				m_masses.commit() ;
				// 6 - We update the internal clock
				m_internalClock -= m_internalPeriod ;
			}
			return updated ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::constrainPosition(int massId)
		///
		/// \brief	Constrains the position of a mass.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	massId	Identifier for the mass.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void constrainPosition(int massId)
		{
			assert(massId<(int)m_masses.current().size()); 
			assert(massId>=0) ;
			m_masses.current()[massId].m_isConstrained = true ;
			m_masses.current()[massId].m_speed = Math::makeVector(0.0f, 0.0f, 0.0f) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::unconstrainPosition(int massId)
		///
		/// \brief	Unconstrain the position of a mass.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	massId	Identifier for the mass.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void unconstrainPosition(int massId)
		{
			assert(massId<(int)m_masses.current().size()); 
			assert(massId>=0) ;
			m_masses.current()[massId].m_isConstrained = false ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool SpringMassSystem::isConstrained(int massId)
		///
		/// \brief	Query if a mass is constrained.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	19/02/2016
		///
		/// \param	massId	Identifier for the mass.
		///
		/// \return	true if constrained, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool isConstrained(int massId)
		{
			return m_masses.current()[massId].m_isConstrained ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::applyTransformation(const Math::Matrix4x4f & transformation)
		///
		/// \brief	Applies the provided transformation to all masses in the system.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	transformation	The transformation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void applyTransformation(const Math::Matrix4x4f & transformation)
		{
			for(unsigned int cpt=0 ; cpt<m_masses.current().size() ; ++cpt)
			{
				m_masses.current()[cpt].m_position = transformation * m_masses.current()[cpt].m_position ;
				m_masses.previous()[cpt].m_position = transformation * m_masses.previous()[cpt].m_position ;
			}
			//	for(auto it=m_masses.current().begin(), end=m_masses.current().end() ; it!=end ; ++it)
			//	{
			//		Mass & mass = (*it) ;
			//		mass.m_position = transformation * mass.m_position ;
			//		//mass.m_previousPosition = transformation * mass.m_previousPosition ;
			//	}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::setPosition(int massId, Math::Vector3f const & position)
		///
		/// \brief	Sets the position of a mass.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	massId  	Identifier for the mass.
		/// \param	position	The new position of the mass.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setPosition(int massId, Math::Vector3f const & position)
		{
			m_masses.current()[massId].m_position = position ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<Mass> & SpringMassSystem::getMasses() const
		///
		/// \brief	Gets the masses.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \return	The masses.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<Mass> & getMasses() const
		{
			return m_masses.current() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<Link> & SpringMassSystem::getLinks() const
		///
		/// \brief	Gets the links.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/02/2016
		///
		/// \return	The links.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<Link> & getLinks() const
		{
			return m_links ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	PatchDescriptor SpringMassSystem::createPatch(float width, int widthSubdivisions,
		/// 	float height, int heightSubdivisions, int extent, float globalMass,
		/// 	Math::Matrix4x4f const & transformation = Math::Matrix4x4f::getIdentity())
		///
		/// \brief	Creates a patch.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		///
		/// \param	width			  	The width of the patch (X axis)
		/// \param	widthSubdivisions 	The width subdivisions.
		/// \param	height			  	The height of the patch (Y axis).
		/// \param	heightSubdivisions	The height subdivisions.
		/// \param	extent			  	The extent of neighborhood relations between masses.
		/// \param	globalMass		  	The global mass of the patch.
		/// \param	transformation	  	(optional) the transformation that must be applied to the masses
		/// 							positions during patch generation.
		///
		/// \return	A descriptor of the new patch.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PatchDescriptor createPatch(float width, int widthSubdivisions, float height, int heightSubdivisions, int extent, float globalMass, Math::Matrix4x4f const & transformation = Math::Matrix4x4f::getIdentity())
		{
			assert(widthSubdivisions>=1) ;
			assert(heightSubdivisions>=1) ;
			float mass = globalMass/((widthSubdivisions+1)*(heightSubdivisions+1)) ;
			Math::Vector3f deltaW = Math::makeVector(width/widthSubdivisions, 0.0f, 0.0f) ;
			Math::Vector3f deltaH = Math::makeVector(0.0f, height/heightSubdivisions, 0.0f) ;
			int firstMass = m_masses.current().size() ;
			// Creates the masses
			for(int h=0 ; h<=heightSubdivisions ; h++)
			{
				for(int w=0 ; w<=widthSubdivisions ; ++w)
				{
					Math::Vector3f position = deltaW*(float)w + deltaH*(float)h ;
					addMass(transformation*position, mass) ;
				}
			}
			PatchDescriptor descriptor(widthSubdivisions+1, heightSubdivisions+1, firstMass, this) ;
			// Connects the masses
			for(int h=0 ; h<=heightSubdivisions ; h++)
			{
				for(int w=0 ; w<=widthSubdivisions ; ++w)
				{
					int baseIndex = descriptor.massIndex(w,h) ;
					for(int y=-extent ; y<=extent ; ++y)
					{
						for(int x=-extent ; x<=extent ; ++x)
						{
							if(x==0 && y==0) { continue ; }
							if(descriptor.isValid(w+x, h+y))
							{
								addLink(baseIndex, descriptor.massIndex(w+x, h+y)) ;
							}
						}
					}
				}
			}
			return descriptor ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class OutputIterator> void SpringMassSystem::copyMassesPositionsTo(OutputIterator output)
		///
		/// \brief	Copies the masses positions to the structure targeted by the output iterator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/02/2016
		///
		/// \tparam	OutputIterator	Type of the output iterator.
		/// \param	output	The output iterator.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class OutputIterator>
		void copyMassesPositionsTo(OutputIterator output)
		{
			for(auto it=m_masses.current().begin(), end=m_masses.current().end() ; it!=end ; ++it)
			{
				(*output) = it->m_position ;
				++output ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class OutputIterator> void SpringMassSystem::copyMassesPositions(int beginId,
		/// 	int endId, OutputIterator output)
		///
		/// \brief	Copies the masses positions in interval [beginId; endId[  to the structure targeted by the output iterator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/02/2016
		///
		/// \tparam	OutputIterator	Type of the output iterator.
		/// \param	beginId	Identifier of the first mass.
		/// \param	endId  	Identifier to the mass pas the last mass.
		/// \param	output 	The output iterator.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class OutputIterator>
		void copyMassesPositions(int beginId, int endId, OutputIterator output)
		{
			for(auto it=m_masses.current().begin()+beginId, end=m_masses.current().begin()+endId ; it!=end ; ++it)
			{
				(*output) = it->m_position ;
				++output ;
			}
		}

	protected:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::updateLinks()
		///
		/// \brief	Updates the links if they have been modified. This update implies redundant links 
		/// 		removal as well as link sorting.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void updateLinks() 
		{
			if(m_linksModified)
			{
				m_linksModified = false ;
				::std::sort(m_links.begin(), m_links.end()) ;
				auto newEnd = ::std::unique(m_links.begin(), m_links.end()) ;
				::std::cout<<"Removed "<<m_links.end()-newEnd<<" redundant links"<<::std::endl ;
				m_links.erase(newEnd, m_links.end()) ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::applyPositionConstraints()
		///
		/// \brief	Applies the position constraints.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void applyPositionConstraints() 
		{
			for(auto it=m_positionConstraints.begin(), end=m_positionConstraints.end() ; it!=end ; ++it)
			{
				(*it)() ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::applyModifiers()
		///
		/// \brief	Applies the modifiers.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void applyModifiers() 
		{
			for(auto it=m_modifiers.begin(), end=m_modifiers.end() ; it!=end ; ++it)
			{
				(*it)() ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void SpringMassSystem::resetForces()
		///
		/// \brief	Resets the forces.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void resetForces() 
		{
			::tbb::parallel_for(::tbb::blocked_range<unsigned int>(0, m_masses.current().size(), 2000), 
				[this](::tbb::blocked_range<unsigned int> const & range)
				{
					for(unsigned int i=range.begin() ; i<range.end() ; ++i)
					{
						m_masses.current()[i].m_forces = Math::makeVector(0.0f, 0.0f, 0.0f) ;
					}
				}
			) ;
		}

	};
}

#endif
