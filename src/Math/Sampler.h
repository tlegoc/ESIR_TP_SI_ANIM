#ifndef _Math_Sampler_H
#define _Math_Sampler_H

#include <Math/Vectorf.h>
#include <Math/Quaternion.h>
#include <Math/Interval.h>
#include <Math/Interpolation.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Math
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Sampler
	///
	/// \brief	Sampler.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	16/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Sampler
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static float Sampler::random()
		///
		/// \brief	Returns a random float in interval [0;1].
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	17/12/2015
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static float random()
		{
			return (float)rand()/RAND_MAX ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Math::Vector3 Sampler::sphere()
		///
		/// \brief	Returns a random point in the unit ball. Distribution is uniform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Math::Vector3f ball()
		{
			Math::Interval<float> generator(-1.0f,1.0f) ;
			float a,b,c ;
			do
			{
				a = generator.random() ;
				b = generator.random() ;
				c = generator.random() ;
			}
			while(a*a+b*b+c*c>1) ;
			return Math::makeVector(a,b,c) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Math::Vector3f Sampler::sphere()
		///
		/// \brief	Returns a random point on the surface of the unit sphere. Distribution is uniform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \return	A random point on the surface of the unit sphere.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Math::Vector3f sphere()
		{
			Math::Interval<float> generator(-1.0f,1.0f) ;
			float x1,x2 ;
			do 
			{
				x1 = generator.random() ;
				x2 = generator.random() ;
			} while (x1*x1+x2*x2>=1.0f);
			float tmp = sqrt(1.0f-x1*x1-x2*x2) ;
			return Math::makeVector(2.0f*x1*tmp, 2.0f*x2*tmp, 1.0f-2.0f*(x1*x1+x2*x2)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Math::Vector3f Sampler::cube()
		///
		/// \brief	Gets a random point in a unit cube (0,1). Distribution is uniform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Math::Vector3f cube()
		{
			Math::Interval<float> interval(0,1) ;
			return Math::makeVector(interval.random(), interval.random(), interval.random()) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Math::Vector2f Sampler::disk()
		///
		/// \brief	Returns a random point inside the unit disk. Distribution is uniform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \return	A random point inside the unit disk.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Math::Vector2f disk()
		{
			float angle = Math::Interval<float>(-(float)M_PI, (float)M_PI).random() ;
			float radius = sqrt(Math::Interval<float>(0.0f,1.0f).random()) ;
			return makeVector(radius*cos(angle), radius*sin(angle)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static Math::Vector2f Sampler::circle()
		///
		/// \brief	Returns a random point on the unit circle.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Math::Vector2f circle()
		{
			Math::Interval<float> generator(-1.0f,1.0f) ;
			float x1,x2 ;
			do 
			{
				x1 = generator.random() ;
				x2 = generator.random() ;
			} while (x1*x1+x2*x2>=1.0f);
			float x1_2 = x1*x1 ;
			float x2_2 = x2*x2 ;
			return Math::makeVector((x1_2-x2_2)/(x1_2+x2_2), (2.0f*x1*x2)/(x1_2+x2_2)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Hemisphere
		///
		/// \brief	Random sampling of an hemisphere. This sampling can be uniform or biased by a cosines law
		/// 		(useful for montecarlo rendering for instance)
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Hemisphere
		{
		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	static ::std::pair<float,float> Hemisphere::randomPolar(float n=1.0)
			///
			/// \brief	Static constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/12/2015
			///
			/// \param	n	(optional) the  float to process.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			static ::std::pair<float,float> randomPolar(float n=1.0)
			{
				float theta = acos(pow(random(), 1/(n+1))) ;
				float phy = 2.0f*(float)M_PI*random() ;
				return ::std::make_pair(theta, phy) ;
			}

			///////////////////////////////////////////////////////////////////////////////////
			/// \brief Calcule le vecteur normalisé correspondant aux coordonnées sphériques
			/// 	   fournies en paramètre.
			/// 
			/// \param theta
			/// \param phy
			/// \return 
			/// 
			/// \author F. Lamarche, University of Rennes 1.
			///////////////////////////////////////////////////////////////////////////////////
			static Math::Vector3f getVector(float theta, float phy)
			{
				return Math::makeVector(sin(theta)*cos(phy), sin(theta)*sin(phy), cos(theta)) ;
			}

			///< La direction privilégiée
			Math::Vector3f m_direction ;

			///< Un vecteur normal à la direction privilégiée
			Math::Vector3f m_directionNormal ;

			///< Indice de spécularité de la surface
			float m_n ;

		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Hemisphere::Hemisphere(Math::Vector3f const & direction, float n=1.0)
			///
			/// \brief	Constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/12/2015
			///
			/// \param	direction	The direction.
			/// \param	n		 	(optional) The shininess of the surface. If this value is 0, the hemisphere 
			/// 					is uniformly sampled, if 1 the probablity distribution corresponds to a diffuse
			/// 					surface (cos), if greater than 1, if corresponds to a specular surface (cos^n)
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Hemisphere(Math::Vector3f const & direction, float n=0.0f)
				: m_direction(direction.normalized()), m_n(n)
			{
				// Recherche d'un vecteur normal à la direction privilégiée
				m_directionNormal = Math::makeVector(1.0f,0.0f,0.0f) ;
				m_directionNormal = m_directionNormal - m_direction*(m_direction*m_directionNormal) ;
				if(m_directionNormal.norm()<std::numeric_limits<float>::epsilon()*10.0f)
				{
					m_directionNormal = Math::makeVector(0.0f,1.0f,0.0f) ;
					m_directionNormal =  m_directionNormal - m_direction*(m_direction*m_directionNormal) ;
					if(m_directionNormal.norm()<std::numeric_limits<float>::epsilon()*10.0f)
					{
						m_directionNormal = Math::makeVector(0.0f,0.0f,1.0f) ;
						m_directionNormal =  m_directionNormal - m_direction*(m_direction*m_directionNormal) ;

					}	
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Vector3f Hemisphere::generate() const
			///
			/// \brief	Generates a random direction.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	17/12/2015
			///
			/// \return	The random direction.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Vector3f generate() const
			{
				::std::pair<float,float> perturbation = randomPolar(m_n) ;
				Quaternion<float> q1(m_directionNormal, perturbation.first) ;
				Quaternion<float> q2(m_direction, perturbation.second) ;
				return (q2*q1).rotate(m_direction) ;
				//return q2.rotate(q1.rotate(m_direction)) ;
				//Math::Quaternion<float> result = q2.rotate(q1.rotate(m_direction)) ;
				//return result.v() ;
			}
		};
	};
}

#endif