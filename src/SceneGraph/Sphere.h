#ifndef _SceneGraph_Sphere_H
#define _SceneGraph_Sphere_H

#include <GL/glew.h>
#include <HelperGl/Material.h>
#include <HelperGl/PhongShader.h>
#include <HelperGl/Draw.h>
#include <HelperGl/LightServer.h>
#include <SceneGraph/NodeInterface.h>

namespace SceneGraph
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Sphere
	///
	/// \brief	A sphere.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	04/04/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Sphere : public NodeInterface
	{
	protected:
		HelperGl::Material m_material ;
		GLUquadric * m_quadric ;
		float m_radius ;
		int m_slices ;
		int m_stacks ;

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Sphere::Sphere(const HelperGl::Material & material, float radius=1.0, int slices=20,
		/// 	int stacks=20);
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	04/04/2016
		///
		/// \param	material	The material.
		/// \param	radius  	(optional) the radius.
		/// \param	slices  	(optional) the number of slices.
		/// \param	stacks  	(optional) the number of stacks.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Sphere(const HelperGl::Material & material, float radius=1.0, int slices=20, int stacks=20);

		virtual void draw();

		virtual ~Sphere();
	};
}

#endif 