#include <Application/Base.h>
#include <HelperGl/Text.h>

namespace Application
{
	bool Base::s_initialized = false ;
	Base * Base::s_activeInstance = NULL ;

	Base::Base( Configuration const & configuration /*= Configuration()*/ ) : m_configuration(configuration), m_lastTime(0), m_drawFPS(false)
	{
		if(s_activeInstance!=NULL)
		{
			::std::cerr<<"Exactly one instance of Application::Base can be created in your program"<<::std::endl;
			exit(-1) ;
		}
		s_activeInstance = this ;
		if(!s_initialized)
		{
			::std::cerr<<"You must call Application::Base::initializeGLUT before creating any instance of Application::Base"<<::std::endl ;
			exit(-1) ;
		}
		createWindow() ;
		initializeOpenGL() ;
		registerCallbacks() ;
		m_mainMenu = new Menu("Main menu") ;
		m_fpsMenu = new Menu("Show FPS") ;
		m_fpsMenu->addItem("True", [this](){ drawFPS(true) ; }) ;
		m_fpsMenu->addItem("False", [this](){ drawFPS(false) ; }) ;
		m_mainMenu->addSubMenu(m_fpsMenu) ;
		m_mainMenu->activate(GLUT_RIGHT_BUTTON) ;

		//onClose([this]() 
		//{	// We destroy the current window
		//	glutDestroyWindow(m_windowId) ;
		//	// We destroy the menus
		//	delete m_fpsMenu ;
		//	delete m_mainMenu ;
		//} ) ;
	}

	Base::~Base()
	{
		// No more active instance
		s_activeInstance = NULL ;
	}

	void Base::initializeGLUT( int & argc, char ** argv )
	{
		if(!s_initialized)
		{
			// GLUT initialization
			glutInit(&argc, argv) ;
			//glutInitContextVersion(3,0);
			//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
			//glutInitContextProfile(GLUT_CORE_PROFILE);
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
			s_initialized = true ;
		}
	}

	void Base::createWindow()
	{
		glutInitWindowSize (m_configuration.width, m_configuration.height);
		//glutSetOption(GLUT_MULTISAMPLE, 4);
		glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ; // | GLUT_MULTISAMPLE); // GLUT_MULTISAMPLE enables antialiasing
		m_windowId = glutCreateWindow (m_configuration.m_windowName.c_str());
		// Initializes GLEW after the creation of a rendering context (mandatory...)
		initializeGLEW();
		// FPS constraint (does not work under Linux)
		//wglSwapIntervalEXT(m_configuration.m_fps) ;
	}

	void Base::initializeOpenGL()
	{
		// Default Open GL initialization
		glEnable(GL_DEPTH_TEST) ; // Enables depth buffer
		glShadeModel(GL_SMOOTH) ; // Enables smooth shading model
		glEnable(GL_LIGHTING) ; // Enables lights
		//glDisable(GL_CULL_FACE) ; // disables back face culling
		glEnable(GL_CULL_FACE) ; // enables back face culling
		glCullFace(GL_BACK) ;
		glFrontFace(GL_CCW) ; 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST) ;
		glEnable( GL_NORMALIZE );
		glLoadIdentity() ;
	}

	void Base::reshape( GLint width, GLint height )
	{
		// Updates the view port size
		glViewport(0, 0, width, height);
		// Changes the projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(m_configuration.m_fovy, (float)width / height, m_configuration.m_nearPlane, m_configuration.m_farPlane);
		glMatrixMode(GL_MODELVIEW);
		// Updates the configuration
		m_configuration.width = width;
		m_configuration.height = height;
	}

	void Base::run()
	{
		// Runs user initializations
		initializeRendering() ;

		m_lastFrameTime = ::tbb::tick_count::now() ;
		// Calls the glut main loop
		glutMainLoop();
	}

	void Base::display()
	{
		::tbb::tick_count newTime = ::tbb::tick_count::now() ;
		m_dt = (newTime-m_lastFrameTime).seconds() ;
		m_lastFrameTime = newTime ;

		// Clears frame and z buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
		// Reinitializes transformations
		glLoadIdentity();
		// Calls the main rendering function
		render(m_dt);
		// We ensure that no shader program is binded (I noticed some crashes with intel drivers...)
		glUseProgram(0);
		// Optionaly draw the FPS
		if(m_drawFPS)
		{
			HelperGl::Text::getStream() << "FPS: " << (unsigned int)(1.0/m_dt) <<::std::endl ;
			HelperGl::Text::getStream() << "Time: " << (unsigned int)(m_dt*10000)/10.0 << "ms"<<::std::endl ;
		}
		HelperGl::Text::display(12, 24, HelperGl::Color(0.0,1.0,0.0)) ;
		// Swaps rendering buffers
		glutSwapBuffers();
		// Asks for redisplay
		glutPostRedisplay();

	}

	void Base::registerCallbacks()
	{
		glutDisplayFunc(displayCallback) ;
		glutReshapeFunc(reshapeCallback) ;
		glutKeyboardFunc(keyboardCallback) ;
		glutKeyboardUpFunc(keyboardUpCallback) ;
		glutMouseFunc(mouseCallback) ;
		glutPassiveMotionFunc(mousePassiveMotionCallback) ;
		glutCloseFunc(closeCallback) ;
	}

	void Base::mousePassiveMotionCallback( int x, int y )
	{
		s_activeInstance->mousePassiveMotion(x,y) ;
	}

	void Base::mouseMotionCallback( int x, int y )
	{
		s_activeInstance->mouseMotion(x,y) ;
	}

	void Base::mouseCallback( int button, int state, int x, int y )
	{
		s_activeInstance->mouse(button, state, x, y) ;
	}

	void Base::keyboardCallback( unsigned char key, int x, int y )
	{
		s_activeInstance->keyPressed(key, x, y) ;
	}

	void Base::reshapeCallback( GLint width, GLint height )
	{
		s_activeInstance->reshape(width, height) ;
	}

	void Base::displayCallback()
	{
		s_activeInstance->display() ;
	}

	void Base::initializeGLEW()
	{
		static bool initialized = false ;
		if(!initialized)
		{
			// GLEW initialization
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				/* Problem: glewInit failed, something is seriously wrong. */
				::std::cerr<<"Failed to initializeGLUT GLEW: "<<glewGetErrorString(err)<<::std::endl ;
				exit(-1);
			}
			initialized = true ;
		}
	}

	void Base::closeCallback()
	{
		for(unsigned int cpt=0 ; cpt<s_activeInstance->m_onCloseFunctions.size() ; ++cpt)
		{
			s_activeInstance->m_onCloseFunctions[cpt]() ;
		}
	}

	void Base::keyboardUpCallback( unsigned char key, int x, int y )
	{
		s_activeInstance->keyReleased(key,x,y) ;
	}

}
