#include <SceneGraph/Sphere.h>


SceneGraph::Sphere::Sphere( const HelperGl::Material & material, float radius/*=1.0*/, int slices/*=20*/, int stacks/*=20*/ ) : m_material(material), m_quadric(gluNewQuadric()), m_radius(radius), m_slices(slices), m_stacks(stacks)
{
	gluQuadricNormals(m_quadric, GLU_SMOOTH) ;
	if(m_material.hasTexture())
	{
		gluQuadricTexture(m_quadric, true);
	}
}

void SceneGraph::Sphere::draw()
{
	HelperGl::PhongShader::getSingleton()->enable(HelperGl::LightServer::getSingleton()->getLightMask(), m_material.hasTexture()) ;
	HelperGl::Draw::draw(m_material) ;
	gluSphere(m_quadric, m_radius, m_slices, m_stacks) ;
	HelperGl::PhongShader::getSingleton()->disable() ;
}

SceneGraph::Sphere::~Sphere()
{
	gluDeleteQuadric(m_quadric) ;
}
