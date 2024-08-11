#ifndef _SceneGraph_MeshVBO_v2_H
#define _SceneGraph_MeshVBO_v2_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SceneGraph/NodeInterface.h>
#include <HelperGl/Mesh.h>
#include <HelperGl/VBO.h>
#include <set>


namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	MeshVBO_v2
	///
	/// \brief	Vertex Buffer Object.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	10/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class MeshVBO_v2 : public NodeInterface
	{
	protected:
		/// \brief	The rendered mesh.
		const HelperGl::Mesh * m_mesh ;
		/// \brief	The Vertex Buffer Object.
		HelperGl::VBO * m_vbo ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	MeshVBO_v2::MeshVBO_v2(const HelperGl::Mesh * mesh)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \param	mesh	The mesh.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		MeshVBO_v2(const HelperGl::Mesh * mesh);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void MeshVBO_v2::draw()
		///
		/// \brief	Draws this object.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void draw();
	};

}

#endif