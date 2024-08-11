#ifndef _SceneGraph_Translate_H
#define _SceneGraph_Translate_H

#include <SceneGraph/Transform.h>
#include <Math/Vectorf.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Translate
	///
	/// \brief	Applies a translation on the son nodes.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Translate : public Transform
	{
	protected:
		Math::Vector3f m_translation ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Translate::Translate(const Math::Vector3f & translation = Math::makeVector(0.0,0.0,0.0))
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	translation	(optional) the translation vector.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Translate(const Math::Vector3f & translation = Math::makeVector(0.0,0.0,0.0))
			: m_translation(translation)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Translate::setTranslation(const Math::Vector3f & translation)
		///
		/// \brief	Sets the translation vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	translation	The translation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setTranslation(const Math::Vector3f & translation)
		{
			m_translation = translation ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const Math::Vector3f & Translate::getTranslation() const
		///
		/// \brief	Gets the translation vector.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \return	The translation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const Math::Vector3f & getTranslation() const
		{
			return m_translation ;
		}

		virtual void draw()
		{
			storeMatrix() ;
			glTranslatef(m_translation[0], m_translation[1], m_translation[2]) ;
			Group::draw();
			restoreMatrix() ;			
		}
	};
}

#endif