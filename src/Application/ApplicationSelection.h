#ifndef _Application_ApplicationSelection_H
#define _Application_ApplicationSelection_H

#include <Application/Base.h>
#include <vector>
#include <functional>
#include <string>
#include <type_traits>

namespace Application
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	ApplicationSelection
	///
	/// \brief	Class hanlding application selection.
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	15/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class ApplicationSelection
	{
	private:
		static ::std::vector<::std::function<Base * ()> > m_factories ;
		static ::std::vector<::std::string> m_descriptions ; 

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void ApplicationSelection::dump()
		///
		/// \brief	Dumps all registered applications.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void dump();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static int ApplicationSelection::select()
		///
		/// \brief	Selection of an application.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static int select();

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void ApplicationSelection::registerFactory(::std::function<Base* ()> const & factory,
		/// 	::std::string const & decription)
		///
		/// \brief	Registers an application factory.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	factory   	The factory. This is a function taking no parameters and returning an
		/// 					instance of application inheriting of Application::Base.
		/// \param	decription	The decription of the application.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void registerFactory(::std::function<Base* ()> const & factory, ::std::string const & decription);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class App> static void ApplicationSelection::registerFactory(::std::string const & description)
		///
		/// \brief	Registers an application of class App inheriting of Application::Base. This application
		/// 		MUST pocess a default constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \tparam	App	Type of the application.
		/// \param	description	The description of the application.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class App>
		static void registerFactory(::std::string const & description)
		{
			registerFactory([]() -> Base * { return new App ; }, description) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	template <class App> static void ApplicationSelection::registerFactory()
		///
		/// \brief	Registers an application of class App inheriting of Application::Base. This application
		/// 		MUST pocess a default constructor. The description of this application is its type...
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \tparam	App	Type of the application.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <class App>
		static void registerFactory()
		{
			registerFactory<App>(typeid(App).name()) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static bool ApplicationSelection::selectAndRun()
		///
		/// \brief	Asks user to select an application and run it. If the function returns false, no 
		/// 		application has been selected and the user wants to quit.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \return	true if it succeeds, false if the user decided to quit without running an application.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static bool selectAndRun();
	};
}

#endif