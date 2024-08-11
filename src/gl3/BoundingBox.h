#pragma once

#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <limits>
#include <vector>

namespace gl3
{
	/// <summary>
	/// An axis aligned bounding box
	/// </summary>
	class BoundingBox
	{
		glm::vec3 m_bounds[2];

	public:
		/// <summary>
		/// Default constructor, initializes an empty bounding box
		/// </summary>
		BoundingBox()
		{
			m_bounds[0] = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			m_bounds[1] = glm::vec3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		}

		/// <summary>
		/// Initializes a bounding box with the provided bounds.
		/// </summary>
		/// <param name="init">The initialization bounds (index 0, the minimum values, index 1, the maximum values).</param>
		BoundingBox(const glm::vec3 init[2])
		{
			m_bounds[0] = init[0];
			m_bounds[1] = init[1];
		}

		/// <summary>
		/// Initializes a bounding box with the provided bounds.
		/// </summary>
		/// <param name="min">The minimum values.</param>
		/// <param name="max">The maximum values.</param>
		BoundingBox(const glm::vec3 & min, const glm::vec3 & max)
		{
			m_bounds[0] = min;
			m_bounds[1] = max;
		}

		/// <summary>
		/// Creates a bounding box containing all vector contained by the provided data structure.
		/// </summary>
		/// <param name="dataStructure">The data structure (must have begin() and end() providing iterators).</param>
		template <typename iterable>
		BoundingBox(const iterable & dataStructure)
			: BoundingBox()
		{
			update(dataStructure);
		}

		/// <summary>
		/// Creates a bounding box containing all vector in the range [begin;end[.
		/// </summary>
		/// <param name="begin">The begin iterator.</param>
		/// <param name="end">The end iterator.</param>
		template <typename iterator>
		BoundingBox(const iterator & begin, const iterator & end)
		{
			update(begin, end);
		}

		/// <summary>
		/// Determines whether this bounding box is empty.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this bounding box is empty; otherwise, <c>false</c>.
		/// </returns>
		bool isEmpty() const
		{
			return !glm::all(glm::lessThan(m_bounds[0], m_bounds[1]));
		}

		/// <summary>
		/// Returns the minimum vertex.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & min() const { return m_bounds[0]; }

		/// <summary>
		/// Returns the maximum vertex.
		/// </summary>
		/// <returns></returns>
		const glm::vec3 & max() const { return m_bounds[1]; }

		/// <summary>
		/// Returns the center of the bounding box.
		/// </summary>
		/// <returns></returns>
		glm::vec3 center() const
		{
			assert(!isEmpty() && "Calling BoundingBox::center with an empty bounding box");
			return (m_bounds[0] + m_bounds[1])*0.5f;
		}

		/// <summary>
		/// returns the extent of the bounding box (size along x, y and z coordinates).
		/// </summary>
		/// <returns></returns>
		glm::vec3 extent() const
		{
			assert(!isEmpty() && "Calling BoundingBox::extent with an empty bounding box");
			return m_bounds[1] - m_bounds[0];
		}

		/// <summary>
		/// Updates the bounding box such as it contains v.
		/// </summary>
		/// <param name="v">The v.</param>
		void update(const glm::vec3 & v)
		{
			m_bounds[0] = glm::min(m_bounds[0], v);
			m_bounds[1] = glm::max(m_bounds[1], v);
		}

		/// <summary>
		/// Updates the bounding box such as it contains all vectors in the range [begin;end[.
		/// </summary>
		/// <param name="begin">The begin.</param>
		/// <param name="end">The end.</param>
		template <typename iterator>
		void update(const iterator & begin, const iterator & end)
		{
			for (auto it = begin; it != end; ++it)
			{
				update(*it);
			}
		}

		/// <summary>
		/// Updates the bounding box such as it contains all vectors of the provided data structure.
		/// </summary>
		/// <param name="dataStructure">The data structure (must have a begin() and a end() methods returning iterators.</param>
		template <typename iterable>
		void update(const iterable & dataStructure)
		{
			update(dataStructure.begin(), dataStructure.end());
		}

		/// <summary>
		/// Gets the vertices of the bounding box.
		/// </summary>
		/// <param name="write">The write iterator.</param>
		template <class iterator>
		void getVertices(iterator write) const
		{
			for (int x = 0; x <= 1; x++)
			{
				for (int y = 0; y <= 1; y++)
				{
					for (int z = 0; z <= 1; z++)
					{
						glm::vec3 maskMin(x,y,z);
						glm::vec3 maskMax = glm::vec3(1, 1, 1) - maskMin;
						(*write) = maskMin * m_bounds[0] + maskMax * m_bounds[1];
						++write;
					}
				}
			}
		}

		/// <summary>
		/// Gets the vertices of the bounding box.
		/// </summary>
		/// <returns></returns>
		std::vector<glm::vec3> getVertices() const
		{
			std::vector<glm::vec3> result;
			result.reserve(8);
			getVertices(std::back_inserter(result));
			return std::move(result);
		}

		/// <summary>
		/// Computes the union of two bounding boxes.
		/// </summary>
		/// <param name="other">The other bounding box.</param>
		/// <returns></returns>
		BoundingBox operator+ (const BoundingBox & other) const
		{
			return BoundingBox(glm::min(m_bounds[0], other.m_bounds[0]), glm::max(m_bounds[1], other.m_bounds[1]));
		}

		/// <summary>
		/// Computes the intersection of two bounding boxes
		/// </summary>
		/// <param name="other">The other bounding box.</param>
		/// <returns></returns>
		BoundingBox operator* (const BoundingBox & other) const
		{
			return BoundingBox(glm::max(m_bounds[0], other.m_bounds[0]), glm::min(m_bounds[1], other.m_bounds[1]));
		}
	};
}