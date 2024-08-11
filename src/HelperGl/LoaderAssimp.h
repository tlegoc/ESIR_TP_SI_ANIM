#pragma once

#include <HelperGl/Mesh.h>
#include <filesystem>

namespace HelperGl
{
	class LoaderAssimp
	{
	protected:
		/// \brief The loaded materials
		std::vector<Material*> m_materials;
		/// \brief The loaded meshes
		::std::vector<Mesh*> m_meshes;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	LoaderAssimp::LoaderAssimp(const ::std::string & filename, const ::std::string & texturePath);
		///
		/// \brief	Constructor which loads the provided 3ds file.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	27/03/2019
		///
		/// \param	filename   	Filename of the file.
		/// \param	texturePath	Full pathname of the texture file.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		LoaderAssimp(const ::std::filesystem::path & filename, const ::std::filesystem::path & texturePath);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const ::std::vector<Geometry*> & Loader3ds::getMeshes()
		///
		/// \brief	Gets the loaded meshes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \return	The meshes.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const ::std::vector<Mesh*> & getMeshes() const
		{
			return m_meshes;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	::std::vector<Material*> Loader3ds::getMaterials()
		///
		/// \brief	Gets the loaded materials.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		::std::vector<Material*> getMaterials() const
		{
			return m_materials;
		}
	};
}