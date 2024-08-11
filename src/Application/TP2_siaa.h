#pragma once

#include <Application/BaseWithKeyboard.h>
#include <glm/common.hpp>
#include <gl3/Camera.h>
#include <Config.h>
#include <iostream>
#include <sstream>
#include <istream>
#include <SceneGraph/Group.h>

#include "Animation/KinematicChain.h"
#include "SceneGraph/Cylinder.h"
#include <Animation/CCD.h>

#include "Math/UniformRandom.h"

#define PI 3.1415926435897932384626f

namespace Application
{
	class TP2_siaa : public BaseWithKeyboard
	{
		HelperGl::Camera m_camera;
		float m_cameraSpeed;
		float m_cameraRotationSpeed;
		float m_lastDt;

		SceneGraph::Group m_root;

	public:
		TP2_siaa()
		{
		}

		virtual ~TP2_siaa()
		{
			deleteChain();
		}

	protected:
		virtual void reshape(GLint width, GLint height)
		{
			BaseWithKeyboard::reshape(width, height);
		}

		SceneGraph::Translate m_chainGroup;
		std::vector<SceneGraph::NodeInterface*> m_chainNodes;
		std::vector<SceneGraph::Rotate*> m_rotationsZ;
		std::vector<SceneGraph::Rotate*> m_rotationsX;
		Animation::KinematicChain m_chain;

		struct Association
		{
			SceneGraph::Rotate* m_rotZ;
			SceneGraph::Rotate* m_rotX;
			Animation::KinematicChain::DynamicNode* m_rotZDOF;
			Animation::KinematicChain::DynamicNode* m_rotXDOF;
		};

		std::vector<Association> m_associations;

		CCD* m_ccd;

		Math::Vector3f m_ik_target = Math::makeVector(1, 0, 0);

		SceneGraph::Translate* m_target_transform;
		SceneGraph::Sphere* m_target_sphere;

		void createChain(int size)
		{
			assert(size >= 1);

			HelperGl::Material mat;
			mat.setDiffuse({.5f, .5f, .5f, 1.0f});

			m_chainGroup = SceneGraph::Translate(Math::makeVector(0, 0, 0));

			SceneGraph::Translate* last = &m_chainGroup;

			Animation::KinematicChain::Node* lastKinematic = m_chain.addStaticTranslation(
				m_chain.getRoot(), Math::makeVector(0, 0, 0));


			auto arti = new SceneGraph::Sphere(mat, .2f);
			auto cyl = new SceneGraph::Cylinder(mat, 0.1f, 0.1f, 0.5f);
			m_chainNodes.push_back(arti);
			m_chainNodes.push_back(cyl);

			for (int i = 0; i < size; i++)
			{
				auto rotZ = new SceneGraph::Rotate(0, Math::makeVector(0, 0, 1));
				auto rotX = new SceneGraph::Rotate(0, Math::makeVector(1, 0, 0));
				last->addSon(rotZ);
				rotZ->addSon(rotX);

				rotX->addSon(arti);

				auto trans1 = new SceneGraph::Translate(Math::makeVector(0.0f, -.1f, 0.0f));

				rotX->addSon(trans1);

				auto rotFixed = new SceneGraph::Rotate(Math::piDiv2, Math::makeVector(1, 0, 0));

				trans1->addSon(rotFixed);
				rotFixed->addSon(cyl);

				auto trans2 = new SceneGraph::Translate(Math::makeVector(0.0f, -.5f, 0.0f));

				trans1->addSon(trans2);

				last = trans2;

				m_chainNodes.push_back(rotZ);
				m_chainNodes.push_back(rotX);
				m_chainNodes.push_back(trans1);
				m_chainNodes.push_back(rotFixed);
				m_chainNodes.push_back(trans2);
				m_rotationsZ.push_back(rotZ);
				m_rotationsX.push_back(rotX);

				// Kinematic chain
				Association asso;
				asso.m_rotZ = rotZ;
				asso.m_rotX = rotX;

				asso.m_rotZDOF = m_chain.addDynamicRotation(lastKinematic, Math::makeVector(0, 0, 1),
				                                            Math::makeInterval(-PI / 2.0f, PI / 2.0f), 0.0f);
				asso.m_rotXDOF = m_chain.addDynamicRotation(asso.m_rotZDOF, Math::makeVector(1, 0, 0),
				                                            Math::makeInterval(-PI / 2.0f, PI / 2.0f), 0.0f);

				lastKinematic = m_chain.addStaticTranslation(asso.m_rotXDOF, Math::makeVector(0.0f, -.6f, 0.0f));

				m_associations.push_back(asso);
			}

			m_root.addSon(&m_chainGroup);

			m_ccd = new CCD(&m_chain, lastKinematic);

			m_target_transform = new SceneGraph::Translate(m_ik_target);
			m_root.addSon(m_target_transform);
			mat.setDiffuse(HelperGl::Color(1, 0, 0, 1));
			m_target_sphere = new SceneGraph::Sphere(mat, 0.2f);
			m_target_transform->addSon(m_target_sphere);
		}

		void deleteChain()
		{
			delete m_ccd;
			delete m_target_transform;
			delete m_target_sphere;
		}

		// Controle si la chaine doit essayer d'atteindre la cible ou non.
		// Evite de continuer a resoudre pour une cible trop lointaine
		bool m_try_to_solve = true;

		void handleKeys()
		{
			static Math::Vector3f xAxis = Math::makeVector(1.0, 0.0, 0.0);
			static Math::Vector3f yAxis = Math::makeVector(0.0, 1.0, 0.0);
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
			if (m_keyboard.isPressed('n'))
			{
				m_try_to_solve = true;
				m_ik_target = Math::makeVector(
					static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f,
					static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f,
					static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f
				) * 10.0f;
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

			createChain(10);
		}

		uint32_t m_frameCount = 0;

		virtual void render(double dt)
		{
			m_frameCount++;

			// We set the last dt
			m_lastDt = dt;
			// Handles interactions with the keyboard
			handleKeys();
			// Draw the scene
			glClearColor(94.f / 255.f, 209.f / 255.f, 1.f, 1.f);
			GL::loadMatrix(m_camera.getInverseTransform());

			m_target_transform->setTranslation(m_ik_target);
			if (m_try_to_solve)
				m_try_to_solve = m_ccd->solve(m_ik_target, Math::makeVector(0, 0, 0), .1f);

			// m_ccd->convergeTowards(m_ik_target, Math::makeVector(0, 0, 0), .1f);

			// Update the chain
			for (auto& association : m_associations)
			{
				association.m_rotX->setAngle(association.m_rotXDOF->getDOF()[0]);
				association.m_rotZ->setAngle(association.m_rotZDOF->getDOF()[0]);
			}

			m_root.draw();
		}

		virtual void keyPressed(unsigned char key, int x, int y)
		{
			BaseWithKeyboard::keyPressed(key, x, y);
		}
	};
}
