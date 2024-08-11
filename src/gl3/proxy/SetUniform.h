#pragma once

namespace gl3
{
	namespace proxy
	{
		/// <summary>
		/// A proxy class to set uniforms in shaders.
		/// </summary>
		template <class Type>
		class SetUniform
		{
			std::function<void(const Type &)> m_function;
		
		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="SetUniform"/> class.
			/// </summary>
			/// <param name="fnc">The function.</param>
			SetUniform(const std::function<void(const Type &)> & fnc)
				: m_function(fnc)
			{}

			/// <summary>
			/// Default constructor, creates a no operation function.
			/// </summary>
			SetUniform()
			{}

			/// <summary>
			/// Sets the uniform to the provided value.
			/// </summary>
			/// <param name="value">The value.</param>
			void setUniform(const Type & value)
			{
				if(m_function) m_function(value);
			}
		};
	}
}