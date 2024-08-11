#pragma once

#include <Application/BaseWithKeyboard.h>
#include <glm/common.hpp>
#include <gl3/Camera.h>
#include <Config.h>
#include <iostream>
#include <sstream>
#include <istream>

#include "Animation/Physics.h"
#include "Animation/SpringMassSystem.h"
#include "HelperGl/LightServer.h"
#include "SceneGraph/Group.h"
#include "SceneGraph/Patch.h"

namespace Application
{
	class TP3_siaa : public BaseWithKeyboard
	{
		HelperGl::Camera m_camera;
		float m_cameraSpeed;
		float m_cameraRotationSpeed;
		float m_lastDt;

		SceneGraph::Group m_root;

		Animation::SpringMassSystem m_system;
		SceneGraph::Patch* m_patch;
		Animation::SpringMassSystem::PatchDescriptor m_descriptor;
		std::vector<std::pair<int, int>> m_constrained;
		

		static inline Math::Vector3f m_spherePos;
		static inline float m_sphereRadius;
		SceneGraph::Sphere* m_sphere;
		SceneGraph::Translate* m_sphereTransform;
		SceneGraph::Cylinder* m_ground;
		SceneGraph::Translate* m_groundTransfrom;

	public:
		TP3_siaa()
		{
		}

		virtual ~TP3_siaa()
		{
			delete m_patch;
			delete m_sphere;
			delete m_ground;
			delete m_sphereTransform;
			delete m_groundTransfrom;
		}

	protected:
		virtual void reshape(GLint width, GLint height)
		{
			BaseWithKeyboard::reshape(width, height);
		}

		bool m_canUnconstrain = true;

		void handleKeys()
		{
			Math::Vector3f xAxis = Math::makeVector(1.0, 0.0, 0.0);
			Math::Vector3f yAxis = Math::makeVector(0.0, 1.0, 0.0);
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

			if (m_keyboard.isPressed('c') && !m_constrained.empty())
			{
				if (m_canUnconstrain)
				{
					std::pair<int, int> toUnconstrain = m_constrained.back();
					m_constrained.pop_back();
					m_descriptor.unconstrainPosition(toUnconstrain.first, toUnconstrain.second);
					m_canUnconstrain = false;
				}
			}
			else
			{
				m_canUnconstrain = true;
			}
		}

		virtual void initializeRendering()
		{
			// 0 - Camera setup
			m_camera.setPosition(Math::makeVector(0.0f, 0.0f, 5.0f));
			m_cameraSpeed = 1.0f;
			m_cameraRotationSpeed = Math::pi / 5.0;
			m_lastDt = 0.1f;

			// Light
			HelperGl::Color lightColor(1.0, 1.0, 1.0);
			HelperGl::Color lightAmbiant(0.0, 0.0, 0.0, 0.0);
			Math::Vector4f lightPosition = Math::makeVector(0.0f, 0.0f, 10000.0f, 1.0f);
			// Point light centered in 0,0,0
			HelperGl::LightServer::Light* light = HelperGl::LightServer::getSingleton()->createLight(
				lightPosition.popBack(), lightColor, lightColor, lightColor);
			light->enable();

			// Cloth
			int res = 50;
			Math::Matrix4x4f transform = Math::Matrix4x4f::getTranslation(Math::makeVector(-1.5f, -1.5f, 0.0f));
			m_descriptor = m_system.createPatch(3.f, res, 3.f, res, 2, 10.f, transform);
			m_system.setIntegrator(eulerIntegrator, true);
			m_system.addForceFunction(gravity, true);
			m_system.addLinkForceFunction(linkForce, true);
			m_system.addPositionConstraint(groundContraint, true);
			m_system.addPositionConstraint(sphereConstraint, true);
			m_system.setInternalPeriod(1.f / 1000.0f);

			// Constraint
			m_descriptor.constrainPosition(0, 0);
			m_descriptor.constrainPosition(res, 0);
			m_descriptor.constrainPosition(0, res);
			m_descriptor.constrainPosition(res, res);
			m_constrained.push_back({0, 0});
			m_constrained.push_back({res, 0});
			m_constrained.push_back({0, res});
			m_constrained.push_back({res, res});

			HelperGl::Material mat;
			mat.setDiffuse(HelperGl::Color(1, 0, 0, 1));

			m_patch = new SceneGraph::Patch(res+1, res+1, mat);

			m_root.addSon(m_patch);

			m_spherePos = Math::makeVector(0.f, -0.3f, -1.0f);
			m_sphereRadius = 1.f;
			
			mat.setDiffuse(HelperGl::Color(1, 1, 1, 1));
			mat.setSpecular(HelperGl::Color(1, 1, 1, 1));
			m_sphere = new SceneGraph::Sphere(mat, m_sphereRadius);
			m_sphereTransform = new SceneGraph::Translate(m_spherePos);
			m_sphereTransform->addSon(m_sphere);
			m_root.addSon(m_sphereTransform);

			mat.setDiffuse(HelperGl::Color(.3f, 1, 1, 1));
			mat.setSpecular(HelperGl::Color(0, 0, 0, 1));
			m_ground = new SceneGraph::Cylinder(mat, 10, 10, 1);
			m_groundTransfrom = new SceneGraph::Translate(Math::makeVector(0.f, 0.f, -2.51f));
			m_groundTransfrom->addSon(m_ground);
			m_root.addSon(m_groundTransfrom);
		}

