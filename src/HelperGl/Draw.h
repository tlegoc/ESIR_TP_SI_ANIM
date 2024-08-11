#ifndef _HelperGl_Draw_H
#define _HelperGl_Draw_H

#include <HelperGl/Material.h>
#include <HelperGl/Mesh.h>

namespace HelperGl
{
	namespace Draw
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	inline void draw(const Material & material)
		///
		/// \brief	Sends OpenGL commands to configure rendering with the given material.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param	material	The material.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void draw(const Material & material);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	inline void draw(const Mesh & mesh)
		///
		/// \brief	Sends OpenGL commands to draw the given mesh.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param	mesh	The mesh.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void draw(const Mesh & mesh);
	}
}

#endif