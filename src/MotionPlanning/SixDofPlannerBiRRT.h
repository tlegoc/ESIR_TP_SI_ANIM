#pragma once

#include <vector>
#include <MotionPlanning/SixDofPlannerBase.h>
#include <MotionPlanning/VPTree.h>

namespace MotionPlanning
{
	class SixDofPlannerBiRRT : public SixDofPlannerBase
	{
	protected:
		struct Node;

		struct Node
		{
		protected:
			Configuration m_configuration;
			Node * m_father;
			float m_radius;
			size_t m_connexionTrials, m_connexionSuccess;
			//float m_obstacleDistance;
			std::vector<Node *> m_sons;

		public:
			Node(const Configuration & configuration, float radius)
				: m_configuration(configuration), m_father(nullptr), m_radius(radius), m_connexionTrials(0), m_connexionSuccess(0)//, m_obstacleDistance(-1.0f)
			{}

			float getRadius() const
			{
				return m_radius; // *float(m_connexionSuccess + 1) / float(m_connexionTrials + 1);
			}

			void setRadius(float value)
			{
				m_radius = value;
			}

			void increaseConnexionTrials()
			{
				m_connexionTrials++;
			}

			void increaseConnexionSuccesses()
			{
				m_connexionSuccess++;
			}

			Node * getFather() const 
			{
				return m_father;
			}

			const Configuration & getConfiguration() const
			{
				return m_configuration;
			}

			void addSon(Node * son)
			{
				m_sons.push_back(son);
				son->m_father = this;
			}

			//void setObstacleDistance(float distance)
			//{
			//	m_obstacleDistance = distance;
			//}

			//float getObstacleDistance() const
			//{
			//	return m_obstacleDistance;
			//}
		};

		::std::vector<Node *> m_startNodes;
		::std::vector<Node *> m_targetNodes;
		VPTree<Node *, Configuration> m_startTree;
		VPTree<Node *, Configuration> m_targetTree;
		size_t m_findNearestCount;

		float distance(Node * node, Node * node2) const
		{
			return configurationDistance(node->getConfiguration(), node2->getConfiguration());
		}

		float distance(Node * node, Configuration config) const
		{
			return configurationDistance(node->getConfiguration(), config);
		}

		Configuration limitDistance(const Configuration & source, Configuration target, float maxDistance)
		{
			std::pair<float, float> interval = { 0.0f, 1.0f };
			if (configurationDistance(source, target) <= maxDistance) { return target; }
			for (size_t cpt = 0; cpt < 32; ++cpt)
			{
				float middle = (interval.first + interval.second) * 0.5f;
				if (configurationDistance(source, source.interpolate(target, middle)) < maxDistance)
				{
					interval.first = middle;
				}
				else
				{
					interval.second = middle;
				}
			}
			return source.interpolate(target, interval.first);
		}

	protected:
		Node * createNode(::std::vector<Node *> & nodePool, VPTree<Node*, Configuration> & tree, const Configuration & configuration, float radius)
		{
			Node * tmp = new Node(configuration, radius);
			nodePool.push_back(tmp);
			tree.add(tmp);
			return tmp;
		}

		Node * findNearest(const VPTree<Node *, Configuration> & tree, const Configuration & configuration)
		{
			++m_findNearestCount;
			return tree.nearestNeighbour(configuration);
		}

		//Configuration limitDistance(const Configuration & source, Configuration target, float maxDistance)
		//{
		//	while (configurationDistance(source, target) > maxDistance)
		//	{
		//		target = source.interpolate(target, 0.5);
		//	}
		//	return target;
		//}

		void cleanup()
		{
			for (auto it = m_startNodes.begin(), end = m_startNodes.end(); it != end; ++it)
			{
				delete (*it);
			}
			m_startNodes.erase(m_startNodes.begin(), m_startNodes.end());
			for (auto it = m_targetNodes.begin(), end = m_targetNodes.end(); it != end; ++it)
			{
				delete (*it);
			}
			m_targetNodes.erase(m_targetNodes.begin(), m_targetNodes.end());
		}

	public:
		SixDofPlannerBiRRT(MotionPlanning::CollisionManager * collisionManager, MotionPlanning::CollisionManager::DynamicCollisionObject object, float maxDistance,
			const std::initializer_list<std::pair<float, float>> & intervals = { defaultPositionInterval(), defaultPositionInterval(), defaultPositionInterval(), defaultAngleInterval(), defaultAngleInterval(),defaultAngleInterval() })
			: SixDofPlannerBase(collisionManager, object, maxDistance, intervals),
			m_startTree([this](Node * node, Node * node2) -> float { return distance(node, node2); },
				[this](Node * node, const Configuration & config) -> float {return distance(node, config); }),
			m_targetTree([this](Node * node, Node * node2) -> float { return distance(node, node2); },
				[this](Node * node, const Configuration & config) -> float {return distance(node, config); }),
			m_findNearestCount(0)
		{}

