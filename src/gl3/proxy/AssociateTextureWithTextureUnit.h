#pragma once

#include <GL/glew.h>
#include <functional>

namespace gl3
{
	namespace proxy
	{
		/// <summary>
		/// A proxy to an associateWithTextureUnit method.
		/// </summary>
		class AssociateTextureWithTextureUnit
		{
			std::function<void(GLuint)> m_association;

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="AssociateTextureWithTextureUnit"/> class.
			/// </summary>
			/// <param name="fnc">The function.</param>
			AssociateTextureWithTextureUnit(const std::function<void(GLuint)> & fnc)
				: m_association(fnc)
			{}

			/// <summary>
			/// Default constructor, creates a no operation function.
			/// </summary>
			AssociateTextureWithTextureUnit()
			{}

			/// <summary>
			/// Associates the texture with the provided texture unit.
			/// </summary>
			/// <param name="unit">The unit.</param>
			void associateWithTextureUnit(GLuint unit)
			{
				if(m_association) m_association(unit);
			}
		};
	}
}