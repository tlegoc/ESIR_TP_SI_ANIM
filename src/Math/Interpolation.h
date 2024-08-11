#ifndef _Math_Interpolation_H
#define _Math_Interpolation_H

namespace Math
{
	class Interpolation
	{
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Flt, unsigned int dimensions> static Math::Vector<Flt, dimensions> Interpolation::lerp(Math::Vector<Flt, dimensions> const & p1,
		/// 	Math::Vector<Flt, dimensions> const & p2, Flt t)
		///
		/// \brief	Lerp: linear interpolation between vectors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \tparam	Flt					   	Type of the float.
		/// \tparam	unsigned int dimensions	Number of dimensions.
		/// \param	p1	The first vector.
		/// \param	p2	The second vector.
		/// \param	t 	The parameter in [0;1].
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Flt, unsigned int dimensions>
		static Math::Vector<Flt, dimensions> lerp(Math::Vector<Flt, dimensions> const & p1, Math::Vector<Flt, dimensions> const & p2, Flt t)
		{
			return p1*(1-t)+p2*t ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Flt, unsigned int dimensions> static Flt Interpolation::angleBetween(Math::Vector<Flt, dimensions> const & p1,
		/// 	Math::Vector<Flt, dimensions> const & p2)
		///
		/// \brief	Returns the angle between two vectors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \tparam	Flt					   	Type of the float.
		/// \tparam	unsigned int dimensions	Number of dimensions.
		/// \param	p1	The first vector.
		/// \param	p2	The second vector.
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Flt, unsigned int dimensions>
		static Flt angleBetween(Math::Vector<Flt, dimensions> const & p1, Math::Vector<Flt, dimensions> const & p2)
		{
			return acos(p1.normalized()*p2.normalized()) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class Flt, unsigned int dimensions> static Math::Vector<Flt, dimensions> Interpolation::slerp(Math::Vector<Flt, dimensions> const & p1,
		/// 	Math::Vector<Flt, dimensions> const & p2, Flt t)
		///
		/// \brief	Slerp: spherical interpolation between vectors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	16/12/2015
		///
		/// \tparam	Flt					   	Type of the float
		/// \tparam	unsigned int dimensions	Number of dimensions.
		/// \param	p1	The first vector.
		/// \param	p2	The second vector.
		/// \param	t 	The paramter in [0;1].
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class Flt, unsigned int dimensions>
		static Math::Vector<Flt, dimensions> slerp(Math::Vector<Flt, dimensions> const & p1, Math::Vector<Flt, dimensions> const & p2, Flt t)
		{
			Flt angle = acos(p1.normalized()*p2.normalized()) ;
			return p1*sin((1-t)*angle)/sin(angle) + p2*sin(t*angle)/sin(angle) ;
		}
	};
}

#endif