#ifndef _Math_Matrix4x4f_H
#define _Math_Matrix4x4f_H

#include <Math/Matrix4x4.h>
#include <iostream>
namespace Math
{
	typedef Matrix4x4<float> Matrix4f ;
	typedef Matrix4x4<float> Matrix4x4f ;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline ::std::ostream & operator<<(::std::ostream & out, const Matrix4x4f & matrix)
	///
	/// \brief	Prints the matrix.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	22/03/2016
	///
	/// \param [in,out]	out	The out.
	/// \param	matrix	   	The matrix.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ::std::ostream & operator<<(::std::ostream & out, const Matrix4x4f & matrix)
	{
		for(int i=0 ; i<4 ; i++)
		{
			for(int j=0 ; j<4 ; j++)
			{
				out<<matrix(i,j)<<" " ;
			}
			out<<::std::endl ;
		}
		return out ;
	}
}

#endif