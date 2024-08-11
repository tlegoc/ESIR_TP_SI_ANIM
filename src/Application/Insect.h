#pragma once

#include <SceneGraph/NodeInterface.h>
#include <SceneGraph/Group.h>

#include "SceneGraph/Rotate.h"
#include "SceneGraph/Scale.h"
#include "SceneGraph/Transform.h"
#include "SceneGraph/Translate.h"

class Insect
{
public:
	Insect(SceneGraph::Group& root)
	{
		m_root = &root;

		HelperGl::Material mat;

		mat.setDiffuse(HelperGl::Color(245.0f / 255.0f, 194.0f / 255.0f, 66.0f / 255.0f, 1));

		m_rootTransformTranslate = new SceneGraph::Translate();

		m_rootTransformRotateZ = new SceneGraph::Rotate(0.0f, Math::makeVector(0.0f, 0.0f, 1.0f));

		m_rootTransformRotateY = new SceneGraph::Rotate(0.0f, Math::makeVector(0.0f, 1.0f, 0.0f));

		m_rootTransformTranslate->addSon(m_rootTransformRotateZ);
		m_rootTransformRotateZ->addSon(m_rootTransformRotateY);

		m_bodyScaling = new SceneGraph::Scale(Math::makeVector(1.0f, 0.3f, 0.3f));
		m_rootTransformRotateY->addSon(m_bodyScaling);

		m_body = new SceneGraph::Sphere(mat);
		m_bodyScaling->addSon(m_body);

		// Left wing
		m_leftWingTranslateBefore = new SceneGraph::Translate(Math::makeVector(0.0f, 0.3f, 0.0f));
		m_rootTransformRotateY->addSon(m_leftWingTranslateBefore);

		m_leftWingRotate = new SceneGraph::Rotate(0.0f, Math::makeVector(1.0f, 0.0f, 0.0f));
		m_leftWingTranslateBefore->addSon(m_leftWingRotate);

		m_leftWingTranslateAfter = new SceneGraph::Translate(Math::makeVector(0.0f, 0.3f, 0.0f));
		m_leftWingRotate->addSon(m_leftWingTranslateAfter);

		// Right wing
		m_rightWingTranslateBefore = new SceneGraph::Translate(Math::makeVector(0.0f, -0.3f, 0.0f));
		m_rootTransformRotateY->addSon(m_rightWingTranslateBefore);

		m_rightWingRotate = new SceneGraph::Rotate(0.0f, Math::makeVector(1.0f, 0.0f, 0.0f));
		m_rightWingTranslateBefore->addSon(m_rightWingRotate);

		m_rightWingTranslateAfter = new SceneGraph::Translate(Math::makeVector(0.0f, -0.3f, 0.0f));
		m_rightWingRotate->addSon(m_rightWingTranslateAfter);

		// Shared
		m_wingScale = new SceneGraph::Scale(Math::makeVector(0.3f, 0.3f, 0.05f));
		m_leftWingTranslateAfter->addSon(m_wingScale);
		m_rightWingTranslateAfter->addSon(m_wingScale);

		mat.setDiffuse(HelperGl::Color(1, 1, 1, 1));
		m_wingBody = new SceneGraph::Sphere(mat);
		m_wingScale->addSon(m_wingBody);

		// Eyes
		m_rightEyeTransform = new SceneGraph::Translate(Math::makeVector(0.7f, 0.2f, 0.2f));
		m_leftEyeTransform = new SceneGraph::Translate(Math::makeVector(0.7f, -0.2f, 0.2f));

		m_eye = new SceneGraph::Sphere(mat, 0.05f);
		m_rightEyeTransform->addSon(m_eye);
		m_leftEyeTransform->addSon(m_eye);
		m_rootTransformRotateY->addSon(m_rightEyeTransform);
		m_rootTransformRotateY->addSon(m_leftEyeTransform);

		m_root->addSon(m_rootTransformTranslate);
	}

	void SetRotationY(float rotation)
	{
		m_rootTransformRotateY->setAngle(rotation);
	}

	void AddAngleY(float angle)
	{
		m_rootTransformRotateY->setAngle(m_rootTransformRotateY->getAngle() + angle);
	}

	void SetRotationZ(float rotation)
	{
		m_rootTransformRotateZ->setAngle(rotation);
	}

	void AddAngleZ(float angle)
	{
		m_rootTransformRotateZ->setAngle(m_rootTransformRotateZ->getAngle() + angle);
	}

	void BattreDezail(float dt)
	{
		m_currentWing += dt * m_wingSpeed;

		m_leftWingRotate->setAngle(sinf(m_currentWing) * m_wingAngle);
		m_rightWingRotate->setAngle(-sinf(m_currentWing) * m_wingAngle);
	}

	void SetPosition(Math::Vector3f pos)
	{
		m_rootTransformTranslate->setTranslation(pos);
	}

	void AddPosition(Math::Vector3f pos)
	{
		m_rootTransformTranslate->setTranslation(m_rootTransformTranslate->getTranslation() + pos);
	}

	~Insect()
	{
		// I hate myself
		delete m_rootTransformTranslate;
		delete m_rootTransformRotateY;
		delete m_rootTransformRotateZ;
		delete m_leftWingTranslateBefore;
		delete m_leftWingRotate;
		delete m_leftWingTranslateAfter;
		delete m_rightWingTranslateBefore;
		delete m_rightWingRotate;
		delete m_rightWingTranslateAfter;
		delete m_wingScale;
		delete m_wingBody;
		delete m_rightEyeTransform;
		delete m_leftEyeTransform;
		delete m_eye;
		delete m_body;
		delete m_bodyScaling;
	}

private:
	SceneGraph::Group* m_root;

	SceneGraph::Translate* m_rootTransformTranslate;
	SceneGraph::Rotate* m_rootTransformRotateY;
	SceneGraph::Rotate* m_rootTransformRotateZ;

	// left wing
	SceneGraph::Translate* m_leftWingTranslateBefore;
	SceneGraph::Rotate* m_leftWingRotate;
	SceneGraph::Translate* m_leftWingTranslateAfter;

	//Right wing
	SceneGraph::Translate* m_rightWingTranslateBefore;
	SceneGraph::Rotate* m_rightWingRotate;
	SceneGraph::Translate* m_rightWingTranslateAfter;

	// Shared wing
	SceneGraph::Scale* m_wingScale;
	SceneGraph::Sphere* m_wingBody;

	// Eye
	SceneGraph::Translate* m_rightEyeTransform;
	SceneGraph::Translate* m_leftEyeTransform;
	SceneGraph::Sphere* m_eye;

	SceneGraph::Sphere* m_body;
	SceneGraph::Scale* m_bodyScaling;

	float m_currentWing = 0.0f;
	float m_wingAngle = 1.0f;
	float m_wingSpeed = 40.0f;
};
