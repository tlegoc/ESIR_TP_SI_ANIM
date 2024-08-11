#ifndef _GL_compatibility_H
#define _GL_compatibility_H

#include <GL/glew.h>
#include <Math/Matrix4x4f.h>
#include <Math/Vectorf.h>
#include <Math/Constant.h>

namespace GL
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void loadMatrix(Matrix4x4f const & matrix)
	///
	/// \brief	Equivalent of glLoadMatrix.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/03/2016
	///
	/// \param	matrix	The matrix.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void loadMatrix(Math::Matrix4x4f const & matrix)
	{
		float tmp[16] ;
		matrix.toBuffer(tmp) ;
		glLoadMatrixf(tmp) ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void translate(Math::Vector3f const & v)
	///
	/// \brief	Equivalent to glTranslate.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/03/2016
	///
	/// \param	v	The Math::Vector3f const &amp; to process.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void translate(Math::Vector3f const & v)
	{
		glTranslatef(v[0], v[1], v[2]) ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline void rotate(float angle, Math::Vector3f const & axis)
	///
	/// \brief	Rotates.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/03/2016
	///
	/// \param	angle	The angle.
	/// \param	axis 	The axis.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void rotate(float angle, Math::Vector3f const & axis)
	{
		glRotatef(angle*float(180.0/Math::pi), axis[0], axis[1], axis[2]) ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline Matrix4x4f getModelViewMatrix()
	///
	/// \brief	Gets the current model view matrix.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/03/2016
	///
	/// \return	The model view matrix.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Math::Matrix4x4f getModelViewMatrix()
	{
		float tmp[16] ;
		glGetFloatv(GL_MODELVIEW_MATRIX, tmp) ;
		Math::Matrix4x4f result ;
		result.loadFromBuffer(tmp) ;
		return result ;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline Matrix4x4f getProjectionMatrix()
	///
	/// \brief	Gets the current projection matrix.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	03/03/2016
	///
	/// \return	The projection matrix.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Math::Matrix4x4f getProjectionMatrix()
	{
		float tmp[16] ;
		glGetFloatv(GL_PROJECTION_MATRIX, tmp) ;
		Math::Matrix4x4f result ;
		result.loadFromBuffer(tmp) ;
		return result ;
	}
}

#endif
