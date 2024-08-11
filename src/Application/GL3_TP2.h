#pragma once

#include <Application/BaseWithKeyboard.h>
#include <glm/common.hpp>
#include <gl3/Camera.h>
#include <Config.h>
#include <iostream>
#include <sstream>
#include <istream>

#include "gl3/MeshLoader.h"
#include "gl3/VertexArrayObject.h"
#include "gl3/proxy/SetUniform.h"

namespace Application
{
	class GL3_TP2 : public BaseWithKeyboard
	{
		gl3::Camera m_camera;
		glm::mat4 m_projection;
		float m_cameraSpeed;
		float m_cameraRotationSpeed;
		float m_lastDt;

		gl3::MeshLoader m_meshLoader;
		gl3::Mesh* m_cubeMesh;
		gl3::VertexArrayObject m_vao;
		gl3::ShaderProgram m_phongShader;

		GLuint m_diffuseTexture;
		GLuint m_specularTexture;
		GLuint m_normalTexture;

		// Proxies
		gl3::proxy::SetUniform<glm::mat4> m_matViewProxy;
		gl3::proxy::SetUniform<glm::mat4> m_matModelProxy;
		gl3::proxy::SetUniform<glm::mat4> m_matProjectionProxy;
		gl3::proxy::SetUniform<glm::mat3> m_matNormalProxy;

		// Material proxies
		gl3::proxy::SetUniform<glm::vec3> m_vec3DiffuseProxy;
		gl3::proxy::SetUniform<glm::vec3> m_vec3SpecularProxy;
		gl3::proxy::SetUniform<float> m_floatShininessProxy;
		gl3::proxy::SetUniform<GLint> m_textureDiffuseProxy;
		gl3::proxy::SetUniform<GLint> m_textureSpecularProxy;
		gl3::proxy::SetUniform<GLint> m_textureNormalsProxy;

		// Light proxies
		// gl3::proxy::SetUniform<glm::vec3> m_vec3LightPosProxy;
		// gl3::proxy::SetUniform<glm::vec3> m_vec3LightColorProxy;
		// We can't use proxies for array
		gl3::proxy::SetUniform<GLint> m_intLightCountProxy;
		GLint m_vec3ArrayLightPositionLocation;
		GLint m_vec3ArrayLightColorLocation;

		// Other proxies
		gl3::proxy::SetUniform<glm::vec3> m_vec3CamPosProxy;

		// Phong
		// glm::vec3 m_diffuseColor = { .6f, .6f, .6f };
		glm::vec3 m_diffuseColor = { 1.f, 1.f, 1.f };
		// glm::vec3 m_specularColor = { .4f, .0f, .0f };
		float m_shininess = 10.f;

		// Lights

		std::vector<GLfloat> m_lightPositions = {
				1.5f, 1.5f, 2.f,  // Light 1
				1.5f, 1.5f, -2.f,  // Light 2
		};

		std::vector<GLfloat> m_lightColors {
				1.2f, 1.f, 1.f, // Light 1
				0.f, 0.f, 1.f, // Light 2
		};

	public:
		GL3_TP2()
		{}

		virtual ~GL3_TP2()
		{}

