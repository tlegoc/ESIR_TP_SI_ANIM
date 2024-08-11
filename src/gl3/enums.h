#pragma once

#include <GL/glew.h>

namespace gl3
{
	/// <summary>
	/// Enumerated type for primitives used for rendering (glDrawXXX)
	/// </summary>
	enum class RenderPrimitive : GLenum {
		points = GL_POINTS,
		lines = GL_LINES,
		triangles = GL_TRIANGLES,
		lineStrip = GL_LINE_STRIP,
		triangleStrip = GL_TRIANGLE_STRIP,
		triangleFan = GL_TRIANGLE_FAN
	};

	/// <summary>
	/// The update frequency of usages of the buffer
	/// </summary>
	enum class BufferUpdate : GLenum {
		/*! <summary> The vertex buffer is never updated </summary> */
		staticDraw = GL_STATIC_DRAW,
		/*! <summary> The vertex buffer is not frequently updated </summary> */
		dynamicDraw = GL_DYNAMIC_DRAW,
		/*! <summary> The vertex buffer is updated at every frame </summary> */
		streamDraw = GL_STREAM_DRAW
	} ;

	/*! The type of a buffer attached to a frame buffer object, either a 2D texture or a render buffer. */
	enum class FboAttachmentType {
		/*! <summary> The attached buffer is a 2D texture </summary> */
		texture, 
		/*! <summary> The attached buffer is a render buffer </summary> */
		renderBuffer 
	} ;

	/// <summary>
	/// The different attachments to a frame buffer object
	/// </summary>
	enum class FboAttachement : GLenum {
		colorAttachment0 = GL_COLOR_ATTACHMENT0,
		colorAttachment1 = GL_COLOR_ATTACHMENT1,
		colorAttachment2 = GL_COLOR_ATTACHMENT2,
		colorAttachment3 = GL_COLOR_ATTACHMENT3,
		colorAttachment4 = GL_COLOR_ATTACHMENT4,
		colorAttachment5 = GL_COLOR_ATTACHMENT5,
		colorAttachment6 = GL_COLOR_ATTACHMENT6,
		colorAttachment7 = GL_COLOR_ATTACHMENT7,
		depthAttachment = GL_DEPTH_ATTACHMENT,
		stencilAttachment = GL_STENCIL_ATTACHMENT,
		depthStencilAttachment = GL_DEPTH_STENCIL_ATTACHMENT
	} ;

	/// <summary>
	/// Type of a shader
	/// </summary>
	enum class ShaderType : GLenum { 
		/*! <summary> A vertex shader </summary> */
		vertexShader = GL_VERTEX_SHADER, 
		/*! <summary> A fragment shader </summary> */
		fragmentShader = GL_FRAGMENT_SHADER,
		/*! <summary> A geometry shader </summary> */
		geometryShader = GL_GEOMETRY_SHADER
	} ;

	/// <summary>
	/// The texture wrapping target
	/// </summary>
	enum class TextureWrappingTarget : GLenum {
		/*! <summary> The s texture coordinate </summary> */
		wrapS = GL_TEXTURE_WRAP_S, 
		/*! <summary> The t texture coordinate </summary> */
		wrapT = GL_TEXTURE_WRAP_T,
		/*! <summary> The r texture coordinate </summary> */
		wrapR = GL_TEXTURE_WRAP_R
	} ;

	/// <summary>
	/// The texture wrapping modes
	/// </summary>
	enum class TextureWrappingMode : GLenum {
		/*! <summary> Repeats the texture </summary> */
		repeat = GL_REPEAT, 
		/*! <summary> Repeats the texture but mirrored </summary> */
		mirroredRepeat = GL_MIRRORED_REPEAT,
		/*! <summary> Clamps the texture on the edge </summary> */
		clampToEdge = GL_CLAMP_TO_EDGE, 
		/*! <summary> Clamps the texture on the edge and gives the border color </summary> */
		clampToBorder = GL_CLAMP_TO_BORDER
	} ;

	/// <summary>
	/// The texture filter mode targets
	/// </summary>
	enum class TextureFilterTarget : GLenum {
		/*! <summary> Texture minifying </summary> */
		minFilter = GL_TEXTURE_MIN_FILTER,
		/*! <summary> Texture magnifying </summary> */
		magFilter = GL_TEXTURE_MAG_FILTER 
	} ;

