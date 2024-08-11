#ifndef _SceneGraph_Rotate_H
#define _SceneGraph_Rotate_H

#include <SceneGraph/Transform.h>
#include <Math/Vectorf.h>
#include <Math/Constant.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Rotate
	///
	/// \brief	A rotation around a given axis with a given angle.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Rotate : public Transform
	{
	protected:
		GLfloat m_angle ;
		Math::Vector3f m_axis ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Rotate::Rotate(GLfloat angle, const Math::Vector3f & axis)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	angle	The angle.
		/// \param	axis 	The axis.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rotate(GLfloat angle, const Math::Vector3f & axis)
			: m_angle(angle), m_axis(axis)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Rotate::setAngle(GLfloat angle)
		///
		/// \brief	Sets the rotation angle.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	angle	The angle.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setAngle(GLfloat angle)
		{
			m_angle = angle ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLfloat Rotate::getAngle() const
		///
		/// \brief	Gets the rotation angle.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \return	The angle.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLfloat getAngle() const
		{
			return m_angle ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Rotate::setAxis(const Math::Vector3f & axis)
		///
		/// \brief	Sets the rotation axis.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	axis	The axis.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setAxis(const Math::Vector3f & axis)
		{
			m_axis = axis ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Math::Vector3f & Rotate::getAxis() const
		///
		/// \brief	Gets the rotation axis.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \return	The axis.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Math::Vector3f & getAxis() const
		{
			return m_axis ;
		}

		virtual void draw() 
		{
			storeMatrix() ;
			glRotatef((float)(m_angle*180.0/Math::pi), m_axis[0], m_axis[1], m_axis[2]) ;
			Group::draw() ;
			restoreMatrix() ;
		}
	};
}

#endif