	protected:
		virtual void reshape(GLint width, GLint height)
		{
			BaseWithKeyboard::reshape(width, height);
			m_projection = glm::perspective(3.141592653589793238462643383279f / 2.0f, (float)getConfiguration().width / (float)getConfiguration().height, 0.001f, 1000.0f);
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

		virtual void initializeRendering()
		{
			// Check if lights are valid
			assert(m_lightColors.size() == m_lightPositions.size() && "You must have the same number of positions and colors");
			assert(m_lightColors.size() % 3 == 0 && "Malformed light data");
			assert(m_lightColors.size() % 3 <= 16 && "Too many lights");

			// 0 - Camera setup
			m_camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
			m_cameraSpeed = 1.0f;
			m_cameraRotationSpeed = Math::pi / 5.0;
			m_lastDt = 0.1f;

			// Loading mesh
			auto iterPair = m_meshLoader.load(Config::dataPath() / "Shapes" / "cube.fbx");

			auto start = iterPair.first;
			auto end = iterPair.second;

			// On doit avoir une seul mesh
			for (auto meshMat = start; meshMat != end; ++meshMat)
			{
				m_cubeMesh = meshMat->first;
			}

			// Program
			m_phongShader = gl3::ShaderProgram(Config::dataPath() / "Shaders" / "phong.vert", Config::dataPath() / "Shaders" / "phong.frag");

			// Recuperation des proxys pour mettre les uniforms
			// Matrices
			m_matViewProxy = m_phongShader.setUniformProxy<glm::mat4>("uni_mat_view");
			m_matModelProxy = m_phongShader.setUniformProxy<glm::mat4>("uni_mat_model");
			m_matProjectionProxy = m_phongShader.setUniformProxy<glm::mat4>("uni_mat_projection");
			m_matNormalProxy = m_phongShader.setUniformProxy<glm::mat3>("uni_mat_normal");

			// Material
			m_vec3DiffuseProxy = m_phongShader.setUniformProxy<glm::vec3>("uni_diffuseColor");
			m_floatShininessProxy = m_phongShader.setUniformProxy<float>("uni_shininess");
			m_textureDiffuseProxy = m_phongShader.setUniformProxy<GLint>("uni_texDiff");
			m_textureSpecularProxy = m_phongShader.setUniformProxy<GLint>("uni_texSpec");
			m_textureNormalsProxy = m_phongShader.setUniformProxy<GLint>("uni_texNorm");

			// Light
			// m_vec3LightPosProxy = m_phongShader.setUniformProxy<glm::vec3>("uni_lightPos");
			// m_vec3LightColorProxy = m_phongShader.setUniformProxy<glm::vec3>("uni_lightColor");
			m_intLightCountProxy = m_phongShader.setUniformProxy<GLint>("uni_lightCount");
			m_vec3ArrayLightPositionLocation = m_phongShader.getUniformLocation("uni_lightPositions");
			assert(m_vec3ArrayLightPositionLocation != -1);
			m_vec3ArrayLightColorLocation = m_phongShader.getUniformLocation("uni_lightColors");
			assert(m_vec3ArrayLightColorLocation != -1);

			// Other
			m_vec3CamPosProxy = m_phongShader.setUniformProxy<glm::vec3>("uni_camPos");

			// Textures
			std::string diffusePath = (Config::dataPath() / "textures" / "container" / "container_diffuse.png").string();
			m_diffuseTexture = SOIL_load_OGL_texture(diffusePath.c_str(), 0, 0, SOIL_FLAG_MIPMAPS);
			assert(m_diffuseTexture != 0 && "Texture couldn't be loaded");

			std::string specularPath = (Config::dataPath() / "textures" / "container" / "container_specular.png").string();
			m_specularTexture = SOIL_load_OGL_texture(specularPath.c_str(), 0, 0, SOIL_FLAG_MIPMAPS);
			assert(m_specularTexture != 0 && "Texture couldn't be loaded");

			std::string normalPath = (Config::dataPath() / "textures" / "container" / "container_normals.png").string();
			m_normalTexture = SOIL_load_OGL_texture(normalPath.c_str(), 0, 0, SOIL_FLAG_MIPMAPS);
			assert(m_normalTexture != 0 && "Texture couldn't be loaded");

			// Filtrage des normales
			glBindTexture(GL_TEXTURE_2D, m_normalTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);

			// Recording

			// Récupération des vbos/ebos pour la suite
			auto vboVertices = m_cubeMesh->getVerticesVbo();
			auto vboNormals = m_cubeMesh->getNormalsVbo();
			auto vboTangent = m_cubeMesh->getTagentsVbo();
			auto vboBitangent = m_cubeMesh->getBitangentsVbo();
			auto vboTexcoord = m_cubeMesh->getTextureCoordinatesVbo();
			auto ebo = m_cubeMesh->getIndicesEbo();

			// Preparation des paires pour le vao
			std::pair<std::string, const gl3::VertexBufferObject*> vertices = std::pair("in_position", vboVertices);
			std::pair<std::string, const gl3::VertexBufferObject*> normals = std::pair("in_normal", vboNormals);
			std::pair<std::string, const gl3::VertexBufferObject*> tangent = std::pair("in_tangent", vboTangent);
			std::pair<std::string, const gl3::VertexBufferObject*> bitangent = std::pair("in_bitangent", vboBitangent);
			std::pair<std::string, const gl3::VertexBufferObject*> texcoord = std::pair("in_texcoord", vboTexcoord);

			// Creation et enregistrement
			m_vao = gl3::VertexArrayObject(m_phongShader, { vertices, normals, bitangent, tangent, texcoord }, ebo);

			// Matrice de proj
			m_projection = glm::perspective(3.141592653589793238462643383279f / 2.0f, (float)getConfiguration().width / (float)getConfiguration().height, 0.001f, 1000.0f);
		}

		virtual void render(double dt)
		{
			// We set the last dt
			m_lastDt = dt;
			// Handles interactions with the keyboard
			handleKeys();
			// Draw the scene

			glClearColor(87.0f / 255.0f, 227.0f / 255.0f, 1.0f, 1.0f);

			m_phongShader.use();
			m_matViewProxy.setUniform(m_camera.getInverseTransform());

			// Modele au centre
			m_matModelProxy.setUniform(glm::identity<glm::mat4>());
			m_matProjectionProxy.setUniform(m_projection);
			m_matNormalProxy.setUniform(glm::identity<glm::mat3>());

			// Material
			m_vec3DiffuseProxy.setUniform(m_diffuseColor);
			m_floatShininessProxy.setUniform(m_shininess);

			// Light
			m_intLightCountProxy.setUniform(m_lightPositions.size()/3);
			glUniform3fv(m_vec3ArrayLightPositionLocation, m_lightPositions.size()/3, m_lightPositions.data());
			glUniform3fv(m_vec3ArrayLightColorLocation, m_lightPositions.size()/3, m_lightColors.data());

			// Other
			m_vec3CamPosProxy.setUniform(m_camera.getPosition());

			// Textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);
			m_textureDiffuseProxy.setUniform(0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_specularTexture);
			m_textureSpecularProxy.setUniform(1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_normalTexture);
			m_textureNormalsProxy.setUniform(2);

			m_vao.bind();
			glDrawElements(GL_TRIANGLES, m_vao.eboSize(), GL_UNSIGNED_INT, nullptr);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_vao.unbind();
			m_phongShader.unuse();
		}

		virtual void keyPressed(unsigned char key, int x, int y)
		{
			BaseWithKeyboard::keyPressed(key, x, y);
		}
	};

}
