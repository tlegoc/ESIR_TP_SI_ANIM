#ifndef _SceneGraph_DisplayList_H
#define _SceneGraph_DisplayList_H

#include <GL/glew.h>
#include <stddef.h>
#include <SceneGraph/NodeInterface.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	DisplayList
	///
	/// \brief	A node that compiles a display list to render its son. This optimizes rendering. Warning
	/// 		the state of the son node should not change as the display list will not be compiled
	/// 		each time the draw method is called.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	29/11/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class DisplayList : public virtual NodeInterface
	{
	private:
		/// \brief	The index of the display list (-1 if not compiled yet).
		int m_index ;
		/// \brief	The son node (compiled into a display list).
		NodeInterface * m_son ;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	DisplayList::DisplayList(NodeInterface * son=NULL)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param	son	(optional) [in,out] If non-null, the son node.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		DisplayList(NodeInterface * son=NULL)
			: m_index(-1), m_son(NULL)
		{}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void DisplayList::setSon(NodeInterface * son)
		///
		/// \brief	Sets the son node.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param [in,out]	son	If non-null, the son.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setSon(NodeInterface * son)
		{
			if(m_index!=-1)
			{
				glDeleteLists(m_index, 1) ;
				m_index = -1 ;
			}
			m_son = son ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void DisplayList::recompile()
		///
		/// \brief	Forces the compilation of thios display list.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void recompile()
		{
			setSon(m_son) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void DisplayList::draw()
		///
		/// \brief	Draws this display list.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void draw()
		{
			// If the list is not compiled yet, we compile it, else we call it
			if(m_index==-1)
			{
				m_index = glGenLists(1) ;
				glNewList(m_index, GL_COMPILE);
				if(m_son!=NULL)
				{
					m_son->draw() ;
				}
				glEndList();
			}
			else
			{
				glCallList(m_index) ;
			}
		}
	};
}

#endif