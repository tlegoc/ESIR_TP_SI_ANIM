#ifndef _Application_TP1_siaa_H
#define _Application_TP1_siaa_H

#include <HelperGl/Camera.h>
#include <HelperGl/LightServer.h>
#include <Application/BaseWithKeyboard.h>
#include <Application/KeyboardStatus.h>
#include <SceneGraph/Group.h>
#include <SceneGraph/MeshVBO_v2.h>
#include <GL/compatibility.h>

#include "HermitInterp.h"
#include "SceneGraph/Sphere.h"
#include "Application/Insect.h"
#include "Application/TrajectoryInterp.h"

namespace Application
{
	class TP1_siaa : public BaseWithKeyboard
	{
	protected:
		HelperGl::Camera m_camera;

		SceneGraph::Group m_root;

		float m_totalTime = 0.0f;

		Insect m_insect;
		HermitInterp m_interp = HermitInterp(
			{0, 0, 0},
			{1, 0, 0},
			{0, 3, 0},
			{0, 3, 0}
		);

		TrajectoryInterp m_traj;

		// std::vector<Insect*> m_bcp;

		virtual void handleKeys()
		{
			// The camera translation speed
			float cameraSpeed = 5.0f;
			// The camera rotation speed (currently not used)
			float rotationSpeed = float(Math::pi / 2.0);

			// quit
			if (m_keyboard.isPressed('w')) { quit(); }
			// Go front
			if (m_keyboard.isPressed('z')) { m_camera.translateLocal(Math::makeVector(0.0f, 0.0f, -cameraSpeed * (float)getDt())); }
			// Go back
			if (m_keyboard.isPressed('s')) { m_camera.translateLocal(Math::makeVector(0.0f, 0.0f, (float)cameraSpeed * (float)getDt())); }
			// Go left
			if (m_keyboard.isPressed('q')) { m_camera.translateLocal(Math::makeVector(-cameraSpeed * (float)getDt(), 0.0f, 0.0f)); }
			// Go right
			if (m_keyboard.isPressed('d')) { m_camera.translateLocal(Math::makeVector((float)cameraSpeed * (float)getDt(), 0.0f, 0.0f)); }
			// Go down
			if (m_keyboard.isPressed('a')) { m_camera.translateLocal(Math::makeVector(0.0f, -cameraSpeed * (float)getDt(), 0.0f)); }
			// Go up
			if (m_keyboard.isPressed('e')) { m_camera.translateLocal(Math::makeVector(0.0f, (float)cameraSpeed * (float)getDt(), 0.0f)); }

		}

	public:
		TP1_siaa() : m_insect(Insect(m_root))
		{
#define NB_INSECT 3
			/*
			for (int i = 0; i < NB_INSECT; i++)
			{
				m_bcp.push_back(new Insect(m_root));
			}*/
		}

		~TP1_siaa()
		{
			/*for (int i = 0; i < NB_INSECT; i++)
			{
				delete m_bcp[i];
			}*/
		}

		virtual void initializeRendering()
		{
			// Light
			HelperGl::Color lightColor(1.0, 1.0, 1.0);
			HelperGl::Color lightAmbiant(0.0, 0.0, 0.0, 0.0);
			Math::Vector4f lightPosition = Math::makeVector(0.0f, 0.0f, 10000.0f, 1.0f); // Point light centered in 0,0,0
			HelperGl::LightServer::Light* light = HelperGl::LightServer::getSingleton()->createLight(lightPosition.popBack(), lightColor, lightColor, lightColor);
			light->enable();
			
			m_traj.addPoint({0, 0, 0}, {5, 0, 0});
			m_traj.addPoint({5, 0, 0}, {0, 5, 0});
			m_traj.addPoint({5, 5, 0}, {-5, 0, 0});
			m_traj.addPoint({0, 5, 0}, {0, -5, 0});
			m_traj.addPoint({0, 0, 0}, {5, 0, 0});
		}

		virtual void render(double dt)
		{
			float dilatation = 2.0f;
			m_totalTime += dt/dilatation;

			glClearColor(94.f/255.f, 209.f/255.f, 1.f, 1.f);

			handleKeys();
			GL::loadMatrix(m_camera.getInverseTransform());

			//m_insect.AddAngleY(dt);
			m_insect.BattreDezail(dt);

			m_insect.SetPosition(m_traj.evaluateLooped(m_totalTime));

			// TODO Rename
			auto rot = m_traj.getEulerRotationLooped(m_totalTime);
			m_insect.SetRotationY(rot[1]);
			m_insect.SetRotationZ(rot[2]);

			/*float delay = 0.05f;
			for (int i = 0; i < NB_INSECT; i++)
			{
				m_bcp[i]->BattreDezail(dt);
				m_bcp[i]->SetPosition(m_interp.evaluateLooped(m_totalTime+delay*i));

				auto rot2 = m_interp.getEulerRotationLooped(m_totalTime+delay*i);
				m_bcp[i]->SetRotationY(rot2[2]);
				m_bcp[i]->SetRotationZ(rot2[1]);
			}*/

			m_root.draw();
		}
	};
}

#endif