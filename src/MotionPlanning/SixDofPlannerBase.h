#pragma once
#include <Math/Vectorf.h>
#include <Math/UniformRandom.h>
#include <Math/Interpolation.h>
#include <Math/Quaternion.h>
#include <MotionPlanning/CollisionManager.h>
#include <Math/Constant.h>
#include <vector>

namespace MotionPlanning
{
	/// <summary>
	/// Base class for six dof planners
	/// </summary>
	class SixDofPlannerBase
	{
	public:

		/// <summary>
		/// Converts Euler angles into a quaternion
		/// </summary>
		/// <param name="eulerAngles">The euler angles.</param>
		/// <returns></returns>
		static Math::Quaternion<float> toQuaternion(const Math::Vector3f & eulerAngles)
		{
			Math::Vector3f xAxis = Math::makeVector(1.f, 0.f, 0.f);
			Math::Vector3f yAxis = Math::makeVector(0.f, 1.f, 0.f);
			Math::Vector3f zAxis = Math::makeVector(0.f, 0.f, 1.f);
			float xAngle = eulerAngles[0];
			float yAngle = eulerAngles[1];
			float zAngle = eulerAngles[2];
			return (Math::Quaternion<float>(xAxis, xAngle)*Math::Quaternion<float>(yAxis, yAngle)*Math::Quaternion<float>(zAxis, zAngle));
		}

		/// <summary>
		/// A configuration
		/// </summary>
		struct Configuration
		{
			/// <summary>
			/// The translation
			/// </summary>
			Math::Vector3f m_translation;
			/// <summary>
			/// The orientation
			/// </summary>
			Math::Quaternion<float> m_orientation;

			/// <summary>
			/// Initializes a new instance of the <see cref="Configuration"/> struct.
			/// </summary>
			Configuration()
			{}

			/// <summary>
			/// Initializes a new instance of the <see cref="Configuration"/> struct.
			/// </summary>
			/// <param name="translation">The translation.</param>
			/// <param name="eulerAngles">The euler angles providing the orientation of the mobile.</param>
			Configuration(const Math::Vector3f & translation, const Math::Vector3f & eulerAngles)
				: m_translation(translation), m_orientation(toQuaternion(eulerAngles))
			{}

			/// <summary>
			/// Initializes a new instance of the <see cref="Configuration"/> struct.
			/// </summary>
			/// <param name="translation">The translation.</param>
			/// <param name="orientation">The orientation of the mobile.</param>
			Configuration(const Math::Vector3f & translation, const Math::Quaternion<float> & orientation)
				: m_translation(translation), m_orientation(orientation)
			{}

			/// <summary>
			/// Interpolates between this configuration and the provided one.
			/// </summary>
			/// <param name="target">The target configuration.</param>
			/// <param name="t">The interpolation parameter in interval [0;1].</param>
			/// <returns></returns>
			Configuration interpolate(const Configuration & target, float t) const
			{
				return Configuration(Math::Interpolation::lerp(m_translation, target.m_translation, t), Math::Quaternion<float>::slerp(m_orientation, target.m_orientation, t));
			}
		};

		/// <summary>
		/// The default search interval for angles.
		/// </summary>
		/// <returns></returns>
		static std::pair<float, float> defaultAngleInterval()
		{
			return std::make_pair(-Math::pi, Math::pi);
		}

		/// <summary>
		/// The default search interval for positions.
		/// </summary>
		/// <returns></returns>
		static std::pair<float, float> defaultPositionInterval()
		{
			return std::make_pair(-1.0f, 1.0f);
		}

