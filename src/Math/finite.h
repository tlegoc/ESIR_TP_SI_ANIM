#ifndef _Math_Finite_H
#define _Math_Finite_H

#include <limits>
#include <math.h>

namespace Math
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline bool is_finite(float value)
	///
	/// \brief	Query if 'value' is finite.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	02/03/2016
	///
	/// \param	value	The value.
	///
	/// \return	true if finite, false if not.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool is_finite(float value)
	{
		return fabsf(value)!=::std::numeric_limits<float>::infinity() ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline bool is_indeterminate(float value)
	///
	/// \brief	Query if 'value' is indeterminate.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	02/03/2016
	///
	/// \param	value	The value.
	///
	/// \return	true if indeterminate, false if not.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool is_indeterminate(float value)
	{
		return !(value==value) ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline bool is_valid(float value)
	///
	/// \brief	Query if 'value' is valid.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	02/03/2016
	///
	/// \param	value	The value.
	///
	/// \return	true if valid, false if not.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool is_valid(float value)
	{
		return is_finite(value) && !is_indeterminate(value) ;
	}
}

#endif
