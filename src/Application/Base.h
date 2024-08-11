#ifndef _Application_Base_H
#define _Application_Base_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Config.h>
#include <map>
#include <Application/Menu.h>
#include <tbb/tick_count.h>
#include <GL/compatibility.h>

namespace Application
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	Base
	///
	/// \brief	Base application. This class is a base for an OpenGL application rendering 3D objects
	/// 		with a perspective projection. This class provides default callbacks methods as well
	/// 		as the creation of a rendering context, helping to rapidly implement a simple OpenGL 
	/// 		application using GLUT and GLEW.
	/// 		
	/// 		Note that before creating any instance of this application, the static method initializeGLUT
	/// 		must be called from the main function. Moreover, the OpenGL application starts when the
	/// 		run method is called (this guarantees a correct order during initializations).
	/// 		
	/// \warning This application must be a singleton (i.e. exactly one instance of Application::Base 
	/// 		 can be created in an application).
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	15/02/2016
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class Base
	{
	private:
		/// \brief	true if GLUT and GLEW have been initialized. see initializeGLUT(int argc, char ** argv) method
		static bool s_initialized ;
		/// \brief	The curretly active instance of Application::Base.
		static Base * s_activeInstance ;
		/// \brief	Time of the last frame.
		tbb::tick_count m_lastFrameTime ;
		/// \brief	The dt (time elapsed since last rendered frame).
		double m_dt ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::displayCallback()
		///
		/// \brief	Display callback function. For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void displayCallback();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::reshapeCallback(GLint width, GLint height)
		///
		/// \brief	Callback, called when the window is reshaped.  For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	width 	The new width of the window.
		/// \param	height	The new height of the window.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void reshapeCallback(GLint width, GLint height);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::keyboardCallback(unsigned char key, int x, int y)
		///
		/// \brief	Callback, called when a keu is pressed.  For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	key	The key.
		/// \param	x  	The x coordinate of the mouse.
		/// \param	y  	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void keyboardCallback(unsigned char key, int x, int y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::keyboardCallback(unsigned char key, int x, int y)
		///
		/// \brief	Callback, called when a key is released.  For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	key	The key.
		/// \param	x  	The x coordinate of the mouse.
		/// \param	y  	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void keyboardUpCallback(unsigned char key, int x, int y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::mouseCallback(int button, int state, int x, int y)
		///
		/// \brief	Callback, called when a button is pressed or released.
		/// 		For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	button	The button.
		/// \param	state 	The state of the button.
		/// \param	x	  	The x coordinate.
		/// \param	y	  	The y coordinate.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void mouseCallback(int button, int state, int x, int y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::mouseMotionCallback(int x, int y)
		///
		/// \brief	Callback, called when the mouse moves and at least a button is pressed.
		/// 		For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	x	The x coordinate of the mouse.
		/// \param	y	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void mouseMotionCallback(int x, int y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::mousePassiveMotionCallback(int x, int y)
		///
		/// \brief	Callback, called when the mouse moves and no button is pressed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	x	The x coordinate of the mouse.
		/// \param	y	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void mousePassiveMotionCallback(int x, int y);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::closeCallback()
		///
		/// \brief	Callback, called when the window is closed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void closeCallback();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::registerCallbacks()
		///
		/// \brief	Registers the GLUT callbacks. For internal use and connection to GLUT.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void registerCallbacks();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::createWindow();
		///
		/// \brief	Creates the window.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void createWindow();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::initializeGLEW()
		///
		/// \brief	Initializes glew. For internal use only.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void initializeGLEW();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::initializeOpenGL();
		///
		/// \brief	Initializes OpenGL for the current application.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void initializeOpenGL();


	private:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::display();
		///
		/// \brief	Callback called when a new frame is rendered. For internal use, see render() method.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void display();

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \struct	Configuration
		///
		/// \brief	Configuration of the application. This structure has a default constructor which 
		/// 		provides a valid configuration. Those default values can be chnaged with caution.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		struct Configuration
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \typedef	FPS
			///
			/// \brief	An enumeration of frame rates.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			typedef enum { FpsMax=0, Fps60=1, Fps30=2} FPS ;

			/// \brief	Name of the window.
			::std::string m_windowName ;
			/// \brief	The display mode, default is GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE.
			GLuint m_displayMode ;
			/// \brief	The width of the window
			unsigned int width ;
			/// \brief	The height of the window
			unsigned int height ;
			/// \brief	Targetted frame rate. The frame rate will never be greater than the provided value (see FPS enum type).
			FPS m_fps ;
			/// \brief	Distance of the near clipping plane.
			GLfloat m_nearPlane ;
			/// \brief	Distance of the far clipping plane.
			GLfloat m_farPlane ;
			/// \brief	Angle of the field of view.
			GLfloat m_fovy ;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Configuration() 
			///
			/// \brief	Default constructor.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/02/2016
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Configuration()
				: m_windowName("OpenGL Application"), m_displayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE), 
				  width(Config::defaultWindowWidth), height(Config::defaultWindowHeight), m_fps(FpsMax), 
				  m_nearPlane(Config::defaultNearPlane), m_farPlane(Config::defaultFarPlane), m_fovy(Config::defaultFovy)
			{}
		};

	protected:
		/// \brief	The configuration.
		Configuration m_configuration ;
		/// \brief	Identifier for the window.
		int m_windowId ;
		/// \brief The last date of a render call
		unsigned int m_lastTime ;
		/// \brief Frame per second
		float m_fps ;
		/// \brief	true to draw FPS.
		bool m_drawFPS ;
		/// \brief	The main menu.
		Menu * m_mainMenu ;
		/// \brief	The FPS menu.
		Menu * m_fpsMenu ;
		/// \brief Functions that should be called when the window closes.
		::std::vector<::std::function<void ()> > m_onCloseFunctions ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Base * Base::getActiveApplication()
		///
		/// \brief	Gets the currently active application.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \return	null if it fails, else the active application.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Base * getActiveApplication()
		{
			return s_activeInstance ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::initializeRendering() = 0;
		///
		/// \brief	Initializes the rendering. This method is called once, before any call to the render 
		/// 		method. If you need to do specific initializations (like loading files, creating a 
		/// 		scene graph etc...), do it there.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void initializeRendering() = 0 ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::render() = 0;
		///
		/// \brief	This render method is called each frame to render your scene.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void render(double dt) = 0 ;

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
		virtual void keyPressed(unsigned char key, int x, int y) {}

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
		virtual void keyReleased(unsigned char key, int x, int y) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::mouse(int button, int state, int x, int y) = 0;
		///
		/// \brief	Mouse callback called when the state of a mouse button changes.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	button	The button (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON).
		/// \param	state 	The state of the mouse button (GLUT_UP, GLUT_DOWN)
		/// \param	x	  	The x coordinate.
		/// \param	y	  	The y coordinate.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void mouse(int button, int state, int x, int y) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::mouseMotion(int x, int y) = 0;
		///
		/// \brief	Callback called when the mouse moves and at least a button is pressed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	x	The x coordinate of the mouse.
		/// \param	y	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void mouseMotion(int x, int y) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual void Base::mousePassiveMotion(int x, int y) = 0;
		///
		/// \brief	Callback called when the mouse moves and no mouse button is pressed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	x	The x coordinate of the mouse.
		/// \param	y	The y coordinate of the mouse.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void mousePassiveMotion(int x, int y) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::reshape(GLint width, GLint height);
		///
		/// \brief	Reshapes the current window.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	width 	The width.
		/// \param	height	The height.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void reshape(GLint width, GLint height);

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	static void Base::initializeGLUT(int argc, char ** argv);
		///
		/// \brief	Initializes OpenGL and GLEW.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	argc The argc parameter of the program.
		/// \param argv	 The argv parameter of the program.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static void initializeGLUT(int & argc, char ** argv);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Base::Base(Configuration const & configuration = Configuration());
		///
		/// \brief	Constructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	configuration	(optional) the configuration.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Base(Configuration const & configuration = Configuration());

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	virtual Base::~Base();
		///
		/// \brief	Destructor.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ~Base();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::run();
		///
		/// \brief	Runs this application.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void run();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::quit()
		///
		/// \brief	Quits this application.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void quit()
		{
			// We ask to leave the main loop
			glutLeaveMainLoop() ;
			// Forces re-intialization of GLUT
			s_initialized = false ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::drawFPS(bool draw) };
		///
		/// \brief	Draw FPS.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	draw	true to draw, false otherwise.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void drawFPS(bool draw) { m_drawFPS = draw ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Menu * Base::getMenu()
		///
		/// \brief	Gets the menu.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \return	null if it fails, else the menu.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Menu * getMenu() { return m_mainMenu ; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void Base::onClose(::std::function<void ()> const & function)
		///
		/// \brief	Adds a function that will be called when the window is closed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/02/2016
		///
		/// \param	function	The function.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void onClose(::std::function<void ()> const & function)
		{
			m_onCloseFunctions.push_back(function) ;
		}

		/// <summary>
		/// Gets the dt.
		/// </summary>
		/// <returns></returns>
		double getDt() const
		{
			return m_dt ;
		}

		/// <summary>
		/// Gets the configuration.
		/// </summary>
		/// <returns></returns>
		const Configuration & getConfiguration() const
		{
			return m_configuration;
		}
	};
}

#endif
