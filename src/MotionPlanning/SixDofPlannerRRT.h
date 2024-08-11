#pragma once

#include <vector>
#include <MotionPlanning/SixDofPlannerBase.h>

namespace MotionPlanning
{
	class SixDifPlannerRRT : public SixDofPlannerBase
	{
	protected:
		struct Node;

		struct Node
		{
			Configuration m_configuration;
			Node * m_father;
			float m_radius;
			size_t m_connexionTrials, m_connexionSuccess;
			std::vector<Node *> m_sons;

			Node(const Configuration & configuration, float radius)
				: m_configuration(configuration), m_father(nullptr), m_radius(radius), m_connexionTrials(0), m_connexionSuccess(0)
			{}

			void addSon(Node * son)
			{
				m_sons.push_back(son);
				son->m_father = this;
			}
		};

		::std::vector<Node *> m_nodes;

	protected:
		Node * createNode(const Configuration & configuration, float radius)
		{
			Node * tmp = new Node(configuration, radius);
			m_nodes.push_back(tmp);
			return tmp;
		}

		Node * findNearest(const Configuration & configuration)
		{
			assert(m_nodes.size() > 0);
			Node * nearest = m_nodes[0];
			float distance = configurationDistance(nearest->m_configuration, configuration);
			for (auto it = m_nodes.begin(), end = m_nodes.end(); it != end; ++it)
			{
				float tmpDist = configurationDistance((*it)->m_configuration, configuration);
				if (tmpDist < distance)
				{
					distance = tmpDist;
					nearest = (*it);
				}
			}
			return nearest;
		}

		Configuration limitDistance(const Configuration & source, Configuration target, float maxDistance)
		{
			while (configurationDistance(source, target) > maxDistance)
			{
				target = source.interpolate(target, 0.5);
			}
			return target;
		}

		void cleanup()
		{
			for (auto it = m_nodes.begin(), end = m_nodes.end(); it != end; ++it)
			{
				delete (*it);
			}
			m_nodes.erase(m_nodes.begin(), m_nodes.end());
		}

	public:
		using SixDofPlannerBase::SixDofPlannerBase;

		/// <summary>
		/// Method called to plan from start to end configurations.
		/// </summary>
		/// <param name="start">The start configuration.</param>
		/// <param name="end">The end configuration.</param>
		virtual bool plan(const Configuration & start, const Configuration & target, float radius, float dq, std::vector<Configuration> & result, float adaptationFactor )
		{
			if (doCollide(start) || doCollide(target)) { return false; }

			Node * root = createNode(start, radius);

			size_t count = 0;

			while (true)
			{
				++count;
				if (count % 1000 == 0) { std::cout << "RRT size: " << m_nodes.size() << ", Trials: " << count << std::endl; }

				// We computed the tested configuration
				Configuration random = randomConfiguration();
				Node * nearest = findNearest(random);
				nearest->m_connexionTrials++;

				float distance = configurationDistance(nearest->m_configuration, random);
				float factor = 1.0f;
				if (distance > nearest->m_radius) { factor = nearest->m_radius / distance; }

				Configuration selected = nearest->m_configuration.interpolate(random, factor);
				// We validate the configuration
				if (doCollide(selected) || doCollide(nearest->m_configuration, selected, dq))
				{
					//nearest->m_radius = radius * (float)nearest->m_connexionSuccess / (float)nearest->m_connexionTrials;

					continue; 
				}
				else
				{
					nearest->m_connexionSuccess++;
					//nearest->m_radius = radius * (float)nearest->m_connexionSuccess / (float)nearest->m_connexionTrials;
				}
				
				//std::cout << "." << std::flush;
				// We add the configuration in the RRT
				Node * newNode = createNode(selected, radius);
				nearest->addSon(newNode);
				// We test if we can connect the target configuration
				if (!doCollide(selected, target, dq))
				{
					// A path is found
					result.push_back(target);
					Node * current = newNode;
					while (current)
					{
						result.push_back(current->m_configuration);
						current = current->m_father;
					}
					cleanup();
					std::cout << "RRT size: " << m_nodes.size() << std::endl;
					return true;
				}
				else if(count==100000)
				{
					break;
				}
			}

			Node * nearest = findNearest(target);
			Node * current = nearest;
			while (current)
			{
				result.push_back(current->m_configuration);
				current = current->m_father;
			}
			cleanup();

			return false;
		}

	};
}