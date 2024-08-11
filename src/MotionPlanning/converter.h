#pragma once

#include <HelperGl/Mesh.h>
#include <fcl/fcl.h>
#include <vector>
#include <memory>
#include <Math/Quaternion.h>
#include <Math/Matrix4x4f.h>

namespace MotionPlanning
{	
	/// <summary>
	/// Converts the specified vector to an fcl vector.
	/// </summary>
	/// <param name="v">The v.</param>
	/// <returns></returns>
	inline fcl::Vector3f convert(Math::Vector3f const & v)
	{
		return fcl::Vector3f(v[0], v[1], v[2]);
	}
	
	/// <summary>
	/// Converts the specified quaternion to an fcl quaternion.
	/// </summary>
	/// <param name="q">The q.</param>
	/// <returns></returns>
	inline fcl::Quaternionf convert(const Math::Quaternion<float> & q)
	{
		return fcl::Quaternionf(q.s(), q.v()[0], q.v()[1], q.v()[2]);
	}

	inline ::std::pair<fcl::Quaternionf, fcl::Vector3f> convert(const Math::Matrix4x4f & m)
	{
		fcl::Vector3f translation = convert(m.getColumn(3).popBack());
		float qw = sqrt(1 + m(0,0) + m(1,1) + m(2,2)) / 2.0;
		float qx = (m(2, 1) - m(1, 2)) / (4 * qw); 
		float qy = (m(0, 2) - m(2, 0)) / (4 * qw); 
		float qz = (m(1, 0) - m(0, 1)) / (4 * qw); 
		return std::make_pair(fcl::Quaternionf(qw, qx, qy, qz), translation);
	}
	
	/// <summary>
	/// Converts the specified face into an fcl triangle.
	/// </summary>
	/// <param name="face">The face.</param>
	/// <returns></returns>
	inline fcl::Triangle convert(const HelperGl::Mesh::Face & face)
	{
		return fcl::Triangle(face.m_verticesIndexes[0], face.m_verticesIndexes[1], face.m_verticesIndexes[2]);
	}
	
	/// <summary>
	/// Converts the specified mesh into an fcl model (useful for collision detection).
	/// </summary>
	/// <param name="mesh">The mesh.</param>
	/// <returns></returns>
	inline ::std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> convert(const HelperGl::Mesh & mesh)
	{
		::std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> model = ::std::make_shared<fcl::BVHModel<fcl::OBBRSS<float>>>();
		::std::vector<fcl::Vector3f> vertices;
		::std::vector<fcl::Triangle> faces;
		for (auto it = mesh.getVertices().begin(), end = mesh.getVertices().end(); it != end; ++it)
		{
			vertices.push_back(convert(*it));
		}
		for (auto it = mesh.getFaces().begin(), end = mesh.getFaces().end(); it != end; ++it)
		{
			faces.push_back(convert(*it));
		}
		model->beginModel();
		model->addSubModel(vertices, faces);
		model->endModel();
		return model;
	}
	
	/// <summary>
	/// Converts the specified model into a collision object.
	/// </summary>
	/// <param name="model">The model.</param>
	/// <returns></returns>
	inline ::std::shared_ptr<fcl::CollisionObject<float>> convert(const ::std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> & model)
	{
		return std::make_shared<fcl::CollisionObject<float>>(model);
	}
}