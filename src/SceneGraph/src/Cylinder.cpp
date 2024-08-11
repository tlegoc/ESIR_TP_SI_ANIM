#include <SceneGraph/Cylinder.h>

SceneGraph::Cylinder::Cylinder( const HelperGl::Material & material, float baseRadius/*=1.0*/, float topRadius/*=1.0*/, float height/*=1.0*/, int slices/*=20*/, int stacks/*=1*/ ) : m_material(material), m_quadric(gluNewQuadric()), m_baseRadius(baseRadius), m_topRadius(topRadius), m_height(height), m_slices(slices), m_stacks(stacks)
{
	gluQuadricNormals(m_quadric, GLU_SMOOTH) ;
	if(m_material.hasTexture())
	{
		gluQuadricTexture(m_quadric, true);
	}
}

void SceneGraph::Cylinder::draw()
{
	HelperGl::PhongShader::getSingleton()->enable(HelperGl::LightServer::getSingleton()->getLightMask(), m_material.hasTexture()) ;
	HelperGl::Draw::draw(m_material) ;
	gluCylinder(m_quadric, m_baseRadius, m_topRadius, m_height, m_slices, m_stacks) ;
	glTranslatef(0.0,0.0,m_height) ;
	gluDisk(m_quadric, 0.0f, m_topRadius, m_slices, 1) ;
	glTranslatef(0.0,0.0,-m_height) ;
	//glScalef(-1.0,-1.0,1.0) ;
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f) ;
	gluDisk(m_quadric, 0.0f, m_baseRadius, m_slices, 1) ;
	glRotatef(-180.0f, 1.0f, 0.0f, 0.0f) ;
	//glScalef(-1.0,-1.0,1.0) ;
	HelperGl::PhongShader::getSingleton()->disable() ;
}

SceneGraph::Cylinder::~Cylinder()
{
	gluDeleteQuadric(m_quadric) ;
}