	/// <summary>
	/// The filter mode
	/// </summary>
	enum class TextureFilterMode : GLenum {
		/*! <summary> Nearest pixel </summary> */
		nearest = GL_NEAREST,
		/*! <summary> Linear pixel interpolation </summary> */
		linear = GL_LINEAR, 
		/*! <summary> Nearest pixel and nearest mipmap interpolation </summary> */
		nearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		/*! <summary> Linear pixel and nearest mipmap interpolation </summary> */
		linearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		/*! <summary> Nearest pixel and linear mipmap interpolation </summary> */
		nearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		/*! <summary> Linear pixel and linear mipmap interpolation </summary> */
		linearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
	} ;

	/// <summary>
	/// Enumeration of the faces of a cube map.
	/// </summary>
	enum class CubeMapPosition : GLenum
	{
		/*! <summary> Face in the X positive direction </summary> */
		xPositive = GL_TEXTURE_CUBE_MAP_POSITIVE_X, right = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		/*! <summary> Face in the X negative direction </summary> */
		xNegative = GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		/*! <summary> Face in the Y positive direction </summary> */
		yPositive = GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		/*! <summary> Face in the Y negative direction </summary> */
		yNegative = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		/*! <summary> Face in the Z positive direction </summary> */
		zPositive = GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		/*! <summary> Face in the Z negative direction </summary> */
		zNegative = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	} ;

	/// <summary>
	/// Enumerated type mapping the OpenGL scalar type enumeration values.
	/// Please, consult <see cref="GlTypeTraits"> GlTypeTraits </see> and <see cref="ToGlTypeTraits"> ToGlTypeTraits </see> for the traits 
	/// associated with this type.
	/// </summary>
	enum class GlType : GLenum {
		Float = GL_FLOAT,
		Double = GL_DOUBLE,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT
	};


	/// <summary>
	/// Traits associated with the <see cref="GlType"> GlType </see> enum.
	/// </summary>
	template <GlType T>
	struct GlTypeTraits
	{
		/// <summary>
		/// Determines whether this enumerated value is valid for traits.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isValid() { return false; }

