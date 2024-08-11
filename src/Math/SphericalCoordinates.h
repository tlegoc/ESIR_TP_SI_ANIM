#pragma once

#include <Math/Vectorf.h>
#include <Math/Constant.h>
#include <math.h>

namespace Math
{
	 /// <summary>
	 /// Computes the spherical coordinates of a 3D vector
	 /// </summary>
	class SphericalCoordinates
	{
	protected:
		float m_module ;
		float m_phy, m_theta ;
	
	private:
		/// <summary>
		/// Gets the angle between the vector (1,0) and the provided vector (x,y).
		/// </summary>
		/// <param name="x">The x coordinate.</param>
		/// <param name="y">The y coordinate.</param>
		/// <returns></returns>
		float angle(float x, float y)
		{
			if(x==0.0)
			{
				if(y>0.0) return float(pi/2.0) ;
				return float(-pi/2.0) ;
			}
			float res = atan(y/x) ;
			if(x<0.0)
			{
				return float(res+pi);
			}
			return res ;
		}
	
	public:
		/// <summary>
		/// Sets the vector for which the spherical coordinates should be computed.
		/// </summary>
		/// <param name="v">The vector.</param>
		void set(Math::Vector3f const & v)
		{
			m_module = v.norm() ;
			m_phy = acos(v[2]/m_module) ;
			m_theta = angle(v[0], v[1]) ;
		}
		
		/// <summary>
		/// Gets the vector.
		/// </summary>
		/// <returns></returns>
		Math::Vector3f get() const
		{
			return Math::makeVector(m_module*sin(m_phy)*cos(m_theta),
								 m_module*sin(m_phy)*sin(m_theta),
								 m_module*cos(m_phy)) ;
		}
	
		/// <summary>
		/// Initializes a new instance of the <see cref="SphericalCoordinates"/> class.
		/// </summary>
		/// <param name="v">The vector.</param>
		SphericalCoordinates(Math::Vector3f const & v)
		{
			set(v) ;
		}
		
		/// <summary>
		/// Gets the theta angle i.e. the angle between the vector projected on the (X,Y) plane and the X axis.
		/// </summary>
		/// <returns></returns>
		float getTheta() const
		{ return m_theta ; }
		
		/// <summary>
		/// Sets the theta angle i.e. the angle between the vector projected on the (X,Y) plane and the X axis.
		/// </summary>
		/// <param name="v">The v.</param>
		void setTheta(float v)
		{ m_theta = v ; }
		
		
		/// <summary>
		/// Gets the angle between the vector and the (X,Y) plane
		/// </summary>
		/// <returns></returns>
		float getPhy() const
		{ return m_phy ; }

		/// <summary>
		/// Sets the angle between the vector and the (X,Y) plane
		/// </summary>
		/// <param name="v">The v.</param>
		void setPhy(float v)
		{ m_phy = v ; }

		/// <summary>
		/// Gets the module i.e. the norm of the orignal vector.
		/// </summary>
		/// <returns></returns>
		float getModule() const
		{ return m_module ; }
		
		/// <summary>
		/// Sets the module i.e. the norm of the orignal vector.
		/// </summary>
		/// <returns></returns>
		void setModule(float v)
		{ m_module = v ; }
	} ;
}

