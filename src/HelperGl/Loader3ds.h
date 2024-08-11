#ifndef _HelperGl_Loader3ds_H
#define _HelperGl_Loader3ds_H

#include <Config.h>
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>
#include <Math/Vectorf.h>
#include <HelperGl/Mesh.h>
#include <HelperGl/Material.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace HelperGl
{
	class Loader3ds
	{
	private:
		Lib3dsFile * m_file;

		::std::map<::std::string, Material *> m_materials;
		::std::vector<Mesh*> m_meshes ;

		void loadMaterial( Lib3dsMaterial * material, const ::std::filesystem::path & texturePath );

		void loadMaterials(const ::std::filesystem::path & texturePath);

		void loadMesh(Lib3dsMesh * mesh);

		void loadMeshes()
		{
			for(Lib3dsMesh * mesh = m_file->meshes ; mesh!=NULL ; mesh = mesh->next)
			{
				loadMesh(mesh) ;
			}
		}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Loader3ds::Loader3ds(const ::std::string & filename, const ::std::string & texturePath);
		///
		/// \brief	Constructor which loads the provided 3ds file.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	08/02/2016
		///
		/// \param	filename   	Filename of the file.
		/// \param	texturePath	Full pathname of the texture file.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Loader3ds(const ::std::filesystem::path & filename, const ::std::filesystem::path & texturePath);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<Mesh*> & Loader3ds::getMeshes()
		///
		/// \brief	Gets the loaded meshes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \return	The meshes.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<Mesh*> & getMeshes() const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	::std::vector<Material*> Loader3ds::getMaterials()
		///
		/// \brief	Gets the loaded materials.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		::std::vector<Material*> getMaterials() const;
	};
}

#endif 