		/// <summary>
		/// The equivalent C++ type.
		/// </summary>
		using CppType = void;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::Float>
	{
		static constexpr bool isValid() { return true; }
		using CppType = float;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::Double>
	{
		static constexpr bool isValid() { return true; }
		using CppType = double;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::Int>
	{
		static constexpr bool isValid() { return true; }
		using CppType = int;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::UnsignedInt>
	{
		static constexpr bool isValid() { return true; }
		using CppType = unsigned int;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::Byte>
	{
		static constexpr bool isValid() { return true; }
		using CppType = char;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::UnsignedByte>
	{
		static constexpr bool isValid() { return true; }
		using CppType = unsigned char;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::Short>
	{
		static constexpr bool isValid() { return true; }
		using CppType = short;
	};

	/// <summary>
	/// Specialization of <see cref="GlTypeTraits"> GlTypeTraits </see>.
	/// </summary>
	template <>
	struct GlTypeTraits<GlType::UnsignedShort>
	{
		static constexpr bool isValid() { return true; }
		using CppType = unsigned short;
	};

	/// <summary>
	/// Traits associated with the <see cref="GlType"> GlType </see> enum, used get the C++ type as an enumerated value of an OpenGL type.
	/// </summary>
	template <typename T>
	struct ToGlTypeTraits
	{
		/// <summary>
		/// Determines whether the provided type is compatible with the GlType enumeration.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isValid() { return false; }

		/// <summary>
		/// Returns the GlType equivalent to the provided C++ type.
		/// </summary>
		/// <returns></returns>
		static constexpr GlType glType() { return GlType(GL_NONE); }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<float>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<double>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::Double; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<char>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::Byte; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<unsigned char>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::UnsignedByte; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<short>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::Short; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<unsigned short>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::UnsignedShort; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<int>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::Int; }
	};

	/// <summary>
	/// Specialization of <see cref="ToGlTypeTraits"> ToGlTypeTraits </see>.
	/// </summary>
	template <>
	struct ToGlTypeTraits<unsigned int>
	{
		static constexpr bool isValid() { return true; }
		static constexpr GlType glType() { return GlType::UnsignedInt; }
	};

	/// <summary>
	/// The internal texture formats. See <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see> for 
	/// manipulating the traits.
	/// </summary>
	enum class TextureInternalFormat : GLenum {
		depth = GL_DEPTH_COMPONENT,
		depth16 = GL_DEPTH_COMPONENT16,
		depth32 = GL_DEPTH_COMPONENT32,
		depthStencil = GL_DEPTH_STENCIL,

		red = GL_RED,
		rg = GL_RG,
		rgb = GL_RGB, 
		rgba = GL_RGBA,

		r16f = GL_R16F,
		rg16f = GL_RG16F,
		rgb16f = GL_RGB16F,
		rgba16f = GL_RGBA16F,

		r32f = GL_R32F,
		rg32f = GL_RG32F, 
		rgb32f = GL_RGB32F, 
		rgba32f = GL_RGBA32F,

		r32ui = GL_R32UI,
		rg32ui = GL_RG32UI,
		rgb32ui = GL_RGB32UI,
		rgba32ui = GL_RGBA32UI
	} ;

	std::pair<GLenum, GLenum> textureInternalFormatBaseType(TextureInternalFormat format)
	{
		if (format == TextureInternalFormat::depth || format == TextureInternalFormat::depth16 || format == TextureInternalFormat::depth32) { return { GL_DEPTH_COMPONENT, GL_FLOAT }; }
		if (format == TextureInternalFormat::depthStencil) { return { GL_DEPTH_STENCIL, GL_FLOAT }; }
		if (format == TextureInternalFormat::red || format == TextureInternalFormat::r32f || format == TextureInternalFormat::r16f || format==TextureInternalFormat::r32ui) { return { GL_RED, GL_UNSIGNED_BYTE }; }
		if (format == TextureInternalFormat::rg || format == TextureInternalFormat::rg32f || format == TextureInternalFormat::rg16f || format==TextureInternalFormat::rg32ui) { return { GL_RG, GL_UNSIGNED_BYTE }; }
		if (format == TextureInternalFormat::rgb || format == TextureInternalFormat::rgb32f || format == TextureInternalFormat::rgb16f || format==TextureInternalFormat::rgb32ui) { return { GL_RGB, GL_UNSIGNED_BYTE }; }
		if (format == TextureInternalFormat::rgba || format == TextureInternalFormat::rgba16f || format == TextureInternalFormat::rgba32f || format==TextureInternalFormat::rgba32ui) { return { GL_RGBA, GL_UNSIGNED_BYTE }; }
		assert(false && "textureInternalFormatBaseType called with an invalid parameter");
		throw std::runtime_error("textureInternalFormatBaseType: enum value not found!!!!");
	}

	/// <summary>
	/// Traits associated with the TextureInternalFormatType
	/// </summary>
	template <TextureInternalFormat V>
	struct TextureInternalFormatTraits
	{
		/// <summary>
		/// Determines whether the provided enumerated value is valid for type traits.
		/// </summary>
		/// <returns>
		///   <c>true</c> if this enumerated value is valid; otherwise, <c>false</c>.
		/// </returns>
		static constexpr bool isValid() { return false; }

		/// <summary>
		/// Returns the base internal format associated with the provided format (useful for glTexImage2D for instance)
		/// </summary>
		/// <returns></returns>
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat(GL_NONE); }

		/// <summary>
		/// Returns the base element type associated with the base internal format (useful for glTexImage2D for instance)
		/// </summary>
		/// <returns></returns>
		static constexpr GlType baseElementType() { return GlType(GL_NONE); }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::depth>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::depth; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template<>
	struct TextureInternalFormatTraits<TextureInternalFormat::depth16> : public TextureInternalFormatTraits<TextureInternalFormat::depth>
	{};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template<>
	struct TextureInternalFormatTraits<TextureInternalFormat::depth32> : public TextureInternalFormatTraits<TextureInternalFormat::depth>
	{};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template<>
	struct TextureInternalFormatTraits<TextureInternalFormat::depthStencil>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::depthStencil; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::red>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::red; }
		static constexpr GlType baseElementType() { return GlType::UnsignedByte; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rg>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rg; }
		static constexpr GlType baseElementType() { return GlType::UnsignedByte; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgb>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgb; }
		static constexpr GlType baseElementType() { return GlType::UnsignedByte; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgba>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgba; }
		static constexpr GlType baseElementType() { return GlType::UnsignedByte; }
	};

	// Float 16
	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::r16f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::red; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rg16f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rg; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgb16f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgb; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgba16f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgba; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	// Float 32
	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::r32f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::red; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rg32f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rg; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgb32f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgb; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgba32f>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgba; }
		static constexpr GlType baseElementType() { return GlType::Float; }
	};

	// Unsigned int 32
	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::r32ui>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::red; }
		static constexpr GlType baseElementType() { return GlType::UnsignedInt; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rg32ui>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rg; }
		static constexpr GlType baseElementType() { return GlType::UnsignedInt; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgb32ui>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgb; }
		static constexpr GlType baseElementType() { return GlType::UnsignedInt; }
	};

	/// <summary>
	/// Specialization of <see cref="TextureInternalFormatTraits"> TextureInternalFormatTraits </see>.
	/// </summary>
	template <>
	struct TextureInternalFormatTraits<TextureInternalFormat::rgba32ui>
	{
		static constexpr bool isValid() { return true; }
		static constexpr TextureInternalFormat baseInternalFormat() { return TextureInternalFormat::rgba; }
		static constexpr GlType baseElementType() { return GlType::UnsignedInt; }
	};

