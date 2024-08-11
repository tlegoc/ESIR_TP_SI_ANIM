#include <HelperGl/Text.h>

std::stringstream * HelperGl::Text::out = new ::std::stringstream ;

void HelperGl::Text::display( GLfloat x, GLfloat y, const HelperGl::Color & color /*= HelperGl::Color(1.0,1.0,1.0)*/ )
{
	int nbCharPerLine = glutGet(GLUT_WINDOW_WIDTH)/12 ;
	char * line = new char[nbCharPerLine+1] ;
	while(out->getline(line, nbCharPerLine+1))
	{		
		display(x,y,line, color) ;
		y += 12.0 ;
	}
	// Trick to reset the stringstream (there is a strange behavior with the use of str to reset the stream... and operator = is not supported by g++)
	delete out ;
	out = new ::std::stringstream ;
	//out.str("") ; //(::std::stringstream());
	//out = (::std::stringstream());
	delete[] line ;
}

void HelperGl::Text::display( GLfloat x, GLfloat y, const ::std::string & text, const HelperGl::Color & color )
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING) ;
	glDisable(GL_TEXTURE_2D) ;
	glDisable(GL_DEPTH_TEST) ;
	glDisable(GL_CULL_FACE) ;
	
	setOrthographicProjection();
	glPushMatrix() ;
	glLoadIdentity();
	glColor3f(color.getBuffer()[0], color.getBuffer()[1], color.getBuffer()[2]);
	glRasterPos2f(x, y);
	for(size_t cpt=0 ; cpt<text.size() ; ++cpt)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[cpt]);
	}
	glPopMatrix();
	resetOrthographicProjection();
	glPopAttrib();
}

void HelperGl::Text::resetOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void HelperGl::Text::setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glScalef(1, -1, 1);
	glTranslatef(0.0f, (float)-glutGet(GLUT_WINDOW_HEIGHT), 0.0f);
	glMatrixMode(GL_MODELVIEW);
}
