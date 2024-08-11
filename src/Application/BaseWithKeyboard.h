#ifndef _Application_BaseWithKeyboard_H
#define _Application_BaseWithKeyboard_H

#include <Application/Base.h>
#include <Application/KeyboardStatus.h>

namespace Application
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	BaseWithKeyboard
	///
	/// \brief	Base application with an associated KeyboardStatus instance to facilitate keyboard
	/// 		interactions.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	22/03/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class BaseWithKeyboard : public Base
	{
	protected:
		/// \brief	The keyboard.
		KeyboardStatus m_keyboard ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::keyPressed(unsigned char key, int x, int y) = 0;
		///
		/// \brief	The keypressed call back. This method is called at each frame, when a key is pressed.
		/// 		If a key remains pressed during multiple frames, this method is called multiple times.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	key	The key.
		/// \param	x  	The x coordinate of the mouse.
		/// \param	y  	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void keyPressed(unsigned char key, int x, int y) 
		{
			m_keyboard.press(key) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::keyReleased(unsigned char key, int x, int y) = 0;
		///
		/// \brief	The key released call back. This method is called at each frame, when a key is released.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	key	The key.
		/// \param	x  	The x coordinate of the mouse.
		/// \param	y  	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void keyReleased(unsigned char key, int x, int y) 
		{
			m_keyboard.release(key) ;
		}
	};
}

#endif