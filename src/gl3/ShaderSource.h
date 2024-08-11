#pragma once

#include <gl3/internal/IncludeParser.h>

namespace gl3
{
	/// <summary>
	/// Source code for a shader. This class enables the use of #include directives in the source of the shaders.
	/// Please note that in the source code, the #include directive must be followed by the filename and must be on the same line.
	/// Comments at the end of the line are not allowed.
	/// </summary>
	class ShaderSource
	{
		internal::IncludeParser m_includeParser;
		std::stringstream m_source;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderSource"/> class.
		/// </summary>
		ShaderSource()
		{}

		/// <summary>
		/// Initializes a new instance of the <see cref="ShaderSource"/> class contaning the source code located in the provided file.
		/// </summary>
		/// <param name="filename">The filename.</param>
		ShaderSource(const std::filesystem::path & filename)
		{
			append(filename);
		}

		/// <summary>
		/// Adds an include path.
		/// </summary>
		/// <param name="path">The path.</param>
		void addIncludePath(const std::filesystem::path & path)
		{
			m_includeParser.addIncludePath(path);
		}

		/// <summary>
		/// Adds the include paths in the range [begin;end[.
		/// </summary>
		/// <param name="begin">The begin iterator.</param>
		/// <param name="end">The end iterator.</param>
		template <typename iterator>
		void addIncludePath(iterator begin, iterator end)
		{
			for (auto it = begin; it != end; ++it)
			{
				addIncludePath(*it);
			}
		}

		/// <summary>
		/// Appends the specified file to the shader source.
		/// </summary>
		/// <param name="filename">The filename.</param>
		void append(const std::filesystem::path & filename)
		{
			m_source << m_includeParser.load(filename);
		}

		/// <summary>
		/// Converts the source to a string.
		/// </summary>
		/// <returns></returns>
		std::string toString() const
		{
			return m_source.str();
		}

		/// <summary>
		/// Adds any element compatible with output operator << to the shader source
		/// </summary>
		/// <returns></returns>
		template <typename type>
		ShaderSource & operator << (const type & data)
		{
			m_source << data;
		}

		/// <summary>
		/// Adds any element compatible with output operator << to the shader source
		/// </summary>
		/// <returns></returns>
		ShaderSource & operator<< (const ShaderSource & other)
		{
			m_source << other.toString();
		}
	};
}