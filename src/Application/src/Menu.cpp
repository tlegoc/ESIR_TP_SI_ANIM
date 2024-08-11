#include <Application/Menu.h>

namespace Application
{
	::std::vector<::std::function<void ()>> Menu::m_menuCallbacks ;

	void Menu::addSubMenu( Menu * menu )
	{
		bind() ;
		glutAddSubMenu(menu->m_title.c_str(), menu->m_menuId) ;
	}

	void Menu::addItem( ::std::string const & name, const ::std::function<void ()> & callback )
	{
		int id = m_menuCallbacks.size() ;
		m_menuCallbacks.push_back(callback) ;
		bind() ;
		glutAddMenuEntry(name.c_str(), id) ;
	}

	void Menu::deactivate()
	{
		if(m_isAttached)
		{
			glutDetachMenu(m_attachedTo) ;
			m_isAttached = false ;
		}
	}

	void Menu::activate( int button )
	{
		assert((button==GLUT_LEFT_BUTTON) || (button==GLUT_MIDDLE_BUTTON) | (button==GLUT_RIGHT_BUTTON)) ;
		deactivate() ;
		bind() ;
		glutAttachMenu(button) ;
	}

	Menu::~Menu()
	{
		glutDestroyMenu(m_menuId) ;
	}

	Menu::Menu( ::std::string const & title ) : m_title(title), m_menuId(-1), m_isAttached(false), m_attachedTo(-1)
	{
		m_menuId = glutCreateMenu(menuCallback) ;
	}

	void Menu::bind()
	{
		glutSetMenu(m_menuId) ;
	}

}