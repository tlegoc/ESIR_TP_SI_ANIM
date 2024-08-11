#ifndef _HelperGl_Camera_H
#define _HelperGl_Camera_H

#include <Math/Matrix4x4f.h>
#include <Math/Vectorf.h>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Camera
	///
	/// \brief	A camera looking toward -Z (default OpenGL camera)
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	22/03/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Camera
	{
	protected:
		/// \brief	The camera orientation matrix.
		Math::Matrix4x4f m_orientation ;
		/// \brief	The camera position.
		Math::Vector3f m_position ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Camera::Camera() : m_orientation(Math::Matrix4x4f::getIdentity()),
		/// 	m_position(Math::makeVector(0.0f,0.0f,0.0f))
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Camera()
			: m_orientation(Math::Matrix4x4f::getIdentity()), m_position(Math::makeVector(0.0f,0.0f,0.0f))
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::setPosition(Math::Vector3f const & position)
		///
		/// \brief	Sets the camera position.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	position	The position.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setPosition(Math::Vector3f const & position)
		{
			m_position = position ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Math::Vector3f & Camera::getPosition() const
		///
		/// \brief	Gets the camera position.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The position.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Math::Vector3f & getPosition() const
		{
			return m_position ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::translateLocal(Math::Vector3f const & translation)
		///
		/// \brief	Translates the camera position with a local vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	translation	The translation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void translateLocal(Math::Vector3f const & translation)
		{
			m_position += m_orientation*translation ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::translateFront(float value)
		///
		/// \brief	Translate toward the front.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	value	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void translateFront(float value)
		{
			translateLocal(Math::makeVector(0.0f, 0.0f, -value)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::translateUp(float value)
		///
		/// \brief	Translate the camera up.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	value	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void translateUp(float value)
		{
			translateLocal(Math::makeVector(0.0f, value, 0.0f)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::translateRight(float value)
		///
		/// \brief	Translate the camera on the right.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	value	The value.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void translateRight(float value)
		{
			translateLocal(Math::makeVector(value, 0.0f, 0.0f)) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::rotateLocal(const Math::Vector3f & axis, float angle)
		///
		/// \brief	Rotates the camera.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	axis 	The axis.
		/// \param	angle	The angle (in radians).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void rotateLocal(const Math::Vector3f & axis, float angle)
		{
			m_orientation = m_orientation * Math::Matrix4x4f::getRotation(axis, angle) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Matrix4x4f Camera::getTransform() const
		///
		/// \brief	Gets the camera transform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The transform.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Matrix4x4f getTransform() const
		{
			return Math::Matrix4x4f::getTranslation(m_position)*m_orientation ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Math::Matrix4x4f Camera::getInverseTransform() const
		///
		/// \brief	Gets the inverse of the camera transform (useful for rendering).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The inverse transform.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Math::Matrix4x4f getInverseTransform() const
		{
			return getTransform().inverse();
		}
	};
}

#endif