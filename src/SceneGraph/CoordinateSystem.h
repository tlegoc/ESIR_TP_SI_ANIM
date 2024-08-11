#ifndef _SceneGraph_CoordinateSystem_H
#define _SceneGraph_CoordinateSystem_H

#include <GL/glew.h>
#include <SceneGraph/NodeInterface.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	CoordinateSystem
	///
	/// \brief	Draws the current coordinate system. X: red, Y: green, Z: blue
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	06/03/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class CoordinateSystem : public NodeInterface
	{
	protected:
		float m_length ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	CoordinateSystem::CoordinateSystem(float length)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	06/03/2016
		///
		/// \param	length	The length of each axis.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		CoordinateSystem(float length)
			: m_length(length)
		{}

		virtual void draw()
		{
			glPushAttrib(GL_ENABLE_BIT) ;
			glDisable(GL_LIGHTING) ;

			glBegin(GL_LINES) ;
			glColor3f(1.0f, 0.0f, 0.0f) ;
			glVertex3f(0.0,0.0,0.0) ;
			glVertex3f(m_length, 0.0f, 0.0f) ;
			glColor3f(0.0f, 1.0f, 0.0f) ;
			glVertex3f(0.0,0.0,0.0) ;
			glVertex3f(0.0f, m_length, 0.0f) ;
			glColor3f(0.0f, 0.0f, 1.0f) ;
			glVertex3f(0.0,0.0,0.0) ;
			glVertex3f(0.0f, 0.0f, m_length) ;
			glEnd() ;

			glPopAttrib() ;
		}
	};
}

#endif