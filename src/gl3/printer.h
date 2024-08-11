#pragma once

#include <glm/matrix.hpp>
#include <iostream>

std::ostream & operator << (std::ostream & out, const glm::vec2 & v)
{
	std::cout << "(" << v.x << ", " << v.y << ")";
	return out;
}

std::ostream & operator << (std::ostream & out, const glm::vec3 & v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}

std::ostream & operator << (std::ostream & out, const glm::vec4 & v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return out;
}

std::ostream & operator<<(std::ostream & out, glm::mat2 const & mat)
{
	std::cout << "[" << mat[0] << ", " << mat[1] << "]";
	return out;
}

std::ostream & operator<<(std::ostream & out, glm::mat3 const & mat)
{
	std::cout << "[" << mat[0] << ", " << mat[1] << ", " << mat[2] << "]";
	return out;
}

std::ostream & operator<<(std::ostream & out, glm::mat4 const & mat)
{
	std::cout << "[" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "]";
	return out;
}