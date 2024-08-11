#pragma once

#include <functional>
#include <GL/glew.h>

namespace gl3
{
	/// <summary>
	/// A proxy class used to associate an element contained in a vertex buffer to a vertex shader attribute.
	/// Just call attribPointer with the attribute index and everything will work ;)
	/// </summary>
	class AttribPointerProxy
	{
		std::function<void(GLint)> m_binding;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="AttribPointerProxy"/> class.
		/// </summary>
		/// <param name="fnc">The function used to associate an element in a vertex buffer with a vertex shader attribute.</param>
		AttribPointerProxy(const std::function<void(GLint)> & fnc)
			: m_binding(fnc)
		{}

		/// <summary>
		/// Associates an element in a vertex buffer with a vertex shader attribute.
		/// </summary>
		/// <param name="shaderAttributeIndex">Index of the shader attribute.</param>
		void attribPointer(GLint shaderAttributeIndex) const
		{
			m_binding(shaderAttributeIndex);
		}
	};
}