#ifndef _HelperGl_Color_H
#define _HelperGl_Color_H

#include <GL/freeglut.h>
#include <iostream>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Color
	///
	/// \brief	RGBA color.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	28/11/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Color
	{
	private:
		GLfloat m_color[4];

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color::Color(float r, float g, float b, float a=1.0f)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \param	r	Red component.
		/// \param	g	Green component.
		/// \param	b	Blue component.
		/// \param	a	Alpha component.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color(float r=0.0f, float g=0.0f, float b=0.0f, float a=1.0f)
		{
			m_color[0] = r ;
			m_color[1] = g ;
			m_color[2] = b ;
			m_color[3] = a ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Color::set(float color[4])
		///
		/// \brief	Sets the color.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \param	color	The color to set.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void set(float color[4])
		{
			for(int cpt=0 ;  cpt<4 ; ++cpt)
			{
				m_color[cpt] = color[cpt] ;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLfloat * Color::getGlColor()
		///
		/// \brief	Gets the color as an array of GLfloat compatible with OpenGL function calls.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \return	An array of 4 GLfloat.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLfloat * getBuffer()
		{
			return m_color ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	const GLfloat * Color::getBuffer() const
		///
		/// \brief	Gets the color as an array of GLfloat compatible with OpenGL function calls.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \return	An array of 4 GLfloat.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		const GLfloat * getBuffer() const
		{
			return m_color ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color Color::operator* (Color const & color) const
		///
		/// \brief	Multiplication operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	color	The color.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////

		Color operator* (Color const & color) const
		{
			return Color(m_color[0]*color.m_color[0], m_color[1]*color.m_color[1], m_color[2]*color.m_color[2], m_color[3]*color.m_color[3]) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color Color::operator* (float value) const
		///
		/// \brief	Multiplication operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	value	The value.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color operator* (float value) const
		{
			return Color(m_color[0]*value, m_color[1]*value, m_color[2]*value, m_color[3]*value) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color Color::operator/ (Color const & color) const
		///
		/// \brief	Division operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	color	The color.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color operator/ (Color const & color) const
		{
			return Color(m_color[0]/color.m_color[0], m_color[1]/color.m_color[1], m_color[2]/color.m_color[2], m_color[3]/color.m_color[3]) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color Color::operator/ (float value) const
		///
		/// \brief	Division operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	value	The value.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color operator/ (float value) const
		{
			return Color(m_color[0]/value, m_color[1]/value, m_color[2]/value, m_color[3]/value) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Color Color::operator+ (Color const & color) const
		///
		/// \brief	Addition operator.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	21/12/2015
		///
		/// \param	color	The color.
		///
		/// \return	The result of the operation.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Color operator+ (Color const & color) const
		{
			return Color(m_color[0]+color.m_color[0], m_color[1]+color.m_color[1], m_color[2]+color.m_color[2], m_color[3]+color.m_color[3]) ;
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \fn	inline Color lerp(Color const & p1, Color const & p2, float t)
	///
	/// \brief	Lerp: linera interpolation between colors.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	21/12/2015
	///
	/// \param	p1	The first Color.
	/// \param	p2	The second Color.
	/// \param	t 	The parameter in [0;1].
	///
	/// \return	The interpolated color.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Color lerp(Color const & p1, Color const & p2, float t)
	{
		return p1*(1-t)+p2*t ;
	}

	inline ::std::ostream & operator<<(::std::ostream & out, const Color & color)
	{
		out<<"("<<color.getBuffer()[0]<<","<<color.getBuffer()[1]<<","<<color.getBuffer()[2]<<","<<color.getBuffer()[3]<<")" ;
		return out ;
	}
}

#endif