#pragma once

#include <functional>
#include <gl3/VertexBufferObject.h>

namespace gl3
{
	namespace proxy
	{
		/// <summary>
		/// A proxy function for the setAttribute methods of a shader program
		/// </summary>
		class SetAttribute
		{
			std::function<void(const VertexBufferObject *, GLuint)> m_function1;
			std::function<void(const proxy::AttribPointer &)> m_function2;

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="SetAttribute"/> class.
			/// </summary>
			/// <param name="fnc1">The first function.</param>
			/// <param name="fnc2">The second function.</param>
			SetAttribute(const std::function<void(const VertexBufferObject *, GLuint)> & fnc1,
				const std::function<void(const proxy::AttribPointer &)> & fnc2)
				: m_function1(fnc1), m_function2(fnc2)
			{}

			/// <summary>
			/// Default constructor, creates a no operation function.
			/// </summary>
			SetAttribute()
			{}
			
			/// <summary>
			/// Sets the attribute.
			/// </summary>
			/// <param name="buffer">The vertex buffer object connected to the attribute.</param>
			/// <param name="nbInstances">The nb instances to pass before updating the attribute.</param>
			void setAttribute(const VertexBufferObject * buffer, GLuint nbInstances = 0)
			{
				if(m_function1) m_function1(buffer, nbInstances);
			}

			/// <summary>
			/// Sets the attribute.
			/// </summary>
			/// <param name="prox">The proxy enabling the connexion with the attribute.</param>
			void setAttribute(const proxy::AttribPointer & prox)
			{
				if(m_function2) m_function2(prox);
			}
		};
	}
}