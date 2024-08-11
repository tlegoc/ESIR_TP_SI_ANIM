#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gl3
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
		glm::mat4x4 m_orientation;
		/// \brief	The camera position.
		glm::vec3 m_position;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Camera::Camera() : m_orientation(glm::mat4x4::getIdentity()),
		/// 	m_position(Math::makeVector(0.0f,0.0f,0.0f))
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Camera()
			: m_orientation(glm::mat4x4(1.0)), m_position(0.0f, 0.0f, 0.0f)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::setPosition(glm::vec3 const & position)
		///
		/// \brief	Sets the camera position.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	position	The position.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setPosition(glm::vec3 const & position)
		{
			m_position = position;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const glm::vec3 & Camera::getPosition() const
		///
		/// \brief	Gets the camera position.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The position.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const glm::vec3 & getPosition() const
		{
			return m_position;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::translateLocal(glm::vec3 const & translation)
		///
		/// \brief	Translates the camera position with a local vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	translation	The translation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void translateLocal(glm::vec3 const & translation)
		{
			m_position += glm::vec3(m_orientation * glm::vec4(translation, 1.0));
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
			translateLocal(glm::vec3(0.0f, 0.0f, -value));
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
			translateLocal(glm::vec3(0.0f, value, 0.0f));
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
			translateLocal(glm::vec3(value, 0.0f, 0.0f));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Camera::rotateLocal(const glm::vec3 & axis, float angle)
		///
		/// \brief	Rotates the camera.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \param	axis 	The axis.
		/// \param	angle	The angle (in radians).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void rotateLocal(const glm::vec3 & axis, float angle)
		{
			m_orientation = m_orientation * glm::rotate(angle, axis);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	glm::mat4x4 Camera::getTransform() const
		///
		/// \brief	Gets the camera transform.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The transform.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4x4 getTransform() const
		{
			return glm::translate(m_position)*m_orientation;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	glm::mat4x4 Camera::getInverseTransform() const
		///
		/// \brief	Gets the inverse of the camera transform (useful for rendering).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	22/03/2016
		///
		/// \return	The inverse transform.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4x4 getInverseTransform() const
		{
			return glm::inverse(getTransform()); // .inverse();
		}
	};

}