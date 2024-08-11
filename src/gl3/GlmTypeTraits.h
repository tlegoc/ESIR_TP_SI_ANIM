#pragma once

#include <glm/glm.hpp>
#include <gl3/enums.h>

namespace gl3
{
	
	/// <summary>
	/// Type traits for glm math types
	/// </summary>
	template <class T>
	struct GlmTypeTraits
	{
		/// <summary>
		/// The primitive type of the elements contained in the GLM data structure
		/// </summary>
		using primitiveType = void;

		/// <summary>
		/// Determines whether the provided type is compatible with this type trais.
		/// </summary>
		/// <returns>
		///   <c>true</c> if the type T is compatible with this type traits; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isCompatible() { return false; }

		/// <summary>
		/// Returns the OpenGL type (enumerated value) corresponding to the <see cref="primitiveType"> primitiveType </see>
		/// </summary>
		/// <returns></returns>
		static constexpr GlType glType() { return GlType(GL_NONE); }

		/// <summary>
		/// Returns the number of elements in the data structure T.
		/// </summary>
		/// <returns></returns>
		static constexpr size_t size() { return 0; }

		/// <summary>
		/// Returns the number of rows in the data structure T.
		/// </summary>
		/// <returns></returns>
		static constexpr size_t rows() { return 0; }

		/// <summary>
		/// Returns the number of columns in the data structure T.
		/// </summary>
		/// <returns></returns>
		static constexpr size_t columns() { return 0; }

		/// <summary>
		/// Determines whether the type T is a scalar.
		/// </summary>
		/// <returns>
		///   <c>true</c> if T is a scalar; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isScalar() { return false; }

		/// <summary>
		/// Determines whether the type T is a vector.
		/// </summary>
		/// <returns>
		///   <c>true</c> if T is a vector; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isVector() { return false; }

		/// <summary>
		/// Determines whether the type T is a matrix.
		/// </summary>
		/// <returns>
		///   <c>true</c> if T is a matrix; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isMatrix() { return false; }

		/// <summary>
		/// Returns the texture internal format compatible with the type T.
		/// </summary>
		/// <returns></returns>
		static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat(GL_NONE); }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<GLfloat>
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 1 ; }
		static constexpr size_t rows() { return 1; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return true ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return false; }
		static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::r32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::vec2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 2 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
		static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rg32f ; }
	};

	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::vec3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 3 ; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
		static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::vec4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 4 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 1; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return true ; }
		static constexpr bool isMatrix() { return false; }
		static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgba32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat2x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 4 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rg32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat2x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 6 ; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rg32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat2x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 8 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 2; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rg32f; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat3x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 6 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat3x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 9; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};

	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat3x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 12 ; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 3; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};

	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat4x2> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 8 ; }
		static constexpr size_t rows() { return 2; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};

	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat4x3> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 12; }
		static constexpr size_t rows() { return 3; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};
	
	/// <summary>
	/// Specialization of <see cref="TypeTraits"> TypeTraits </see>.
	/// </summary>
	template <>
	struct GlmTypeTraits<glm::mat4x4> 
	{
		using primitiveType = GLfloat;
		
		static constexpr bool isCompatible() { return true ; }

		static constexpr GlType glType() { return GlType::Float; }
		static constexpr size_t size() { return 16; }
		static constexpr size_t rows() { return 4; }
		static constexpr size_t columns() { return 4; }
		static constexpr bool isScalar() { return false ; }
		static constexpr bool isVector() { return false ; }
		static constexpr bool isMatrix() { return true; }
		//static constexpr TextureInternalFormat glTextureFormat() { return TextureInternalFormat::rgb; }
	};
}