#ifndef _Math_Interval_H
#define _Math_Interval_H

#include <cassert>
#include <stdlib.h>

namespace Math
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Interval
	///
	/// \brief	Class handling intervals. 
	///
	/// \author	Fabrice Lamarche, University Of Rennes 1
	/// \date	20/12/2009
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Float>
	class Interval 
	{
	protected: 
		//! Inferior and superior limits of the interval. 
		Float m_limits[2] ;
		//! Is the interval empty. 
		bool  m_isEmpty ;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Interval(Float const & a, Float const & b)
		///
		/// \brief	Constructor. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	a	Inferior limit of the interval. 
		/// \param [in,out]	b	Superior limit of the interval. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Interval(Float const & a, Float const & b)
		{
			m_limits[0] = a ;
			m_limits[1] = b ;
			m_isEmpty=(a>b) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Interval(Float const & value)
		///
		/// \brief	Initialisation of a point interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	16/12/2009
		///
		/// \param value	the value. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Interval(Float const & value)
		{
			m_limits[0] = value ;
			m_limits[1] = value ;
			m_isEmpty = false ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Interval()
		///
		/// \brief	Default constructor. Creates an empty interval
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Interval()
			: m_isEmpty(true) 
		{}

		/** \name Operations on intervals */
		//@{

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool disjoint(Interval<Float> const & i) const
		///
		/// \brief	Tests if two intervals are disjoint. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	i	the. 
		///
		/// \return	true if intervals are disjoint, false if not. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool disjoint(Interval<Float> const & i) const
		{ return !empty() && !i.empty() && ((sup()<i.inf()) || (i.sup()<inf())) ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool intersect(Interval<Float> const & i) const
		///
		/// \brief	Tests if two intervals intersect. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	i	the. 
		///
		/// \return	true if it succeeds, false if it fails. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool intersect(Interval<Float> const & i) const
		{ return !disjoint(i) ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool contains(Interval<Float> const & inter) const
		///
		/// \brief	Determines if the interval contains the other interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	inter	the interval to test for containment. 
		///
		/// \return	true if the 'inter' is contained by this interval, false if not. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool contains(Interval<Float> const & inter) const
		{ return !empty() && !inter.empty() && in(inter.inf()) && in(inter.sup()) ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Interval<Float> operator+ (Interval<Float> const & i) const
		///
		/// \brief	Computes the bounding interval of the union of two intervals. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	i	the. 
		///
		/// \return	The bounding interval. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Interval<Float> operator+ (Interval<Float> const & i) const
		{
			if(m_isEmpty)   return Interval<Float>(i) ;
			if(i.m_isEmpty) return Interval<Float>(*this) ;
			return Interval<Float>(std::min(inf(),i.inf()), std::max(sup(), i.sup())) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Interval<Float> operator* (Interval<Float> const & i) const
		///
		/// \brief	Computes the intersection of two intervals. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	i	the. 
		///
		/// \return	The result of the operation. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Interval<Float> operator* (Interval<Float> const & i) const
		{
			if(m_isEmpty | i.m_isEmpty) return Interval<Float>() ;
			return Interval<Float>(std::max(inf(),i.inf()), std::min(sup(),i.sup())) ;
		}

		//@}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool empty() const
		///
		/// \brief	Tests if this interval is empty. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \return	true if this interval is empty, false if not. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool empty() const
		{ return m_isEmpty ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void reset()
		///
		/// \brief	Resets this interval to an empty interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	17/12/2009
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void reset()
		{
			m_isEmpty = true ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float const & inf() const
		///
		/// \brief	Returns the inferior limit of the interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \return	. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float const & inf() const 
		{ 
			//assert(!empty()) ;
			return m_limits[0] ; 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float const & sup() const
		///
		/// \brief	Returns the high bound of this interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \return	. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float const & sup() const
		{
			//assert(!empty()) ;
			return m_limits[1] ; 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float delta() const
		///
		/// \brief	Returns the length of the interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \return	. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float delta() const
		{
			//assert(!empty()) ;
			return sup()-inf() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float middle() const
		///
		/// \brief	Returns the middle value of this interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \return	. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float middle() const
		{
			//assert(!empty()) ;
			return (inf()+sup())*Float(0.5) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool in(Float const & v) const
		///
		/// \brief	Tests if a value lies in the interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	v	the v. 
		///
		/// \return	true if it succeeds, false if it fails. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool in(Float const & v) const
		{
			//return !empty() && (v>=inf()) && (v<=sup()) ;
			return !empty() & (v>=inf()) & (v<=sup()) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Interval::isBefore(Float const & v) const
		///
		/// \brief	Query if 'v' is before the interval.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	14/10/2013
		///
		/// \param	v	The tested value.
		///
		/// \return	true if before, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool isBefore(Float const & v) const
		{
			return v<inf() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	bool Interval::isAfter(Float const & v) const
		///
		/// \brief	Query if 'v' is after the interval.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	14/10/2013
		///
		/// \param	v	The Float const &amp; to process.
		///
		/// \return	true if after, false if not.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool isAfter(Float const & v) const
		{
			return v>sup() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float clamp(Float value) const
		///
		/// \brief	Clamps the value in the interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	18/01/2010
		///
		/// \param	value	The value. 
		///
		/// \return	min(max(inf(), value), sup()). 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float clamp(Float value) const
		{
			if(value<inf()) { return inf() ; }
			else if(value>sup()) { return sup() ; }
			return value ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float random() const
		///
		/// \brief	Returns a random value lying in the interval. 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	23/01/2010
		///
		/// \return	a random value lying in the interval. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float random() const
		{
			Float value = Float(rand())/Float(RAND_MAX) ;
			return value * delta() + inf() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Float distance(Math::Interval<Float> const & inter) const
		///
		/// \brief	Computes the distance between two intervals. If intervals intersect, the distance is 0.0
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in,out]	inter	the inter. 
		///
		/// \return	. 
		/// \warning Valid if none of the two intervals is empty.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float distance(Math::Interval<Float> const & inter) const
		{
			assert(!empty() && !inter.empty()) ;
			if(intersect(inter)) { return Float(0.0) ; }
			if((*this)<inter)
			{
				return (inter.inf()-sup()) ;
			}
			return inf()-inter.sup() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief	Computes the distance between a floating point value and this interval interval.
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	20/12/2009
		///
		/// \param [in]	value	the value.
		///
		/// \return	the distance. 
		/// \warning Valid if this interval is not empty.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Float distance(Float const & value) const
		{
			assert(!empty()) ;
			if(value<inf()) { return inf()-value ; }
			if(value>sup()) { return value-sup() ; }
			return 0.0 ;
			//if(in(value)) { return 0.0 ; }
			//return ::std::min(fabs(value-inf()), fabs(value-sup())) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \property	bool operator<(Interval<Float> const & i) const
		///
		/// \brief	 Compares two intervals in lexicographical order (STL compliance). 
		///
		/// \return	The constant. 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool operator<(Interval<Float> const & i) const
		{
			return !empty() && !i.empty() && (inf()<i.inf() || (inf()==i.inf() && sup()<i.sup())) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Float & operator[] (int index) const
		///
		/// \brief	 Gets inf limit (if index=0) or sup limit (if index=1). 
		///
		/// \author	Fabrice Lamarche, University Of Rennes 1
		/// \date	22/04/2010
		///
		/// \param	index	index of the limit. 
		///
		/// \return	inf limit (if index=0) or sup limit (if index=1). 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		const Float & operator[] (int index) const
		{
			assert(index==0 || index==1) ;
			return m_limits[index] ;
		}
	} ;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	Interval<Float> makeInterval(Float cont & min, Float const & max)
	///
	/// \brief	Helper function that creates an Interval. 
	///
	/// \author	Fabrice Lamarche, University Of Rennes 1
	/// \date	23/01/2010
	///
	/// \param [in,out]	min	the minimum value. 
	/// \param [in,out]	max	the maximum value. 
	///
	/// \return	An interval. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <class Float>
	Interval<Float> makeInterval(Float const & min, Float const & max)
	{
		return Interval<Float>(min, max) ;
	}
}

#endif