	/// <summary>
	/// When face culling is enable, this mode describes which type of face should be culled.
	/// </summary>
	enum class CullingMode : GLenum
	{
		front = GL_FRONT,
		back = GL_BACK,
		frontAndBack = GL_FRONT_AND_BACK
	};

	/// <summary>
	/// Enumeration of the possible blending weights associated with the blending function. 
	/// See </see cref="BlendingEquation"> BlendingEquation </see> for the list of the possible blending equations.
	/// </summary>
	enum class BlendingWeight : GLenum
	{
		zero=GL_ZERO, 
		one=GL_ONE, 
		scrColor=GL_SRC_COLOR, 
		oneMinusScrColor=GL_ONE_MINUS_SRC_COLOR, 
		dstColor=GL_DST_COLOR, 
		oneMinusDstColor=GL_ONE_MINUS_DST_COLOR, 
		srcAlpha=GL_SRC_ALPHA, 
		oneMinusSrcAlpha=GL_ONE_MINUS_SRC_ALPHA, 
		dstAlpha=GL_DST_ALPHA, 
		oneMinusDstAlpha=GL_ONE_MINUS_DST_ALPHA,
		constantColor=GL_CONSTANT_COLOR, 
		oneMinusConstantColor=GL_ONE_MINUS_CONSTANT_COLOR, 
		constantAlpha=GL_CONSTANT_ALPHA, 
		oneMinusConstantAplha=GL_ONE_MINUS_CONSTANT_ALPHA
	};

	/// <summary>
	/// Enumeration of the possible blending equations. See </see cref="BlendingWeight"> BlendingWeight </see> for 
	/// the weights assocaited with this equation.
	/// </summary>
	enum class BlendingEquation : GLenum
	{		
		/// <summary> result = sourceWeight * source + destinationWeight * destination </summary>
		add = GL_FUNC_ADD,
		/// <summary> result = sourceWeight * source - destinationWeight * destination </summary>
		sub=GL_FUNC_SUBTRACT, 
		/// <summary> result = destinationWeight * destination + sourceWeight * source </summary>
		reverseSub=GL_FUNC_REVERSE_SUBTRACT,
		/// <summary> result = min(source, destination) </summary>
		min=GL_MIN, 
		/// <summary> result = max(source, destination) </summary>
		max=GL_MAX
	};

	/// <summary>
	/// Enumeration of the possible stencil operations.
	/// </summary>
	enum class StencilOperation : GLenum
	{
		/// <summary> Keeps the value in the stencil buffer </summary>
		keep=GL_KEEP, 
		/// <summary> The value in the stencil buffer is set to zero </summary>
		zero=GL_ZERO, 
		/// <summary> The value in the stencil buffer is replaced (set to the reference value, see <see cref="GlTestFunction"> GlTestFunction </see>)</summary>.
		replace=GL_REPLACE, 
		/// <summary> Increments the value in the stencil buffer (value clamped to the maximum value)</summary>
		increment=GL_INCR, 
		/// <summary> Increments the value in the stencil buffer (if the value is greater than the maximum value, it is set to zero)</summary>
		incrementWrap=GL_INCR_WRAP, 
		/// <summary> Decrements the value in the stencil buffer (value clamped to the minimum value)</summary>
		decrement=GL_DECR,
		/// <summary> Decrements the value in the stencil buffer (if the value is lesser than zero, it is set to the maximum value)</summary>
		decrementWrap=GL_DECR_WRAP,
		/// <summary> Bitwise invert to the value contained in the stencil buffer </summary>
		invert=GL_INVERT
	};

	/// <summary>
	/// Enumeration of the test functions used in OpenGL. Those functions compare a value to a reference (either manually set or read in a buffer)
	/// </summary>
	enum class GlTestFunction : GLenum
	{
		/// <summary> Never pass </summary>
		never=GL_NEVER, 
		/// <summary> Always pass </summary>
		always = GL_ALWAYS,
		/// <summary> Pass if value &lt; reference </summary>
		less=GL_LESS, 
		/// <summary> Pass if value &lt;= reference </summary>
		lessEqual=GL_LEQUAL,
		/// <summary> Pass if value &gt; reference </summary>
		greater=GL_GREATER, 
		/// <summary> Pass if value &gt;= reference </summary>
		greaterEqual=GL_GEQUAL, 
		/// <summary> Pass if value == reference </summary>
		equal=GL_EQUAL,
		/// <summary> Pass if value != reference </summary>
		notEqual=GL_NOTEQUAL
	};
}