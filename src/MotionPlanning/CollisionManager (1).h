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
		//using CollisionObject = ::std::shared_ptr<fcl::CollisionObject<float>>;

		struct HashFunction;

		class CollisionObject
		{
			friend class CollisionManager;
			friend class HashFunction;

		protected:
			::std::shared_ptr<fcl::CollisionObject<float>> m_object;

			const std::shared_ptr<fcl::CollisionObject<float>> & get() const
			{
				return m_object;
			}

			CollisionObject()
			{}

			CollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object)
				: m_object(object)
			{}

			const ::std::shared_ptr<fcl::CollisionObject<float>> & operator->() const
			{
				return get();
			}

		public:
			CollisionObject(const CollisionObject &) = default;
			CollisionObject & operator= (const CollisionObject &) = default;

			bool operator==(const CollisionObject & other) const
			{
				return m_object == other.m_object;
			}
		};

		struct HashFunction
		{
			std::size_t operator()(const CollisionObject & object) const
			{
				return ::std::hash< ::std::shared_ptr<fcl::CollisionObject<float>>>()(object.get());
			}
		};

		class DynamicCollisionObject : public CollisionObject
		{
			friend class CollisionManager;

		protected:
			DynamicCollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object)
				: CollisionObject(object)
			{}

		public:
			DynamicCollisionObject() {}
			DynamicCollisionObject(const DynamicCollisionObject &) = default;
			DynamicCollisionObject & operator= (const DynamicCollisionObject &) = default;
		};

		class StaticCollisionObject : public CollisionObject
		{
			friend class CollisionManager;

		protected:
			StaticCollisionObject(const ::std::shared_ptr<fcl::CollisionObject<float>> & object)
				: CollisionObject(object)
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

		static bool recordCollisionCallback(fcl::CollisionObject<float> * o1, fcl::CollisionObject<float> * o2, void * data)
		{
			std::vector<std::pair<fcl::CollisionObject<float> *, fcl::CollisionObject<float> *>> * v = (std::vector<std::pair<fcl::CollisionObject<float> *, fcl::CollisionObject<float> *>>*)data;
			(*v).push_back(std::make_pair(o1, o2));
			return false;
		}

		static bool doCollideCallback(fcl::CollisionObject<float> * o1, fcl::CollisionObject<float> * o2, void * data)
		{
			fcl::CollisionRequest<float> request;
			fcl::CollisionResult<float> result;
			fcl::collide(o1, o2, request, result);
			*((bool*)data) = result.isCollision();
			return result.isCollision();
		}

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
		DynamicCollisionObject registerDynamicObject(const HelperGl::Mesh * mesh)
		{
			// We register the mesh if necessary
			auto it = m_meshesToModel.find(mesh);
			if (it == m_meshesToModel.end())
			{
				it = m_meshesToModel.insert(std::make_pair(mesh, convert(*mesh))).first;
			}
			// We create the collision object
			::std::shared_ptr<fcl::CollisionObject<float>> object = convert(it->second);
			m_dynamicManager.registerObject(object.get()); // Problem with fcl library... Not consistent...
			m_collisionObjects.insert(object);
			return object;
		}
		
		/// <summary>
		/// Registers a mesh as a static collision object.
		/// </summary>
		/// <param name="mesh">The mesh.</param>
		/// <returns></returns>
		StaticCollisionObject registerStaticObject(const HelperGl::Mesh * mesh)
		{
			// We register the mesh if necessary
			auto it = m_meshesToModel.find(mesh);
			if (it == m_meshesToModel.end())
			{
				it = m_meshesToModel.insert(std::make_pair(mesh, convert(*mesh))).first;
			}
			// We create the collision object
			::std::shared_ptr<fcl::CollisionObject<float>> object = convert(it->second);
			m_staticManager.registerObject(object.get()); // Problem with fcl library... Not consistent...
			m_collisionObjects.insert(object);
			return object;
		}

		/// <summary>
		/// Unregisters the provided collision object.
		/// </summary>
		/// <param name="object">The collision object.</param>
		void unregister(const DynamicCollisionObject & object)
		{
			m_dynamicManager.unregisterObject(object.get().get());
			m_collisionObjects.erase(object);
		}
		
		/// <summary>
		/// Unregisters the provided collision object.
		/// </summary>
		/// <param name="object">The collision object.</param>
		void unregister(const StaticCollisionObject & object)
		{
			m_staticManager.unregisterObject(object.get().get());
			m_collisionObjects.erase(object);
		}

		/// <summary>
		/// Sets the translation of the collision object identified by index.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <param name="position">The position.</param>
		void setTranslation(const CollisionObject & object, const Math::Vector3f & position)
		{
			object->setTranslation(convert(position));
		}
		
		/// <summary>
		/// Sets the orientation of the collision object identified by index.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <param name="quaternion">The quaternion.</param>
		void setOrientation(const CollisionObject & object, const Math::Quaternion<float> & quaternion)
		{
			object->setQuatRotation(convert(quaternion));
		}
		
		/// <summary>
		/// Sets the transform of the collision object identified by index.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <param name="matrix">The matrix.</param>
		void setTransform(const CollisionObject & object, const Math::Matrix4x4f & matrix)
		{
			std::pair<fcl::Quaternionf, fcl::Vector3f> t = convert(matrix);
			object->setTranslation(t.second);
			object->setQuatRotation(t.first);
		}
		
		/// <summary>
		/// Tests if dynamic objects collide together of with static objects.
		/// </summary>
		/// <returns></returns>
		bool doCollide()
		{
			if (!m_isInitialized)
			{
				m_staticManager.setup();
				m_dynamicManager.setup();
				m_isInitialized = true;
			}
			bool result = false;
			m_staticManager.update();
			m_dynamicManager.update();
			m_dynamicManager.collide(&m_staticManager, &result, &doCollideCallback);
			if (!result)
			{
				m_dynamicManager.collide(&result, &doCollideCallback);
			}
			return result;
		}
	};
}