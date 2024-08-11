#include <Application/ApplicationSelection.h>

namespace Application
{
	::std::vector<::std::function<Base * ()> > ApplicationSelection::m_factories ;
	::std::vector<::std::string> ApplicationSelection::m_descriptions ;

	bool ApplicationSelection::selectAndRun()
	{
		Base * current = NULL ;
		dump() ;
		int selected = select() ;
		if(selected!=-1)
		{
			current = m_factories[selected]() ;
			current->run() ;
			delete current ;
			return true ;
		}
		return false ;
	}

	void ApplicationSelection::registerFactory( ::std::function<Base* ()> const & factory, ::std::string const & decription )
	{
		m_factories.push_back(factory) ;
		m_descriptions.push_back(decription) ;
	}

	int ApplicationSelection::select()
	{
		while(true)
		{
			::std::cout<<"Selection (0 - "<<m_factories.size()-1<<") : "<<::std::flush ;
			int selected = 0 ;
			::std::cin>>selected ;
			if(selected>=-1 && selected<(int)m_factories.size()) { return selected ; }
		}
	}

	void ApplicationSelection::dump()
	{
		::std::cout<<"-1 - Quit selection"<<::std::endl ;
		for(unsigned int cpt=0 ; cpt<m_descriptions.size() ; ++cpt)
		{
			if(cpt<10) { ::std::cout<<" " ; }
			::std::cout<<cpt<<" - "<<m_descriptions[cpt]<<::std::endl ;
		}
	}

}