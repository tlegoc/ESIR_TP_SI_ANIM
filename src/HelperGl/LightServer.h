#ifndef _HelperGl_LightServer_H
#define _HelperGl_LightServer_H

#include <GL/freeglut.h>
#include <HelperGl/Color.h>
#include <Math/Vectorf.h>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace HelperGl
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class	LightServer
	///
	/// \brief	Light server that automatically handle light number allocation / deallocation. 
	///
	/// \author	F. Lamarche, Université de Rennes 1
	/// \date	28/11/2015
	////////////////////////////////////////////////////////////////////////////////////////////////////
	class LightServer
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	Light
		///
		/// \brief	A light that can be enabled / disabled.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		class Light
		{
		public:
			friend class LightServer ;

		protected:
			/// \brief	The position of the light.
			Math::Vector4f m_position ;
			/// \brief	The ambient color.
			Color m_ambient ;
			/// \brief	The diffuse color.
			Color m_diffuse ;
			/// \brief	The specular color.
			Color m_specular ;
			/// \brief	The identifier of the light (0 if the light is disabled).
			GLuint m_id ;

		protected:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Light::Light(Math::Vector3f const & position, Color const & ambient,
			/// 	Color const & diffuse, Color const & specular)
			///
			/// \brief	Constructor. You can't create your own lights without using the LightServer, please use 
			/// 		LightServer::createLight.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	position	The position.
			/// \param	ambient 	The ambient.
			/// \param	diffuse 	The diffuse.
			/// \param	specular	The specular.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Light(Math::Vector3f const & position, Color const & ambient, Color const & diffuse, Color const & specular)
				: m_position(position.pushBack(1.0f)), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_id(0)
			{}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Light::~Light()
			///
			/// \brief	Destructor. You can't destroy a light. Please use LighServer::destroyLight.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			////////////////////////////////////////////////////////////////////////////////////////////////////
			~Light()
			{
				if(isEnabled()) { disable() ; }
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::bind()
			///
			/// \brief	Binds this light (for internal use).
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void bind()
			{
				assert(m_id != 0) ;
				glDisable(m_id) ;
				glEnable(m_id) ;
				glLightfv(m_id, GL_AMBIENT, m_ambient.getBuffer()) ;
				glLightfv(m_id, GL_DIFFUSE, m_diffuse.getBuffer()) ;
				glLightfv(m_id, GL_SPECULAR, m_specular.getBuffer()) ;
				glLightfv(m_id, GL_POSITION, m_position.getBuffer()) ;
			}

		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::enable()
			///
			/// \brief	Enables the light.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void enable()
			{
				assert(m_id==0) ;
				m_id = LightServer::getSingleton()->allocateLight() ;
				bind();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::disable()
			///
			/// \brief	Disables the light.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void disable()
			{
				assert(m_id!=0) ;
				LightServer::getSingleton()->deallocateLight(m_id) ;
				m_id = 0 ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	bool Light::isEnabled() const
			///
			/// \brief	Query if this light is enabled.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \return	true if enabled, false if not.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			bool isEnabled() const
			{
				return m_id != 0 ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Color & Light::getAmbient() const
			///
			/// \brief	Gets the ambient color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \return	The ambient.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Color & getAmbient() const
			{
				return m_ambient ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::setAmbient(Color const & color)
			///
			/// \brief	Sets the ambient color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	color	The color.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void setAmbient(Color const & color)
			{
				m_ambient = color ;
				if(isEnabled())
				{
					glLightfv(m_id, GL_AMBIENT, m_ambient.getBuffer()) ;
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Color & Light::getDiffuse(Color const & color) const
			///
			/// \brief	Gets the diffuse color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	color	The color.
			///
			/// \return	The diffuse.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Color & getDiffuse(Color const & color) const
			{
				return m_diffuse ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::setDiffuse(Color const & color)
			///
			/// \brief	Sets the diffuse color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	color	The color.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void setDiffuse(Color const & color)
			{
				m_diffuse = color ;
				if(isEnabled())
				{
					glLightfv(m_id, GL_DIFFUSE, m_diffuse.getBuffer()) ;
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	const Color & Light::getSpecular() const
			///
			/// \brief	Gets the specular color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \return	The specular.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			const Color & getSpecular() const
			{
				return m_specular ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::setSpecular(Color const & color)
			///
			/// \brief	Sets the specular color.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	color	The color.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void setSpecular(Color const & color)
			{
				m_specular = color ;
				if(isEnabled())
				{
					glLightfv(m_id, GL_SPECULAR, m_specular.getBuffer()) ;					
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	Math::Vector3f Light::getPosition() const
			///
			/// \brief	Gets the position.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \return	The position.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			Math::Vector3f getPosition() const
			{
				return m_position.popBack() ;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \fn	void Light::setPosition(Math::Vector3f const & position)
			///
			/// \brief	Sets the position.
			///
			/// \author	F. Lamarche, Université de Rennes 1
			/// \date	15/12/2015
			///
			/// \param	position	The position.
			////////////////////////////////////////////////////////////////////////////////////////////////////
			void setPosition(Math::Vector3f const & position)
			{
				m_position = position.pushBack(1.0f) ;
				if(isEnabled())
				{
					glLightfv(m_id, GL_POSITION, m_position.getBuffer()) ;
				}
			}
		};

	private:
		/// \brief	The free lights.
		::std::set<GLint> m_freeLights ;
		/// \brief	The used lights.
		::std::set<GLint> m_usedLights ;
		/// \brief	The used lights. For each active light, a bit is set to one (index of the bit = index of the light)
		unsigned char m_lightMask ;
		/// \brief The allocated lights
		::std::vector<Light *> m_lights ;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	LightServer::LightServer()
		///
		/// \brief	Default constructor. It is private as the light server is a singleton.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		////////////////////////////////////////////////////////////////////////////////////////////////////
		LightServer()
			: m_lightMask(0)
		{
			for(int cpt=0 ; cpt<8 ; cpt++)
			{
				glDisable(GL_LIGHT0+cpt) ;
			}
		}

		~LightServer()
		{
			// We destroy all the allocated lights
			for(auto it=m_lights.begin(); it!=m_lights.end() ; ++it)
			{
				delete (*it) ;
			}
		}

	protected:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	int LightServer::freeLights() const
		///
		/// \brief	Gets the number of free lights.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \return	.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int freeLights() const
		{
			int result = 0 ;
			for(int cpt=0 ; cpt<8 ; ++cpt)
			{
				if(((m_lightMask>>cpt)&1)==0) // If the slot is free
				{
					result++ ;
				}
			}
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	GLint LightServer::allocateLight()
		///
		/// \brief	Allocates a light and return its identifier.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \return	The allocated light identifier.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		GLint allocateLight()
		{
			assert(freeLights()>0);
			for(int cpt=0 ; cpt<8 ; ++cpt)
			{
				if(((m_lightMask>>cpt)&1)==0) // If the slot is free
				{
					m_lightMask |= 1<<cpt ;
					glEnable(GL_LIGHT0+cpt) ;
					return GL_LIGHT0+cpt ;
				}
			}
			assert(false) ;
			return 8 ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LightServer::deallocateLight(GLint light)
		///
		/// \brief	Deallocates a light.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \param	light	The freed light.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void deallocateLight(GLint light)
		{
			unsigned int index = light-GL_LIGHT0 ;
			assert(((m_lightMask>>index)&1)==1) ;
			glDisable(light) ;
			m_lightMask &= ~(1<<index) ;
		}

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	Light * LightServer::createLight(Math::Vector3f const & position, Color const & ambient,
		/// 	Color const & diffuse, Color const & specular)
		///
		/// \brief	Creates a light. By default, the created light if not activated. Please, call the enable
		/// 		method if needed.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \param	position	The position.
		/// \param	ambient 	The ambient color.
		/// \param	diffuse 	The diffuse color.
		/// \param	specular	The specular color.
		///
		/// \return	The new light.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Light * createLight(Math::Vector3f const & position, Color const & ambient, Color const & diffuse, Color const & specular)
		{
			Light * result = new Light(position, ambient, diffuse, specular) ;
			m_lights.push_back(result) ;
			return result ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	void LightServer::destroyLight(Light * light)
		///
		/// \brief	Destroys the light described by light.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	15/12/2015
		///
		/// \param [in,out]	light	If non-null, the light.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void destroyLight(Light * light)
		{
			auto it = ::std::find(m_lights.begin(), m_lights.end(), light) ;
			if(it==m_lights.end()) { assert(0) ; return ; }
			::std::swap(m_lights.back(), (*it)) ;
			m_lights.pop_back() ;
			m_lights.erase(it) ;
			delete light ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	LightServer * LightServer::getSingleton()
		///
		/// \brief	Return the instance of light server.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	28/11/2015
		///
		/// \return	The instance of light server.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static LightServer * getSingleton()
		{
			static LightServer s_server ;
			return &s_server;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \fn	unsigned char LightServer::getLightMask()
		///
		/// \brief	Gets light mask. In this mask, the index of each bit set to 1 is
		/// 		the index of a used light.
		///
		/// \author	F. Lamarche, Université de Rennes 1
		/// \date	10/12/2015
		///
		/// \return	The light mask.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		unsigned char getLightMask()
		{
			return m_lightMask ;
		}

	};
}

#endif