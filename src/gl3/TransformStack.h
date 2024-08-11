#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stack>

namespace gl3
{
	/// <summary>
	/// A transform stack containing 4x4 matrices.
	/// </summary>
	class TransformStack
	{
		std::stack<glm::mat4x4> m_stack;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="TransformStack"/> class. The stack contains an identity matrix.
		/// </summary>
		TransformStack()
		{
			m_stack.push(glm::mat4x4(1.0));
		}

		/// <summary>
		/// Consultation and modification of the matrix on top of the stack.
		/// </summary>
		/// <returns></returns>
		glm::mat4x4 & current() { return m_stack.top(); }

		/// <summary>
		/// Consultation of the matrix on top of the stack.
		/// </summary>
		/// <returns></returns>
		const glm::mat4x4 & current() const { return m_stack.top(); }

		/// <summary>
		/// Pushes the matrix on top of the stack.
		/// </summary>
		void push() { m_stack.push(m_stack.top()); }

		/// <summary>
		/// Pops the matrix on top of the stack.
		/// </summary>
		void pop() { assert(!m_stack.empty()); m_stack.pop(); }

		/// <summary>
		/// Multiplies the matrix on top of the stack by a translation matrix.
		/// </summary>
		/// <param name="translation">The translation.</param>
		void translate(const glm::vec3 & translation) { current() *= glm::translate(translation); }

		/// <summary>
		/// Multiplies the matrix on top of the stack by a rotation matrix.
		/// </summary>
		/// <param name="angle">The angle.</param>
		/// <param name="axis">The axis.</param>
		void rotate(float angle, const glm::vec3 & axis) { current() *= glm::rotate(glm::degrees(angle), axis); }

  		/// <summary>
		/// Multiplies the matrix on top of the stack by a scale matrix.
		/// </summary>
		/// <param name="factors">The factors.</param>
		void scale(const glm::vec3 & factors) { current() *= glm::scale(factors); }

		/// <summary>
		/// Multiplies the matrix on top of the stack by the provided matrix.
		/// </summary>
		/// <param name="matrix">The matrix.</param>
		void multiply(const glm::mat4x4 & matrix) { current() *= matrix; }

		/// <summary>
		/// Sets the matrix on top of the stack.
		/// </summary>
		/// <param name="matrix">The matrix.</param>
		void set(const glm::mat4x4 & matrix) { current() = matrix; }
	};
}