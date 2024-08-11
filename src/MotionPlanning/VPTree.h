#pragma once
#include <algorithm>
#include <limits>
#include <cassert>

static size_t s_vpTreeDistanceCount = 0;

namespace MotionPlanning
{
	/// <summary>
	/// Vantage point tree for nearest neighbor queries
	/// </summary>
	template <typename Data, typename SearchData>
	class VPTree
	{
		/// <summary>
		/// The buckets size (maximum number of elements in the leaves).
		/// </summary>
		/// <returns></returns>
		static constexpr size_t bucketSize() { return 16; }

		/// <summary>
		/// A node of vantage point tree
		/// </summary>
		class Node
		{
		protected:
			/// <summary>
			/// The m centroid
			/// </summary>
			Data m_centroid;
			/// <summary>
			/// The elements stored in the node
			/// </summary>
			::std::vector<Data> m_data;
			/// <summary>
			/// The actual radius of the node (covers all sub nodes)
			/// </summary>
			float m_radius;
			/// <summary>
			/// The distance limit used to separate inside and outside 
			/// </summary>
			float m_limit;
			/// <summary>
			/// The left node (node with elements which distance is lesser than m_limit)
			/// </summary>
			Node * m_left;
			/// <summary>
			/// The right node (node with elements which distance is greater than m_limit)
			/// </summary>
			Node * m_right;

		public:
			/// <summary>
			/// Initializes a new instance of the <see cref="VPTree"/> class.
			/// </summary>
			/// <param name="centroid">The centroid of the node.</param>
			Node(const Data & centroid)
				: m_centroid(centroid), m_radius(0), m_left(nullptr), m_right(nullptr), m_limit(std::numeric_limits<float>::max())
			{
				m_data.reserve(bucketSize());
			}

			/// <summary>
			/// Finalizes an instance of the <see cref="VPTree"/> class. Destroys all sub nodes 
			/// </summary>
			~Node()
			{
				if(m_left != nullptr) delete m_left;
				if(m_right != nullptr) delete m_right;
			}

			/// <summary>
			/// Collects the data in the tree.
			/// </summary>
			/// <param name="result">The collected data.</param>
			void collectData(std::vector<Data> & result)
			{
				result.push_back(m_centroid);
				if (m_left) { m_left->collectData(result); }
				if (m_right) { m_right->collectData(result);  }
			}

			/// <summary>
			/// Adds the specified value in the tree.
			/// </summary>
			/// <param name="value">The value to be added.</param>
			/// <param name="distance">The distance function.</param>
			template <typename DistanceFunction>
			void add(const Data & value, const DistanceFunction & distance)
			{
				m_radius = std::max(m_radius, distance(m_centroid, value));
				if (m_left && m_right)
				{
					if (distance(m_centroid, value) <= m_limit)
					{
						m_left->add(value, distance);
					}
					else
					{
						m_right->add(value, distance);
					}
				}
				else
				{
					m_data.push_back(value);
					if (m_data.size() == bucketSize())
					{
						auto comp = [this, &distance](const Data & d1, const Data & d2) { return distance(m_centroid, d1) < distance(m_centroid, d2); };
						std::sort(m_data.begin(), m_data.end(), comp);
						m_limit = distance(m_centroid, m_data[bucketSize() / 2 - 1]);
						m_left = new Node(m_data.front());
						m_right = new Node(m_data.back());
						for (auto it = m_data.begin() + 1, end = m_data.end() - 1; it != end; ++it)
						{
							if (distance(m_centroid, *it) <= m_limit)
							{
								m_left->add(*it, distance);
							}
							else
							{
								m_right->add(*it, distance);
							}
						}
						m_data.clear();
					}
				}
			}
		
			/// <summary>
			/// Finds the nearest neighbor of value
			/// </summary>
			/// <param name="value">The value.</param>
			/// <param name="distance">The distance function.</param>
			/// <param name="nearestRadius">The nearest distance found so far.</param>
			/// <returns></returns>
			template <typename DistanceFunction>
			Data nearestNeighbour(const SearchData & value, const DistanceFunction & distance, float nearestRadius=std::numeric_limits<float>::max()) const
			{
				Data nearest = m_centroid;
				if (distance(m_centroid, value) > m_radius + nearestRadius) { return nearest; }
				nearestRadius = std::min(nearestRadius, distance(m_centroid, value));
				if (m_left && m_right) // Internal node
				{
					s_vpTreeDistanceCount += 5;
					bool swap = false;
					Node * first = m_left, *second = m_right;
					float centroidDistance = distance(m_centroid, value);
					if (centroidDistance > m_limit)
					{
						std::swap(first, second);
						swap = true;
					}
					Data nearestFirst = first->nearestNeighbour(value, distance, nearestRadius);
					if (distance(nearestFirst, value) < distance(nearest, value))
					{
						nearest = nearestFirst;
						nearestRadius = distance(nearest, value);
					}
					float nearestDistance = distance(nearest, value);
					if (!swap && centroidDistance + nearestDistance > m_limit ||
						swap && centroidDistance - nearestDistance <= m_limit)
					{
						Data nearestSecond = second->nearestNeighbour(value, distance, nearestRadius);
						if (distance(nearestSecond, value) < nearestDistance)
						{
							nearest = nearestSecond;
							nearestRadius = distance(nearest, value);
						}
					}
				}
				else // leaf node
				{
					float optimalDistance = distance(nearest, value);
					for (auto it = m_data.begin(), end = m_data.end(); it != end; ++it)
					{
						s_vpTreeDistanceCount += 2;
						if (distance(*it, value) < optimalDistance)
						{
							nearest = *it;
							optimalDistance = distance(*it, value);
						}
					}
				}
				return nearest;
			}
		};

