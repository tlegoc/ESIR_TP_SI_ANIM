#ifndef _SceneGraph_Scale_H
#define _SceneGraph_Scale_H

#include <SceneGraph/Transform.h>
#include <Math/Vectorf.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Scale
	///
	/// \brief	Applyes a scale factor to the sons.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Scale : public Transform
	{
	protected:
		Math::Vector3f m_scale ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Scale::Scale(const Math::Vector3f & scale)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	scale	The scale factors (on X, Y and Z)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Scale(const Math::Vector3f & scale)
			: m_scale(scale)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Math::Vector3f & Scale::getScale() const
		///
		/// \brief	Gets the scale factors.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \return	The scale.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Math::Vector3f & getScale() const
		{
			return m_scale ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Scale::setScale(const Math::Vector3f & scale)
		///
		/// \brief	Sets the scale factor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	scale	The scale.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setScale(const Math::Vector3f & scale)
		{
			m_scale = scale ;
		}

		virtual void draw()
		{
			storeMatrix() ;
			glScalef(m_scale[0], m_scale[1], m_scale[2]) ;
			Group::draw() ;
			restoreMatrix();
		}
	};
}

#endif