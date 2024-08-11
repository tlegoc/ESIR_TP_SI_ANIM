#ifndef _HelperGl_Text_H
#define _HelperGl_Text_H

#include <HelperGl/Color.h>
#include <GL/freeglut.h>
#include <sstream>

namespace HelperGl
{
	class Text
	{
	private:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Text::setOrthographicProjection();
		///
		/// \brief	Sets orthographic projection mode.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void setOrthographicProjection();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Text::resetOrthographicProjection();
		///
		/// \brief	Resets the orthographic projection mode.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void resetOrthographicProjection();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Text::display(GLfloat x, GLfloat y, const ::std::string & text,
		/// 	const HelperGl::Color & color);
		///
		/// \brief	Displays the provided string.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param	x	 	The x position of the text.
		/// \param	y	 	The y position of the text.
		/// \param	text 	The text.
		/// \param	color	The color.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void display(GLfloat x, GLfloat y, const ::std::string & text, const HelperGl::Color & color);

		/// \brief	The output stream used to display strings in OpenGL rendering context.
		static ::std::stringstream * out ;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Text::getStream()
		///
		/// \brief	Gets the output stream.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/03/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static ::std::ostream & getStream()
		{
			return *out ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Text::display(GLfloat x, GLfloat y,
		/// 	const HelperGl::Color & color = HelperGl::Color(1.0,1.0,1.0))
		///
		/// \brief	Displays the content of out in the rendering context.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	29/11/2015
		///
		/// \param	x	 	The x coordinate where display starts.
		/// \param	y	 	The y coordinate where display starts.
		/// \param	color	The font color.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void display(GLfloat x, GLfloat y, const HelperGl::Color & color = HelperGl::Color(0.0,1.0,0.5));
	} ;
}

#endif
