#include <HelperGl/PhongShader.h>
#include <Config.h>

namespace HelperGl
{


	unsigned int PhongShader::computeIdentifier( unsigned char lightMask, bool useTexture )
	{
		unsigned int identifier = lightMask ;
		if(useTexture) { identifier |= 1<<8 ; } 
		return identifier ;
	}

	ShaderProgram * PhongShader::compileShaderProgram( unsigned char lightMask, bool useTexture )
	{
		Shader * fragmentShader = new Shader(Shader::fragment) ;
		for(int cpt=0 ; cpt<8 ; ++cpt)
		{
			if(((lightMask>>cpt)&1)==1)
			{
				fragmentShader->getStream()<<"#define USE_LIGHT"<<cpt<<::std::endl ;
			}
		}
		if(useTexture)
		{
			fragmentShader->getStream()<<"#define USE_TEXTURE0"<<::std::endl ;
		}
		fragmentShader->loadFromFile(Config::PhongFragmentShaderFile().string()) ;
		fragmentShader->compile() ;
		ShaderProgram * program = new ShaderProgram(m_vertexShader, fragmentShader) ;
		program->compileAndLink() ;
		return program ;
	}

	PhongShader::PhongShader()
	{
		m_vertexShader = new Shader(Shader::vertex) ;
		m_vertexShader->loadFromFile(Config::PhongVertexShaderFile().string()) ;
		m_vertexShader->compile() ;
	}

	PhongShader * PhongShader::getSingleton()
	{
		static PhongShader * s_shader = new PhongShader ;
		return s_shader ;
	}

	void PhongShader::enable( unsigned char lightMask, bool useTexture )
	{
		// Computes the identifier of the shader
		unsigned int identifier = computeIdentifier(lightMask, useTexture);
		// Retrieves the shader
		auto it=m_precompiledShaders.find(identifier) ;
		if(it!=m_precompiledShaders.end()) 
		{ 
			it->second->enable() ; 
			return ;
		}
		// The shader has not been compiled... Let's go!
		ShaderProgram * program = compileShaderProgram(lightMask, useTexture );
		m_precompiledShaders[identifier] = program ;
		program->enable() ;
	}

	void PhongShader::disable()
	{
		if(m_precompiledShaders.size()>0)
			m_precompiledShaders.begin()->second->disable() ;
	}

}