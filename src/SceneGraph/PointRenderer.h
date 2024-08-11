#ifndef _SceneGraph_PointRenderer_H
#define _SceneGraph_PointRenderer_H

#include <SceneGraph/NodeInterface.h>
#include <HelperGl/Buffer.h>
#include <HelperGl/Color.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	PointRenderer
	///
	/// \brief	Point renderer class. This class renders points with optional colors.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	21/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class PointRenderer : public NodeInterface 
	{
	protected:
		/// \brief	Buffer for point position data.
		HelperGl::Buffer<Math::Vector3f>  * m_positionBuffer ;
		/// \brief	Number of points used for rendering.
		size_t m_pointCount ;
		/// \brief	Buffer for point color data.
		HelperGl::Buffer<HelperGl::Color> * m_colorBuffer ;
		/// \brief	Size of the point.
		float m_pointSize ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	PointRenderer::PointRenderer(HelperGl::Buffer<Math::Vector3f> * positionBuffer,
		/// 	float pointSize = 1.0, HelperGl::Buffer<HelperGl::Color> * colorBuffer=NULL)
		///
		/// \brief	Constructor. This constructor takes a.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	18/12/2015
		///
		/// \param [in,out]	positionBuffer	If non-null, buffer for position data.
		/// \param	pointSize			  	(optional) size of the point (default 1.0).
		/// \param	colorBuffer			  	(optional) [in,out] If non-null, buffer for color data.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PointRenderer(HelperGl::Buffer<Math::Vector3f> * positionBuffer, float pointSize = 1.0,
					  HelperGl::Buffer<HelperGl::Color> * colorBuffer=NULL)
			: m_positionBuffer(positionBuffer), m_pointCount(m_positionBuffer->size()),
			  m_colorBuffer(colorBuffer), m_pointSize(pointSize)	 
		{
			if(!m_positionBuffer->isRegistered())
			{
				m_positionBuffer->registerBuffer(GL_DYNAMIC_DRAW) ;
			}
			if(m_colorBuffer!=NULL && !m_colorBuffer->isRegistered())
			{
				m_colorBuffer->registerBuffer(GL_DYNAMIC_DRAW) ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void PointRenderer::setPointCount(size_t count)
		///
		/// \brief	Sets the number of points to render.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	count	Number of point that must be rendered.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void setPointCount(size_t count)
		{
			m_pointCount = count ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void PointRenderer::draw()
		///
		/// \brief	Draws this object.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void draw() 
		{
			glPushAttrib(GL_ENABLE_BIT) ;
			glDisable(GL_LIGHTING) ;
			glPointSize(m_pointSize) ;

			// Vertex Buffer
			glEnableClientState(GL_VERTEX_ARRAY) ;
			m_positionBuffer->bindBuffer();
			glVertexPointer(3, GL_FLOAT, 0, NULL) ;
			// Color buffer 
			if(m_colorBuffer!=NULL)
			{
				glEnableClientState(GL_COLOR_ARRAY) ;
				m_colorBuffer->bindBuffer() ;
				glColorPointer(4, GL_FLOAT, 0, NULL) ;
			}
			// Draws the points
			glDrawArrays(GL_POINTS, 0, m_pointCount) ;
			
			if(m_colorBuffer!=NULL)
			{
				glDisableClientState(GL_COLOR_ARRAY) ;
				m_colorBuffer->unbindBuffer() ;
			}
			glDisableClientState(GL_VERTEX_ARRAY) ;
			m_positionBuffer->unbindBuffer();
			
			glPopAttrib() ;
		}
	};
}

#endif