		SixDofPlannerBiRRT(MotionPlanning::CollisionManager * collisionManager, MotionPlanning::CollisionManager::DynamicCollisionObject object, float maxDistance, const std::vector<std::pair<float, float>> & intervals)
			: SixDofPlannerBase(collisionManager, object, maxDistance, intervals),
			m_startTree([this](Node * node, Node * node2) -> float { return distance(node, node2); },
				[this](Node * node, const Configuration & config) -> float {return distance(node, config); }),
			m_targetTree([this](Node * node, Node * node2) -> float { return distance(node, node2); },
				[this](Node * node, const Configuration & config) -> float {return distance(node, config); }),
			m_findNearestCount(0)
		{}

		Node * tryConnect(std::vector<Node *> & nodes, VPTree<Node *, Configuration> & tree, const Configuration & random, float radius, float dq)
		{
			Node * nearest = findNearest(tree, random);
			float distance = configurationDistance(nearest->getConfiguration(), random);
			nearest->increaseConnexionTrials();
			//float factor = 1.0f;
			//if (distance > nearest->getRadius()) { factor = nearest->getRadius() / distance; }
			//Configuration selected = nearest->getConfiguration().interpolate(random, factor);
			Configuration selected = limitDistance(nearest->getConfiguration(), random, nearest->getRadius());
			//float selectedDistance = configurationDistance(selected, nearest->getConfiguration());
			//float deltaDistance = nearest->getObstacleDistance() - selectedDistance ;
			if (/*(deltaDistance <= 0.0f) && */(doCollide(selected) || doCollide(nearest->getConfiguration(), selected, dq)))
			{
				return nullptr;
			}
			nearest->increaseConnexionSuccesses();
			Node * newNode = createNode(nodes, tree, selected, radius);
			//if (deltaDistance <= 0.0f)
			//{
			//	newNode->setObstacleDistance(distanceToObstacles(selected));
			//}
			//else
			//{
			//	//std::cout << "." ;
			//	newNode->setObstacleDistance(deltaDistance);
			//}
			nearest->addSon(newNode);
			return newNode;
		}


		/// <summary>
		/// Method called to plan from start to end configurations.
		/// </summary>
		/// <param name="start">The start configuration.</param>
		/// <param name="end">The end configuration.</param>
		virtual bool plan(const Configuration & start, const Configuration & target, float radius, float dq, std::vector<Configuration> & result)
		{
			if (doCollide(start) || doCollide(target)) { return false; }

			Node * startRoot = createNode(m_startNodes, m_startTree, start, radius);
			//startRoot->setObstacleDistance(distanceToObstacles(startRoot->getConfiguration()));
			Node * targetRoot = createNode(m_targetNodes, m_targetTree, target, radius);
			//targetRoot->setObstacleDistance(distanceToObstacles(targetRoot->getConfiguration()));

			size_t count = 0;

			while (true)
			{
				++count;
				if (count % 1000 == 0) 
				{ 
					std::cout << "RRT size: " << m_startNodes.size()+m_targetNodes.size() << ", Trials: " << count 
						<< " distance computation: " << s_vpTreeDistanceCount << ", average: " << s_vpTreeDistanceCount/m_findNearestCount << std::endl; 
				}

				// We computed the tested configuration
				Configuration random1 = randomConfiguration();
				Configuration random2 = randomConfiguration();

				Node * startConnected = tryConnect(m_startNodes, m_startTree, random1, radius, dq);
				Node * targetConnected = tryConnect(m_targetNodes, m_targetTree, random2, radius, dq);

				if (startConnected)
				{
					Node * nearest = findNearest(m_targetTree, startConnected->getConfiguration());
					if (!doCollide(startConnected->getConfiguration(), nearest->getConfiguration(), dq))
					{
						// We solved the problem, we compute the resulting plan
						targetConnected = nearest;
						computePlan(startConnected, targetConnected, result);
						cleanup();
						return true;
					}
				}
				if (targetConnected)
				{
					Node * nearest = findNearest(m_startTree, targetConnected->getConfiguration());
					if (!doCollide(targetConnected->getConfiguration(), nearest->getConfiguration(), dq))
					{
						// We solved the problem, we compute the resulting plan
						startConnected = nearest;
						computePlan(startConnected, targetConnected, result);
						cleanup();
						return true;
					}
				}
			}

			return false;
		}

		void computePlan(Node * startConnected, Node * targetConnected, std::vector<Configuration> &result)
		{
			Node * target = targetConnected;
			while (target)
			{
				result.push_back(target->getConfiguration());
				target = target->getFather();
			}
			std::reverse(result.begin(), result.end());
			Node * start = startConnected;
			while (start)
			{
				result.push_back(start->getConfiguration());
				start = start->getFather();
			}
		}

	};
}