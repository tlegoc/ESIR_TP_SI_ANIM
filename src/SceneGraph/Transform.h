#ifndef _SceneGraph_Transform_H
#define _SceneGraph_Transform_H

#include <GL/glew.h>
#include <SceneGraph/Group.h>

namespace SceneGraph
{
	class Transform : public Group
	{
	private:
		/// \brief	The transform matrix storage[ 16].
		GLfloat m_transformStorage[16] ;

	protected:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Transform::storeMatrix()
		///
		/// \brief	Locally stores the current transformation matrix.
		/// 		This is a solution to the stack size limitations of OpenGL.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void storeMatrix()
		{
			glGetFloatv(GL_MODELVIEW_MATRIX, m_transformStorage) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Transform::restoreMatrix()
		///
		/// \brief	Restore the previously stored trasnformation matrix.
		/// 		This is a solution to the stack size limitations of OpenGL.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	01/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void restoreMatrix()
		{
			glLoadMatrixf(m_transformStorage) ;
		}

	public:
	};
}

#endif