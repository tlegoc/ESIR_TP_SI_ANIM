#ifndef _HelperGl_PhongShader_H
#define _HelperGl_PhongShader_H

#include <HelperGl/Shader.h>
#include <HelperGl/ShaderProgram.h>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	PhongShader
	///
	/// \brief	A parameterizable Phong shader. You can select lights and texture use
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	10/12/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class PhongShader 
	{
	protected:
		/// \brief	The pre-compiled vertex shader.
		Shader * m_vertexShader ;

		/// \brief	The pre-compiled fragement shaders.
		::std::map<unsigned int, ShaderProgram *> m_precompiledShaders ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	unsigned int PhongShader::computeIdentifier( unsigned char lightMask, bool useTexture )
		///
		/// \brief	Calculates the identifier of the shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \param	lightMask 	The light mask.
		/// \param	useTexture	true to use texture.
		///
		/// \return	The calculated identifier.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned int computeIdentifier( unsigned char lightMask, bool useTexture );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	ShaderProgram * PhongShader::compileShaderProgram( unsigned char lightMask,
		/// 	bool useTexture )
		///
		/// \brief	Compile shader program fulfilling the lights and texture requirements.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \param	lightMask 	The light mask.
		/// \param	useTexture	true to use texture.
		///
		/// \return	null if it fails, else.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		ShaderProgram * compileShaderProgram( unsigned char lightMask, bool useTexture );

	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	PhongShader::PhongShader()
		///
		/// \brief	Default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		PhongShader();

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static PhongShader * PhongShader::getSingleton()
		///
		/// \brief	Gets the Phong shader singleton.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \return	null if it fails, else the singleton.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static PhongShader * getSingleton();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void PhongShader::enable(unsigned char lightMask, bool useTexture)
		///
		/// \brief	Enables the shader fulfilling the requirements (possbly compiles it).
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \param	lightMask 	The light mask.
		/// \param	useTexture	true to use texture.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void enable(unsigned char lightMask, bool useTexture);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void PhongShader::disable()
		///
		/// \brief	Disables the current shader.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void disable();
	};
}

#endif