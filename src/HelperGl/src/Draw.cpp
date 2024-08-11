#include <GL/glew.h>
#include <HelperGl/Draw.h>

namespace HelperGl
{


	void Draw::draw( const Material & material )
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.getAmbiant().getBuffer()) ;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.getDiffuse().getBuffer()) ;
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.getSpecular().getBuffer()) ;
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.getEmission().getBuffer()) ;
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.getShininess()) ;
		if(!material.hasTexture())
		{
			glDisable(GL_TEXTURE_2D) ;
		}
		else
		{
			glEnable(GL_TEXTURE_2D) ;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material.getTextureId()) ;
		}
	}

	void Draw::draw( const Mesh & mesh )
	{
		draw(*mesh.getMaterial()) ;
		// 1 - Choice of the shading model
		if(mesh.hasVerticesNormals()) { glShadeModel(GL_SMOOTH) ; }
		else { glShadeModel(GL_FLAT) ; }
		// 2 - Geometry rendering
		glBegin(GL_TRIANGLES) ;
		bool hasTextureCoordinates = mesh.hasTextureCoordinates() ;
		bool hasVerticesNormals = mesh.hasVerticesNormals() ;
		for(auto it = mesh.getFaces().begin() ; it!=mesh.getFaces().end() ; ++it)
		{
			const Mesh::Face & current = *it ;
			// If no normal per vertex is provided, we use the face normal
			if(!hasVerticesNormals) { glNormal3fv(current.m_normal.getBuffer()) ; }
			// First vertex
			if(hasTextureCoordinates) {glTexCoord2fv(mesh.getTextureCoordinates()[current.m_verticesIndexes[0]].getBuffer()) ;}
			if(hasVerticesNormals) { glNormal3fv(mesh.getVerticesNormals()[current.m_verticesIndexes[0]].getBuffer()) ; }
			glVertex3fv(mesh.getVertices()[current.m_verticesIndexes[0]].getBuffer()) ;
			// Second vertex
			if(hasTextureCoordinates) {glTexCoord2fv(mesh.getTextureCoordinates()[current.m_verticesIndexes[1]].getBuffer()) ;}
			if(hasVerticesNormals) { glNormal3fv(mesh.getVerticesNormals()[current.m_verticesIndexes[1]].getBuffer()) ; }
			glVertex3fv(mesh.getVertices()[current.m_verticesIndexes[1]].getBuffer()) ;
			// Third vertex
			if(hasTextureCoordinates) {glTexCoord2fv(mesh.getTextureCoordinates()[current.m_verticesIndexes[2]].getBuffer()) ;}
			if(hasVerticesNormals) { glNormal3fv(mesh.getVerticesNormals()[current.m_verticesIndexes[2]].getBuffer()) ; }
			glVertex3fv(mesh.getVertices()[current.m_verticesIndexes[2]].getBuffer()) ;
		}
		glEnd();
	}

}