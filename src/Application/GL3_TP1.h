#pragma once

#include <Application/BaseWithKeyboard.h>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <glm/common.hpp>
#include <gl3/Camera.h>
#include <gl3/GlobalState.h>
#include <fstream>

#include "Helpers.h"

namespace Application
{
	// -----------------
	// Premier TP OpenGL
	// -----------------
	class GL3_TP1 : public BaseWithKeyboard
	{
		gl3::Camera m_camera;
		glm::mat4 m_projection;
		float m_cameraSpeed, m_cameraRotationSpeed;
		float m_lastDt;

		// Data
		GLuint m_vbo, m_ebo, m_vao;
		GLuint m_vertexColorBuffer;
		GLuint m_baseProgram;
		GLuint m_baseProgram_verticesAttribArray, m_baseProgram_colorAttribArray;
		GLint m_baseProgram_uniformMatView, m_baseProgram_uniformMatModel, m_baseProgram_uniformMatProjection;

		std::vector<glm::mat4> m_modelMatrices = {
			glm::translate(glm::vec3(-0.5, 0, 0)),
			glm::translate(glm::vec3(0.5, 0, 0)),
		};

	public:
		GL3_TP1()
		{}

		virtual ~GL3_TP1()
		{}

	protected:
		/// <summary>
		/// Method called when the size of the window changes.
		/// </summary>
		/// <param name="width">The new width.</param>
		/// <param name="height">The new height.</param>
		virtual void reshape(GLint width, GLint height)
		{
			BaseWithKeyboard::reshape(width, height);
		}

