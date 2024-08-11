#pragma once

#include <glm/glm.hpp>

namespace gl3
{
	
	template <class T>
	struct TypeTraits
	{
		using primitiveType = void;

		static constexpr bool isCompatible() { return false; } 

		static constexpr GLenum glType() { return GL_NONE; }
		static constexpr size_t size() { return 0; }
		static constexpr size_t rows() { return 0; }
		static constexpr size_t columns() { return 0; }
		static constexpr bool isScalar() { return false; }
		static constexpr bool isVector() { return false; }
		static constexpr bool isMatrix() { return false; }
	};
	
	template <>
	struct TypeTraits<GLfloat>
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 1 ; }
		static constexpr size_t rows() { return 1; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return true ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return false; }
	};
	
	template <>
	struct TypeTraits<glm::vec2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 2 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
	};

	template <>
	struct TypeTraits<glm::vec3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 3 ; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
	};
	
	template <>
	struct TypeTraits<glm::vec4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 4 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
	};
	
	template <>
	struct TypeTraits<glm::mat2x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 4 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
	
	template <>
	struct TypeTraits<glm::mat2x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 6 ; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
	
	template <>
	struct TypeTraits<glm::mat2x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 8 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
	
	template <>
	struct TypeTraits<glm::mat3x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 6 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
	
	template <>
	struct TypeTraits<glm::mat3x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 9; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};

	template <>
	struct TypeTraits<glm::mat3x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 12 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};

	template <>
	struct TypeTraits<glm::mat4x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 8 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};

	template <>
	struct TypeTraits<glm::mat4x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 12; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
	
	template <>
	struct TypeTraits<glm::mat4x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GLenum glType() { return GL_FLOAT; }
		static constexpr size_t size() { return 16; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
	};
}