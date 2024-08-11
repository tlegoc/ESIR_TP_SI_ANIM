#ifndef _Application_Menu_H
#define _Application_Menu_H

#include <GL/freeglut.h>
#include <vector>
#include <functional>
#include <cassert>
#include <string>
namespace Application
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Menu
	///
	/// \brief	Class that ease the use of GLUT menus.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	15/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Menu
	{
	private:
		/// \brief	The menu callbacks.
		static ::std::vector<::std::function<void ()>> m_menuCallbacks ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Menu::menuCallback(int selected)
		///
		/// \brief	Callback, called when an item is selected.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	selected	The selected.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void menuCallback(int selected)
		{
			m_menuCallbacks[selected]() ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Menu::bind();
		///
		/// \brief	Binds this menu as the current menu. For internal use only.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void bind();	
	
	protected:
		/// \brief	The title of the menu (not displayed if this menu is the top most one).
		::std::string m_title;
		/// \brief	Identifier for the menu.
		int m_menuId ;
		/// \brief	true if menu is attached to a button (see m_attachedTo).
		bool m_isAttached ;
		/// \brief	If this menu is attached to a button, this member provides this button.
		int m_attachedTo ;

	
	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Menu::Menu(::std::string const & title)
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	title	The title.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Menu(::std::string const & title);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Menu::~Menu()
		///
		/// \brief	Destructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		~Menu();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Menu::attach(int button)
		///
		/// \brief	Attaches the menu to a button
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	button	The button (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON).
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void activate(int button);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Menu::deactivate()
		///
		/// \brief	Deactivates this menu by removing any attachment to a button.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void deactivate();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Menu::addItem(::std::string const & name, const ::std::function<void ()> & callback)
		///
		/// \brief	Adds an item to this menu. When the item is selected, the callback function is called.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	name		The name of the item.
		/// \param	callback	The callback function.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void addItem(::std::string const & name, const ::std::function<void ()> & callback);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Menu::addSubMenu(Menu * menu)
		///
		/// \brief	Adds a sub menu to this menu.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param [in,out]	menu	If non-null, the menu.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void addSubMenu(Menu * menu);
	};
}

#endif