		void handleKeys()
		{
			glm::vec3 xAxis(1.0, 0.0, 0.0);
			glm::vec3 yAxis(0.0, 1.0, 0.0);
			if (m_keyboard.isPressed('!')) { quit(); }
			if (m_keyboard.isPressed('r')) { m_camera.translateFront(m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('f')) { m_camera.translateFront(-m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('d')) { m_camera.translateRight(m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('q')) { m_camera.translateRight(-m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('z')) { m_camera.translateUp(m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('s')) { m_camera.translateUp(-m_cameraSpeed * m_lastDt); }
			if (m_keyboard.isPressed('g')) { m_camera.rotateLocal(yAxis, m_cameraRotationSpeed * m_lastDt); }
			if (m_keyboard.isPressed('j')) { m_camera.rotateLocal(yAxis, -m_cameraRotationSpeed * m_lastDt); }
			if (m_keyboard.isPressed('y')) { m_camera.rotateLocal(xAxis, m_cameraRotationSpeed * m_lastDt); }
			if (m_keyboard.isPressed('h')) { m_camera.rotateLocal(xAxis, -m_cameraRotationSpeed * m_lastDt); }
		}

		/// <summary>
		/// Loads a text file into a string.
		/// </summary>
		/// <param name="file">The file to load.</param>
		/// <returns></returns>
		static std::string loadTextFile(const std::filesystem::path& file)
		{
			if (!std::filesystem::exists(file))
			{
				std::cerr << "File " << file.string() << " does not exists" << std::endl;
				throw std::ios_base::failure(file.string() + " does not exists");
			}
			std::stringstream result;
			std::ifstream input(file);
			while (!input.eof())
			{
				std::string tmp;
				std::getline(input, tmp);
				result << tmp << std::endl;
			}
			return result.str();
		}

		virtual void initializeRendering()
		{
			// For debug purpose
			bool outputMessages = true;
			bool breakpointOnMessage = false;
			gl3::GlobalState::getSingleton()->enableDebugMode(outputMessages, breakpointOnMessage);

			// 0 - Camera setup
			m_camera.setPosition(glm::vec3(0.0f, 0.0f, 0.5f));
			m_cameraSpeed = 1.0f;
			m_cameraRotationSpeed = Math::pi / 5.0;
			m_lastDt = 0.1f;

			// Vertices positions
			std::vector<glm::vec3> vertices = {
				glm::vec3(-0.5f, -0.5f, 0.0f),
				glm::vec3(0.5f, -0.5f, 0.0f),
				glm::vec3(0.0f,  0.5f, 0.0f)
			};
			// Vertices colors
			std::vector<glm::vec3> colors = {
				glm::vec3(1.0, 0.0, 0.0),
				glm::vec3(0.0, 1.0, 0.0),
				glm::vec3(0.0, 0.0, 1.0)
			};
			// Indexes for the EBO
			std::vector<GLuint> indexes = { 0, 1, 2 };

			// The base path of the shader files
			std::filesystem::path shaderPath = Config::dataPath() / "Shaders";

			// Projection
			m_projection = glm::perspective(3.141592653589793238462643383279f/2.0f, (float) getConfiguration().width/ (float) getConfiguration().height, 0.001f, 1000.0f);

			// Init VBO
			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Init EBO
			glGenBuffers(1, &m_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexes.size(), indexes.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

			// Init Color buffer
			glGenBuffers(1, &m_vertexColorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Init shaders
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			std::string vertexShaderSource = loadTextFile(shaderPath / "base.vert");
			const char* vertexShaderSourceChar = vertexShaderSource.c_str();
			GLint vertexShaderSourceLength = vertexShaderSource.length();
			glShaderSource(vertexShader, 1, &vertexShaderSourceChar, &vertexShaderSourceLength);
			glCompileShader(vertexShader);

			if (!Helpers::ShaderCompileStatus(vertexShader)) {
				glDeleteShader(vertexShader);
			}

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			std::string fragmentShaderSource = loadTextFile(shaderPath / "red.frag");
			const char* fragmentShaderSourceChar = fragmentShaderSource.c_str();
			GLint fragmentShaderSourceLength = fragmentShaderSource.length();
			glShaderSource(fragmentShader, 1, &fragmentShaderSourceChar, &fragmentShaderSourceLength);
			glCompileShader(fragmentShader);

			if (!Helpers::ShaderCompileStatus(fragmentShader)) {
				glDeleteShader(fragmentShader);
			}

			m_baseProgram = glCreateProgram();
			glAttachShader(m_baseProgram, vertexShader);
			glAttachShader(m_baseProgram, fragmentShader);
			glLinkProgram(m_baseProgram);

			if (!Helpers::ProgramCompileStatus(m_baseProgram)) {
				glDeleteProgram(m_baseProgram);
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
			}

			glDetachShader(m_baseProgram, vertexShader);
			glDeleteShader(vertexShader);
			glDetachShader(m_baseProgram, fragmentShader);
			glDeleteShader(fragmentShader);

			m_baseProgram_verticesAttribArray = glGetAttribLocation(m_baseProgram, "in_position");
			m_baseProgram_colorAttribArray = glGetAttribLocation(m_baseProgram, "in_color");
			m_baseProgram_uniformMatView = glGetUniformLocation(m_baseProgram, "uni_mat_view");
			m_baseProgram_uniformMatModel = glGetUniformLocation(m_baseProgram, "uni_mat_model");
			m_baseProgram_uniformMatProjection = glGetUniformLocation(m_baseProgram, "uni_mat_projection");

			// Init VAO
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			// RECORD
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glEnableVertexAttribArray(m_baseProgram_verticesAttribArray);
			glVertexAttribPointer(m_baseProgram_verticesAttribArray, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexColorBuffer);
			glEnableVertexAttribArray(m_baseProgram_colorAttribArray);
			glVertexAttribPointer(m_baseProgram_colorAttribArray, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			glBindVertexArray(0);
		}

		virtual void render(double dt)
		{
			// We set the last dt
			m_lastDt = dt;
			// Handles interactions with the keyboard
			handleKeys();

			glUseProgram(m_baseProgram); //glDrawArrays(GL_TRIANGLES, 0, 3);
			glUniformMatrix4fv(m_baseProgram_uniformMatView, 1, GL_FALSE, &m_camera.getInverseTransform()[0][0]);
			glUniformMatrix4fv(m_baseProgram_uniformMatProjection, 1, GL_FALSE, &m_projection[0][0]);
			glBindVertexArray(m_vao);

			for(glm::mat4 &model : m_modelMatrices)
			{
				glUniformMatrix4fv(m_baseProgram_uniformMatModel, 1, GL_FALSE, &model[0][0]);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);
			glUseProgram(0);
		}

		virtual void keyPressed(unsigned char key, int x, int y)
		{
			BaseWithKeyboard::keyPressed(key, x, y);
		}
	};

}