	protected:
		/// <summary>
		/// A uniform random generator
		/// </summary>
		Math::UniformRandom m_uniformRandom;
		/// <summary>
		/// The search intervals (x, y, z, angle X, angle Y, angle Z)
		/// </summary>
		std::vector<std::pair<float, float>> m_intervals;
		/// <summary>
		/// The collision manager
		/// </summary>
		MotionPlanning::CollisionManager * m_collisionManager;
		/// <summary>
		/// The mobile
		/// </summary>
		MotionPlanning::CollisionManager::DynamicCollisionObject m_object;
		/// <summary>
		/// The maximum distance between the origin and the vertices of the mobile geometry
		/// </summary>
		float m_maxDistance;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="SixDofPlannerBase"/> class.
		/// </summary>
		/// <param name="collisionManager">The collision manager.</param>
		/// <param name="object">The mobile.</param>
		/// <param name="maxDistance">The maximum distance between two samples along an interpolation path.</param>
		/// <param name="intervals">The search intervals (x, y, z, angle X, angle Y, angle Z).</param>
		SixDofPlannerBase(MotionPlanning::CollisionManager * collisionManager, MotionPlanning::CollisionManager::DynamicCollisionObject object, float maxDistance,
			const std::initializer_list<std::pair<float, float>> & intervals = { defaultPositionInterval(), defaultPositionInterval(), defaultPositionInterval(), defaultAngleInterval(), defaultAngleInterval(),defaultAngleInterval() }
			)
			: m_intervals(intervals), m_collisionManager(collisionManager), m_object(object), m_maxDistance(maxDistance)
		{
			assert(m_intervals.size() == 6);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SixDofPlannerBase"/> class.
		/// </summary>
		/// <param name="collisionManager">The collision manager.</param>
		/// <param name="object">The mobile.</param>
		/// <param name="maxDistance">The maximum distance between two samples along an interpolation path.</param>
		/// <param name="intervals">The search intervals (x, y, z, angle X, angle Y, angle Z).</param>
		SixDofPlannerBase(MotionPlanning::CollisionManager * collisionManager, MotionPlanning::CollisionManager::DynamicCollisionObject object, float maxDistance, const std::vector<std::pair<float, float>> & intervals)
			: m_intervals(intervals), m_collisionManager(collisionManager), m_object(object), m_maxDistance(maxDistance)
		{
			assert(m_intervals.size() == 6);
		}

		/// <summary>
		/// Generates a random configuration.
		/// </summary>
		/// <returns></returns>
		Configuration randomConfiguration();

		/// <summary>
		/// Computes the minimum distance between the mobile and the obstacles.
		/// </summary>
		/// <param name="configuration">The configuration.</param>
		/// <returns></returns>
		float distanceToObstacles(const Configuration & configuration);

		/// <summary>
		/// Tests if this configuration is in collision with the environment
		/// </summary>
		/// <param name="configuration">The configuration.</param>
		/// <returns></returns>
		bool doCollide(const Configuration & configuration);

		/// <summary>
		/// Distance between two configurations
		/// </summary>
		/// <param name="c1">The c1.</param>
		/// <param name="c2">The c2.</param>
		/// <returns></returns>
		float configurationDistance(const Configuration & c1, const Configuration & c2) const;

		/// <summary>
		/// Recursively tests if the interpolation between two configurations collides with the environment.
		/// </summary>
		/// <param name="startPosition">The start position.</param>
		/// <param name="startQ">The start orientation.</param>
		/// <param name="endPosition">The end position.</param>
		/// <param name="endQ">The end orientation.</param>
		/// <param name="dq">The maximum distance between two samples along the path.</param>
		/// <returns></returns>
		bool doCollideRecursive(const Math::Vector3f & startPosition, const Math::Quaternion<float> & startQ, const Math::Vector3f & endPosition, const Math::Quaternion<float> & endQ, float dq);

		/// <summary>
		/// Tests if the interpolation between two configurations collides with the environment.
		/// </summary>
		/// <param name="start">The start configuration.</param>
		/// <param name="end">The target configuration.</param>
		/// <param name="dq">The maximum distance between two samples along the path.</param>
		/// <returns></returns>
		bool doCollide(const Configuration & start, const Configuration & end, float dq);

		/// <summary>
		/// Optimizes the specified path.
		/// </summary>
		/// <param name="toOptimize">The path to optimize.</param>
		/// <param name="dq">The maximum distance between to samples along the path.</param>
		void optimize(::std::vector<Configuration> & toOptimize, float dq);
	};
}