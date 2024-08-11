#pragma once

#include <GL/glew.h>
#include <iostream>
#include <gl3/enums.h>

namespace gl3
{
	/// <summary>
	/// Enables to parameterize the global state of OpenGL. This class is a singleton.
	/// </summary>
	class GlobalState
	{
		/// <summary>
		/// Data structure used to control the behavior during the emission of OpenGL messages
		/// </summary>
		struct DebugControl
		{
			/// <summary> Should we automatically break on error / warning ? </summary>
			bool useBreakpoint;

			DebugControl()
				: useBreakpoint(true)
			{}
		};

		DebugControl m_debugControl;

		/// <summary>
		/// Prevents a default instance of the <see cref="GlobalState"/> class from being created outside this scope.
		/// </summary>
		GlobalState() {}

	public:
		/// <summary>
		/// Gets the unique instance of GlobalControl.
		/// </summary>
		/// <returns></returns>
		static GlobalState * getSingleton()
		{
			static GlobalState globalSate;
			return &globalSate;
		}

		/// <summary>
		/// Enables or disables the back face culling. See <see cref="GlobalState::cullingMode"> cullingMode </see> to control 
		/// which faces should be culled.
		/// </summary>
		/// <param name="status">if set to <c>true</c> back face culling is enabled.</param>
		void enableCulling(bool status=true)
		{
			if (status)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}

		/// <summary>
		/// Sets the culling mode. See <see cref="GlobalState::enableCulling"> enableCulling </see> too control the activation of face culling.
		/// </summary>
		/// <param name="mode">The culling mode.</param>
		void cullingMode(CullingMode mode=CullingMode::back)
		{
			glCullFace(GLenum(mode));
		}

		/// <summary>
		/// Enables / disables the depth test. See <see cref="GlobalState::enableDepthWrite"> enableDepthWrite </see> to control 
		/// depth buffer modifications.
		/// </summary>
		/// <param name="status">if set to <c>true</c> the depth test is enabled.</param>
		void enableDepthTest(bool status=true)
		{
			if (status)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		/// <summary>
		/// Enables / disables the depth write. <see cref="GlobalState::enableDepthTest"> enableDepthTest </see> to control whether the depth test should
		/// be used.
		/// </summary>
		/// <param name="status">if set to <c>true</c> depth write is activated.</param>
		void enableDepthWrite(bool status=true)
		{
			glDepthMask((status) ? GL_TRUE : GL_FALSE);
		}

		/// <summary>
		/// Sets the depth test function. 
		/// </summary>
		/// <param name="function">The function.</param>
		void depthTestFunction(GlTestFunction function = GlTestFunction::less)
		{
			glDepthFunc(GLenum(function));
		}

		/// <summary>
		/// If enabled, the primitives are not rasterized; they are discarded after the optional 
		/// transform feedback stage.
		/// </summary>
		/// <param name="status">if set to <c>true</c> [status].</param>
		void enableRasterizerDiscard(bool status=true)
		{
			if (status)
			{
				glEnable(GL_RASTERIZER_DISCARD);
			}
			else
			{
				glDisable(GL_RASTERIZER_DISCARD);
			}
		}

		/// <summary>
		/// Enables or disables the scissor test. See <see cref="GlobalState::scissorWindow"> scissorWindow </see> 
		/// to parmeterize the scissor window.
		/// </summary>
		/// <param name="status">if set to <c>true</c> to enable the scissor test.</param>
		void enableScissorTest(bool status=true)
		{
			if (status)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}
		}

		/// <summary>
		/// Description of the scissor window. See See <see cref="GlobalState::enableScissorTest"> enableScissorTest </see> 
		/// To enable scissor test.
		/// </summary>
		/// <param name="x">The x starting coordinate.</param>
		/// <param name="y">The y starting coordinate.</param>
		/// <param name="width">The width of the window.</param>
		/// <param name="height">The height of the window.</param>
		void scissorWindow(GLint x, GLint y, GLsizei width, GLsizei height)
		{
			glScissor(x, y, width, height);
		}

		/// <summary>
		/// Globally enables the blending. See <see cref="GlobalState::blendingWeights"> blendingWeights </see>, 
		///  <see cref="GlobalState::blendingEquation"> blendingEquation </see>,
		///  <see cref="GlobalState::blendingConstantColor"> blendingConstantColor </see> to parameterize the blending equation.
		/// </summary>
		/// <param name="status">if set to <c>true</c> [status].</param>
		void enableBlending(bool status=true)
		{
			if (status)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}

