#pragma once

#include <MotionPlanning/converter.h>
#include <unordered_map>
#include <unordered_set>
#include <fcl/fcl.h>

namespace MotionPlanning
{
	class CollisionManager
	{
	public:

		struct HashFunction;
		class CollisionObject;
		class StaticCollisionObject;
		class DynamicCollisionObject;

		/// <summary>
		/// A base class for a registered collision object
		/// </summary>
		class CollisionObject
		{
			friend class CollisionManager;
			friend class HashFunction;

		protected:
			::std::shared_ptr<fcl::CollisionObject<float>> m_object;
			CollisionManager * m_manager;

			const std::shared_ptr<fcl::CollisionObject<float>> & get() const
			{
				return m_object;
			}

			CollisionObject()
			{}

			CollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object, CollisionManager * manager)
				: m_object(object), m_manager(manager)
			{}

			const ::std::shared_ptr<fcl::CollisionObject<float>> & operator->() const
			{
				return get();
			}

		public:
			/// <summary>
			/// Copy constructor.
			/// </summary>
			/// <param name="">The .</param>
			CollisionObject(const CollisionObject &) = default;

			/// <summary>
			/// Assignment operator.
			/// </summary>
			/// <param name="">The other instance.</param>
			/// <returns></returns>
			CollisionObject & operator= (const CollisionObject &) = default;

			bool operator==(const CollisionObject & other) const
			{
				return m_object == other.m_object;
			}

			/// <summary>
			/// Sets the translation of the collision object identified by index.
			/// </summary>
			/// <param name="index">The index.</param>
			/// <param name="position">The position.</param>
			void setTranslation(const Math::Vector3f & position)
			{
				m_object->setTranslation(convert(position));
			}

			/// <summary>
			/// Sets the orientation of the collision object identified by index.
			/// </summary>
			/// <param name="index">The index.</param>
			/// <param name="quaternion">The quaternion.</param>
			void setOrientation(const Math::Quaternion<float> & quaternion)
			{
				m_object->setQuatRotation(convert(quaternion));
			}

			/// <summary>
			/// Sets the orientation from Euler angles.
			/// </summary>
			/// <param name="xAngle">The x angle.</param>
			/// <param name="yAngle">The y angle.</param>
			/// <param name="zAngle">The z angle.</param>
			void setOrientation(float xAngle, float yAngle, float zAngle)
			{
				Math::Vector3f xAxis = Math::makeVector(1.f, 0.f, 0.f);
				Math::Vector3f yAxis = Math::makeVector(0.f, 1.f, 0.f);
				Math::Vector3f zAxis = Math::makeVector(0.f, 0.f, 1.f);
				setOrientation(Math::Quaternion<float>(xAxis, xAngle)*Math::Quaternion<float>(yAxis, yAngle)*Math::Quaternion<float>(zAxis, zAngle));
			}

			/// <summary>
			/// Sets the transform of the collision object identified by index.
			/// </summary>
			/// <param name="index">The index.</param>
			/// <param name="matrix">The matrix.</param>
			void setTransform(const Math::Matrix4x4f & matrix)
			{
				std::pair<fcl::Quaternionf, fcl::Vector3f> t = convert(matrix);
				m_object->setTranslation(t.second);
				m_object->setQuatRotation(t.first);
			}
		};

		struct HashFunction
		{
			std::size_t operator()(const CollisionObject & object) const
			{
				return ::std::hash< ::std::shared_ptr<fcl::CollisionObject<float>>>()(object.get());
			}
		};

		/// <summary>
		/// A dynamic collision object
		/// </summary>
		/// <seealso cref="CollisionObject" />
		class DynamicCollisionObject : public CollisionObject
		{
			friend class CollisionManager;

		protected:
			DynamicCollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object, CollisionManager * manager)
				: CollisionObject(object, manager)
			{}

		public:
			DynamicCollisionObject() {}
			DynamicCollisionObject(const DynamicCollisionObject &) = default;
			DynamicCollisionObject & operator= (const DynamicCollisionObject &) = default;
		};

		/// <summary>
		/// A static collision object
		/// </summary>
		/// <seealso cref="CollisionObject" />
		class StaticCollisionObject : public CollisionObject
		{
			friend class CollisionManager;

		protected:
			StaticCollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object, CollisionManager * manager)
				: CollisionObject(object, manager)
			{}

		public:
			StaticCollisionObject() {}
			StaticCollisionObject(const StaticCollisionObject &) = default;
			StaticCollisionObject & operator=(const StaticCollisionObject &) = default;
		};

	protected:
		::std::unordered_map<const HelperGl::Mesh *, ::std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>>> m_meshesToModel;
		std::unordered_set<CollisionObject, HashFunction> m_collisionObjects;
		fcl::DynamicAABBTreeCollisionManager<float> m_staticManager;
		fcl::DynamicAABBTreeCollisionManager<float> m_dynamicManager;
		bool m_isInitialized;

		static bool recordCollisionCallback(fcl::CollisionObject<float> * o1, fcl::CollisionObject<float> * o2, void * data);

		static bool doCollideCallback(fcl::CollisionObject<float> * o1, fcl::CollisionObject<float> * o2, void * data);

		static bool distanceCallback(fcl::CollisionObject<float> * o1, fcl::CollisionObject<float> * o2, void * data, float & dist);

	public:				
		/// <summary>
		/// Initializes a new instance of the <see cref="CollisionManager"/> class.
		/// </summary>
		CollisionManager()
			: m_isInitialized(false)
		{}

		/// <summary>
		/// Registers a mesh as a dynamic collision object in the manager
		/// </summary>
		/// <param name="mesh">The mesh.</param>
		/// <returns> The identifier of the collision object corresponding to the mesh. </returns>
		DynamicCollisionObject registerDynamicObject(const HelperGl::Mesh * mesh);
		
		/// <summary>
		/// Registers a mesh as a static collision object.
		/// </summary>
		/// <param name="mesh">The mesh.</param>
		/// <returns></returns>
		StaticCollisionObject registerStaticObject(const HelperGl::Mesh * mesh);

		/// <summary>
		/// Unregisters the provided collision object.
		/// </summary>
		/// <param name="object">The collision object.</param>
		void unregister(const DynamicCollisionObject & object);
		
		/// <summary>
		/// Unregisters the provided collision object.
		/// </summary>
		/// <param name="object">The collision object.</param>
		void unregister(const StaticCollisionObject & object);

		
		/// <summary>
		/// Tests if dynamic objects collide together of with static objects.
		/// </summary>
		/// <returns></returns>
		bool doCollide() ;

		/// <summary>
		/// returns the minimal distance between the mobile and the environment / other mobile objects.
		/// </summary>
		/// <returns></returns>
		float computeDistance() ;

		std::pair<float, Math::Vector3f> computeDistanceAndTranslation() const
		{}
	};
}