		/// <summary>
		/// The root node of the tree
		/// </summary>
		Node *  m_root;
		/// <summary>
		/// The number of elements in the tree
		/// </summary>
		size_t m_nbData;
		/// <summary>
		/// The number of elements in the tree during last reorganization
		/// </summary>
		size_t m_previousNbData;

		/// <summary>
		/// Reorganizes the tree. This method creates a more balanced tree.
		/// </summary>
		void reorganize()
		{
			if (m_root == nullptr || m_nbData != m_previousNbData) { return; } // Nothing to do
			std::cout << "Reorganizing tree..." << std::flush;
			std::vector<Data> collected;
			collected.reserve(m_nbData);
			m_root->collectData(collected);
			m_previousNbData *= 2;
			delete m_root;
			m_root = nullptr;
			while (!collected.empty())
			{
				size_t index = rand() % collected.size();
				std::swap(collected[index], collected.back());
				add(collected.back());
				collected.pop_back();
			}
			std::cout << "OK" << std::endl;
		}

		/// <summary>
		/// Adds the specified value in the tree.
		/// </summary>
		/// <param name="value">The value to be added.</param>
		/// <param name="distance">The distance function.</param>
		template <typename Distance>
		void add(const Data & value, const Distance & distance)
		{
			reorganize();
			if (m_root == nullptr) { m_root = new Node(value); }
			else { m_root->add(value, distance); }
			m_nbData++;
		}

		/// <summary>
		/// Finds the nearest neighbor.
		/// </summary>
		/// <param name="value">The value.</param>
		/// <param name="distance">The distance function.</param>
		/// <returns></returns>
		template <typename Distance>
		Data nearestNeighbour(const SearchData & value, const Distance & distance) const
		{
			return m_root->nearestNeighbour(value, distance);
		}

		/// <summary>
		/// Function used to add elements in the tree (trick to hide distance function type without big loss of performances)
		/// </summary>
		std::function<void(const Data &)> addFunction;

		/// <summary>
		/// Function used to find the nearest neighbor (trick to hide distance function type without big loss of performances)
		/// </summary>
		std::function<Data(const SearchData &)> neighbourFunction;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="VPTree"/> class.
		/// </summary>
		/// <param name="distanceData">The distance function between data elements.</param>
		/// <param name="distanceSearch">The distance function between a data element and the type used to request a nearest neighbor.</param>
		template <typename Distance, typename DistanceSearch>
		VPTree(Distance distanceData, DistanceSearch distanceSearch)
			: m_root(nullptr), m_nbData(0), m_previousNbData(bucketSize()*2)
		{
			addFunction = [this, distanceData](const Data & value) { add(value, distanceData); };
			neighbourFunction = [this, distanceSearch](const SearchData & value) -> Data { return nearestNeighbour(value, distanceSearch); };
		}

		/// <summary>
		/// We delete the copy constructor
		/// </summary>
		/// <param name="">The .</param>
		VPTree(const VPTree &) = delete;

		/// <summary>
		/// We delete the assignment operator
		/// </summary>
		/// <param name="">The .</param>
		/// <returns></returns>
		VPTree & operator= (const VPTree &) = delete;

		/// <summary>
		/// Finalizes an instance of the <see cref="VPTree"/> class.
		/// </summary>
		~VPTree()
		{
			if (m_root != nullptr) { delete m_root; }
		}

		/// <summary>
		/// Adds the specified value in the tree.
		/// </summary>
		/// <param name="value">The value.</param>
		void add(const Data & value)
		{
			addFunction(value);
		}

		/// <summary>
		/// Computes the nearest neighbor.
		/// </summary>
		/// <param name="value">The value.</param>
		/// <returns></returns>
		Data nearestNeighbour(const SearchData & value) const
		{
			return neighbourFunction(value);
		}
	};
}