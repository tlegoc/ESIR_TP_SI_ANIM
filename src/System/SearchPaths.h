#pragma once

#include <filesystem>
#include <exception>
#include <cassert>

namespace System
{
	/// <summary>
	/// Class helping to find a file based on a set of provided paths.
	/// </summary>
	class SearchPaths
	{
		::std::vector<std::filesystem::path> m_paths;

	public:
		using iterator = std::vector<std::filesystem::path>::iterator;
		using const_iterator = std::vector<std::filesystem::path>::const_iterator;
		
		/// <summary>
		/// Initializes a new instance of the <see cref="SearchPaths"/> class.
		/// </summary>
		SearchPaths()
		{}

		/// <summary>
		/// Adds a search path.
		/// </summary>
		/// <param name="path">The path.</param>
		void push(const std::filesystem::path & path)
		{
			m_paths.push_back(path);
		}

		/// <summary>
		/// Pushes the paths in the provided iterator range.
		/// </summary>
		/// <param name="begin">The begin iterator.</param>
		/// <param name="end">The end iterator.</param>
		template <typename iterator>
		void push(iterator begin, iterator end)
		{
			for (auto it = begin; it != end; ++it)
			{
				push(*it);
			}
		}

		/// <summary>
		/// Pops the last search path.
		/// </summary>
		void pop()
		{
			assert(!m_paths.empty());
			m_paths.pop_back();
		}

		/// <summary>
		/// Returns an iterator to the beginning of the paths.
		/// </summary>
		/// <returns></returns>
		iterator begin() { return m_paths.begin(); }

		
		/// <summary>
		/// Returns an iterator past the end of the paths.
		/// </summary>
		/// <returns></returns>
		iterator end() { return m_paths.end(); }

		/// <summary>
		/// Returns an iterator to the beginning of the paths.
		/// </summary>
		/// <returns></returns>
		const_iterator begin() const { return m_paths.begin(); }

		/// <summary>
		/// Returns an iterator past the end of the paths.
		/// </summary>
		/// <returns></returns>
		const_iterator end() const { return m_paths.end(); }

		/// <summary>
		/// Finds the path of the provided file. If the file exists in the current path, it is returned as it was provided.
		/// An std::io_base::failure exception is throw if the file is not found.
		/// </summary>
		/// <param name="file">The searched file.</param>
		/// <returns></returns>
		std::filesystem::path findFile(const std::filesystem::path & file) const
		{
			if (std::filesystem::exists(file)) { return file; }
			for (auto it = m_paths.begin(), end = m_paths.end(); it != end; ++it)
			{
				std::filesystem::path current = (*it);
				current /= file;
				if (std::filesystem::exists(current)) { return current; }
			}
			throw std::ios_base::failure("File: " + file.string() + " not found");
		}
	};
}