		static std::pair<Math::Vector3f, Math::Vector3f> eulerIntegrator(
			const Animation::SpringMassSystem::Mass& previous,
			const Animation::SpringMassSystem::Mass& current,
			float dt)
		{
			Math::Vector3f velocity = current.m_forces * (dt / current.m_mass) + current.m_speed;
			Math::Vector3f position = current.m_position + velocity * dt;

			return {position, velocity};
		}

		static Math::Vector3f gravity(const Animation::SpringMassSystem::Mass& mass)
		{
			return Math::makeVector(0.0f, 0.0f, -9.81f * mass.m_mass);
		}

		static Math::Vector3f linkForce(const Animation::SpringMassSystem::Mass& mass1,
		                                const Animation::SpringMassSystem::Mass& mass2,
		                                const Animation::SpringMassSystem::Link& link)
		{
			Animation::Physics::SpringForce spring(100.f);
			return spring(mass1, mass2, link.m_initialLength) - mass1.m_speed * 0.01f;
		}

		static std::pair<Math::Vector3f, Math::Vector3f> groundContraint(
			const Animation::SpringMassSystem::Mass& previous,
			const Animation::SpringMassSystem::Mass& current)
		{
			auto position = Math::makeVector(
				current.m_position[0],
				current.m_position[1],
				std::max(-1.5f, current.m_position[2]));

			if (current.m_position[2] <= -1.5f)
			{
				auto velocity = Math::makeVector(
					current.m_speed[0],
					current.m_speed[1],
					std::max(0.0f, current.m_speed[2])) * .99f;

				return {position, velocity};
			}

			return {position, current.m_speed};
		}

		static std::pair<Math::Vector3f, Math::Vector3f> sphereConstraint(
			const Animation::SpringMassSystem::Mass& previous,
			const Animation::SpringMassSystem::Mass& current)
		{
			Math::Vector3f dist = current.m_position - m_spherePos;

			if (dist.norm() <= m_sphereRadius)
			{
				Math::Vector3f N = dist.normalized();
				Math::Vector3f position = N * m_sphereRadius;

				// https://computergraphics.stackexchange.com/questions/5498/compute-sphere-tangent-for-normal-mapping
				Math::Vector3f T = (Math::makeVector(0.f, 0.f, 1.f) ^ (position - m_spherePos)).normalized();

				Math::Vector3f B = (N ^ T).normalized();

				float nProj = N * current.m_speed;
				float tProj = T * current.m_speed;
				float bProj = B * current.m_speed;

				nProj = std::max(nProj, 0.0f);

				Math::Vector3f velocity = (N * nProj) + (T * tProj) + (B * bProj);

				return { m_spherePos + position, velocity * 0.99f};
			}

			return {current.m_position, current.m_speed};
		}

		virtual void render(double dt)
		{
			// We set the last dt
			m_lastDt = dt;
			// Handles interactions with the keyboard
			handleKeys();
			// Draw the scene
			glClearColor(94.f / 255.f, 209.f / 255.f, 1.f, 1.f);
			GL::loadMatrix(m_camera.getInverseTransform());

			m_system.update(dt);

			HelperGl::Buffer<Math::Vector3f>& buffer = m_patch->getVertices();

			auto masses = m_system.getMasses();

			for (int i = 0; i < buffer.size(); i++)
			{
				buffer[i] = masses[i].m_position;
			}

			m_root.draw();
		}

		virtual void keyPressed(unsigned char key, int x, int y)
		{
			BaseWithKeyboard::keyPressed(key, x, y);
		}
	};
}
