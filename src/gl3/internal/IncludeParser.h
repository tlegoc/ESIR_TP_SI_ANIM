#pragma once

#include <vector>
#include <filesystem>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

namespace gl3
{
	namespace internal
	{
		/// <summary>
		/// Parser loading a file containing #include statements. 
		/// This parser only support one #include directive per line and it has to be the only instruction on the line 
		/// (comments are not accepted).
		/// This parser also adds #line directives for better error handling during shader compilation.
		/// </summary>
		class IncludeParser
		{
			std::vector<::std::filesystem::path> m_includePaths;

			static std::string trim(const std::string & str)
			{
				int beginIndex = 0;
				int endIndex = str.size() - 1;
				while (beginIndex<str.size() && std::isspace(str[beginIndex])) { ++beginIndex; }
				while (endIndex>0 && std::isspace(str[endIndex])) { --endIndex; }
				if (endIndex < beginIndex) { return ""; }
				return str.substr(beginIndex, endIndex - beginIndex + 1);
			}

			static bool extractInclude(const std::string & str, std::filesystem::path & result)
			{
				std::string tmp = trim(str);
				std::string search = "#include";
				if(search == tmp.substr(0, search.size()))
				{
					tmp = trim(tmp.substr(search.size(), tmp.size() - search.size()));
					result = tmp.substr(1, tmp.size()-2);
					return true;
				}
				return false;
			}

			template <typename iterator>
			void loadFile(const std::filesystem::path & file, iterator output)
			{
				std::cout << "IncludeParser: processing file " << file.string() << std::endl;
				if (!std::filesystem::exists(file)) { throw std::ios_base::failure(std::string("File ") + file.string() + " not found."); }
				std::ifstream input(file.string());
				size_t lineCounter = 1;
				while (!input.eof())
				{
					std::string line;
					std::getline(input, line);
					std::filesystem::path includePath;
					if (extractInclude(line, includePath))
					{
						loadFile(findFile(includePath), output);
					}
					else
					{
						//(*output) = "#line " + std::to_string(lineCounter) + " \"" + file.string() + "\"" + "\n";
						//++output;
						(*output) = line + "\n";
						++output;
					}
					++lineCounter;
				}
			}

			std::filesystem::path findFile(const std::filesystem::path & file)
			{
				if (std::filesystem::exists(file)) { return file; }
				for (auto it = m_includePaths.begin(), end = m_includePaths.end(); it != end; ++it)
				{
					std::filesystem::path current = (*it) / file;
					if (std::filesystem::exists(current)) { return current; }
				}
				throw std::ios_base::failure(std::string("IncludeParser: include file ") + file.string() + " not found.");
			}

			template <typename iterator>
			std::string createString(iterator begin, iterator end)
			{
				std::stringstream result;
				for (iterator it=begin; it!= end; ++it)
				{
					result << (*it);
				}
				return result.str();
			}

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="IncludeParser"/> class.
			/// </summary>
			IncludeParser() {}

			/// <summary>
			/// Adds and include path to the parser.
			/// </summary>
			/// <param name="path">The path to add.</param>
			void addIncludePath(const std::filesystem::path & path)
			{
				m_includePaths.push_back(path);
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
			/// Loads the specified file and replace the includes by the designated files. 
			/// This loader also adds #line directives for better error handling during shader compilation.
			/// </summary>
			/// <param name="file">The file.</param>
			/// <returns></returns>
			std::string load(const std::filesystem::path & file)
			{
				std::filesystem::path newPath = file;
				newPath.remove_filename();
				m_includePaths.push_back(newPath);
				std::list<std::string> result;

				try
				{
					loadFile(file, std::back_inserter(result));
				}
				catch (const std::exception & except)
				{
					m_includePaths.pop_back(); 
					throw except;
				}

				m_includePaths.pop_back();
				std::stringstream output;
				for (auto it = result.begin(), end = result.end(); it != end; ++it)
				{
					output << *it;
				}
				return output.str();
			}
		};
	}
}