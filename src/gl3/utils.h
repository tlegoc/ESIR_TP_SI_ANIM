#pragma once

#include <vector>
#include <tuple>
#include <gl3/BoundingBox.h>

namespace gl3
{
	/// <summary>
	/// Packs the elements of the provided vectors into a tuple and returns a vector containing those tuples.
	/// </summary>
	/// <param name="first">The first vector.</param>
	/// <param name="...others">The other vectors.</param>
	/// <returns></returns>
	template <typename First, typename... Others>
	std::vector<std::tuple<First, Others...>> pack(const std::vector<First> & first, const std::vector<Others> &... others)
	{
		std::vector<std::tuple<First, Others...>> result;
		for (size_t cpt = 0; cpt < first.size(); ++cpt)
		{
			result.push_back(std::make_tuple(first[cpt], others[cpt]...));
		}
		return std::move(result);
	}

	/// <summary>
	/// Applies a transformation to a table of vertices. Please note that the perspective division is applied.
	/// </summary>
	/// <param name="transform">The transform to apply.</param>
	/// <param name="toTransform">The table of vectors that should be transformed.</param>
	void applyTransform(const glm::mat4 & transform, std::vector<glm::vec3> & toTransform)
	{
		for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
		{
			glm::vec4 tmp = transform * glm::vec4(*it, 1.0f);
			float invW = 1.0f / tmp.w;
			(*it) = glm::vec3(tmp)*glm::vec3(invW, invW, invW);
		}
	}
}

/// <summary>
/// Computes the transformation of a table of vertices. Please note that the perspective division is applied.
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat4 & transform, const std::vector<glm::vec3> & toTransform)
{
	std::vector<glm::vec3> result;
	result.reserve(toTransform.size());
	for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
	{
		glm::vec4 tmp = transform * glm::vec4(*it, 1.0);
		float invW = 1.0f / tmp.w;
		result.push_back(glm::vec3(tmp)*glm::vec3(invW, invW, invW));
	}
	return std::move(result);
}

/// <summary>
/// Computes the transformation of a table of vertices. Please note that the perspective division is applied.
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat4 & transform, std::vector<glm::vec3> && toTransform)
{
	gl3::applyTransform(transform, toTransform);
	return std::move(toTransform);
}

namespace gl3
{
	/// <summary>
	/// Applies a transformation to a table of vertices. 
	/// </summary>
	/// <param name="transform">The transform to apply.</param>
	/// <param name="toTransform">The table of vectors that should be transformed.</param>
	void applyTransform(const glm::mat4x3 & transform, std::vector<glm::vec3> & toTransform)
	{
		for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
		{
			(*it) = transform * glm::vec4(*it, 1.0f);
		}
	}
}

/// <summary>
/// Computes the transformation of a table of vertices. 
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat4x3 & transform, const std::vector<glm::vec3> & toTransform)
{
	std::vector<glm::vec3> result;
	result.reserve(toTransform.size());
	for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
	{
		result.push_back(transform * glm::vec4(*it, 1.0));
	}
	return std::move(result);
}

/// <summary>
/// Computes the transformation of a table of vertices. Please note that the perspective division is applied.
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat4x3 & transform, std::vector<glm::vec3> && toTransform)
{
	gl3::applyTransform(transform, toTransform);
	return std::move(toTransform);
}

namespace gl3
{
	/// <summary>
	/// Applies a transformation to a table of vertices. 
	/// </summary>
	/// <param name="transform">The transform to apply.</param>
	/// <param name="toTransform">The table of vectors that should be transformed.</param>
	void applyTransform(const glm::mat3 & transform, std::vector<glm::vec3> & toTransform)
	{
		for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
		{
			(*it) = transform * (*it);
		}
	}
}

/// <summary>
/// Computes the transformation of a table of vertices. 
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat3 & transform, const std::vector<glm::vec3> & toTransform)
{
	std::vector<glm::vec3> result;
	result.reserve(toTransform.size());
	for (auto it = toTransform.begin(), end = toTransform.end(); it != end; ++it)
	{
		result.push_back(transform * (*it));
	}
	return std::move(result);
}

/// <summary>
/// Computes the transformation of a table of vertices. Please note that the perspective division is applied.
/// </summary>
/// <param name="transform">The transform to apply.</param>
/// <param name="toTransform">The vertices to transform.</param>
/// <returns></returns>
std::vector<glm::vec3> operator* (const glm::mat3 & transform, std::vector<glm::vec3> && toTransform)
{
	gl3::applyTransform(transform, toTransform);
	return std::move(toTransform);
}

/// <summary>
///Computes the bounding box of the transformed box.
/// </summary>
/// <param name="transform">The transform.</param>
/// <param name="box">The box to transform.</param>
/// <returns></returns>
gl3::BoundingBox operator* (const glm::mat4 & transform, const gl3::BoundingBox & box)
{
	return gl3::BoundingBox(transform*box.getVertices());
}

/// <summary>
///Computes the bounding box of the transformed box.
/// </summary>
/// <param name="transform">The transform.</param>
/// <param name="box">The box to transform.</param>
/// <returns></returns>
gl3::BoundingBox operator* (const glm::mat4x3 & transform, const gl3::BoundingBox & box)
{
	return gl3::BoundingBox(transform * box.getVertices());
}
