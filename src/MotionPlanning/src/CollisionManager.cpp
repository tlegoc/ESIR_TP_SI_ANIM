#include <MotionPlanning/CollisionManager.h>

namespace MotionPlanning
{
	bool CollisionManager::recordCollisionCallback(fcl::CollisionObject<float>* o1, fcl::CollisionObject<float>* o2, void * data)
	{
		std::vector<std::pair<fcl::CollisionObject<float> *, fcl::CollisionObject<float> *>> * v = (std::vector<std::pair<fcl::CollisionObject<float> *, fcl::CollisionObject<float> *>>*)data;
		(*v).push_back(std::make_pair(o1, o2));
		return false;
	}

	bool CollisionManager::doCollideCallback(fcl::CollisionObject<float>* o1, fcl::CollisionObject<float>* o2, void * data)
	{
		fcl::CollisionRequest<float> request;
		fcl::CollisionResult<float> result;
		fcl::collide(o1, o2, request, result);
		*((bool*)data) = result.isCollision();
		return result.isCollision();
	}

	inline bool CollisionManager::distanceCallback(fcl::CollisionObject<float>* o1, fcl::CollisionObject<float>* o2, void * data, float & dist)
	{
		float * result = (float*)data; // User data
		fcl::DistanceRequest<float> distanceRequest;
		fcl::DistanceResult<float> distanceResult;
		fcl::distance(o1, o2, distanceRequest, distanceResult);
		*result = std::min(*result, distanceResult.min_distance);
		return false;
	}

	CollisionManager::DynamicCollisionObject CollisionManager::registerDynamicObject(const HelperGl::Mesh * mesh)
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
		m_collisionObjects.insert(DynamicCollisionObject(object, this));
		return DynamicCollisionObject(object, this);
	}

	/// <summary>
	/// Registers a mesh as a static collision object.
	/// </summary>
	/// <param name="mesh">The mesh.</param>
	/// <returns></returns>

	CollisionManager::StaticCollisionObject CollisionManager::registerStaticObject(const HelperGl::Mesh * mesh)
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
		m_collisionObjects.insert(StaticCollisionObject(object, this));
		return StaticCollisionObject(object, this);
	}

	void CollisionManager::unregister(const DynamicCollisionObject & object)
	{
		m_dynamicManager.unregisterObject(object.get().get());
		m_collisionObjects.erase(object);
	}

	void CollisionManager::unregister(const StaticCollisionObject & object)
	{
		m_staticManager.unregisterObject(object.get().get());
		m_collisionObjects.erase(object);
	}

	bool CollisionManager::doCollide() 
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

	float CollisionManager::computeDistance() 
	{
		if (!m_isInitialized)
		{
			m_staticManager.setup();
			m_dynamicManager.setup();
			m_isInitialized = true;
		}
		float result = std::numeric_limits<float>::max();
		m_staticManager.update();
		m_dynamicManager.update();
		m_dynamicManager.distance(&m_staticManager, &result, distanceCallback);
		return result;
	}

}