		/// <summary>
		/// Setup of the blending function : written = blendingFunction(source*RGBA_source, destination*RGBA_destination) 
		/// except of min / max functions.
		/// </summary>
		/// <param name="source">The source.</param>
		/// <param name="destination">The destination.</param>
		void blendingWeights(BlendingWeight sourceWeight=BlendingWeight::one, BlendingWeight destinationWeight=BlendingWeight::zero)
		{
			glBlendFunc(GLenum(sourceWeight), GLenum(destinationWeight));
		}

		/// <summary>
		/// Set the blending equation. 
		/// </summary>
		/// <param name="equation">The equation.</param>
		void blendingEquation(BlendingEquation equation = BlendingEquation::add)
		{
			glBlendEquation(GLenum(equation));
		}

		/// <summary>
		/// Sets the constant blending color .
		/// </summary>
		/// <param name="color">The color.</param>
		void blendingConstantColor(const glm::vec4 & color)
		{
			glBlendColor(color.r, color.g, color.b, color.a);
		}

		/// <summary>
		/// Enables / disables the stencil test. If enabled, the stencil tests and stencil buffers updates can be used.
		/// See <see cref="GlobalState::stencilOperation"> stencilOperation </see> and <see cref="GlobalState::stencilFunction"> stencilFunction </see> to
		/// configure the modification and test functions applied to the stencil buffer.
		/// </summary>
		/// <param name="status">if set to <c>true</c> to enables stencil operations.</param>
		void enableStencilTest(bool status=true)
		{
			if (status)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}
		}

		/// <summary>
		/// Describes the stencil operations.
		/// </summary>
		/// <param name="stencilFailed">Operation when the stencil test failed.</param>
		/// <param name="stencilPassedDepthFailed">Operation when the stencil test passed but the depth test failed.</param>
		/// <param name="stencilPassedDepthPassed">Operation when the stencil test passed and the depth test passed.</param>
		void stencilOperation(StencilOperation stencilFailed=StencilOperation::keep, StencilOperation stencilPassedDepthFailed=StencilOperation::keep, StencilOperation stencilPassedDepthPassed=StencilOperation::keep)
		{
			glStencilOp(GLenum(stencilFailed), GLenum(stencilPassedDepthFailed), GLenum(stencilPassedDepthPassed));
		}

		/// <summary>
		/// Describes the stencil test function.
		/// result = function(tested &amp; mask, reference &amp; mask)
		/// </summary>
		/// <param name="function">The function used for the test.</param>
		/// <param name="referenceValue">The reference value used for the test.</param>
		/// <param name="mask">The mask applied to the reference value and the tested value..</param>
		void stencilFunction(GlTestFunction function, GLint referenceValue, GLuint mask=0xFFFFFFFF)
		{
			glStencilFunc(GLenum(function), referenceValue, mask);
		}

		/// <summary>
		/// Enables / disables the debug mode. Warning this option uses OpenGL 4.3 commands...
		/// </summary>
		/// <param name="status">if set to <c>true</c> debug mode is enabled.</param>
		void enableDebugMode(bool status, bool breakOnMessage = true)
		{
			m_debugControl.useBreakpoint = breakOnMessage;
			if (!glDebugMessageCallback)
			{
				std::cerr << "Debug mode is only available with OpenGL 4.3 capable devices." << std::endl;
				return;
			}
			if (status)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutputCallback, &m_debugControl);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
			else
			{
				glDisable(GL_DEBUG_OUTPUT);
			}
		}

	private:

		/// <summary>
		/// The callback function used to print OpenGL warnings / error messages.
		/// </summary>
		/// <returns></returns>
		static void GLAPIENTRY glDebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
		{
			// ignore non-significant error/warning codes
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

			std::cout << "---------------" << std::endl;
			std::cout << "Debug message (" << id << "): " << message << std::endl;

			switch (source)
			{
			case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
			}
			std::cout << std::endl;

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
			}
			std::cout << std::endl;

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
			}
			std::cout << std::endl;

			DebugControl * debugControl = (DebugControl*)userParam;

			if (debugControl->useBreakpoint)
			{
#ifdef _MSC_VER
				__debugbreak;
#else
				std::cout << "Automatic breakpoint not available on your platform" << std::endl;
#endif
			}
			std::cout << "---------------" << std::endl;
		}

	};
}