#pragma once
#include <random>
#include <cassert>

namespace Math
{	
	/// <summary>
	/// Uniform random number generator based on mersen twister algorithm
	/// </summary>
	class UniformRandom
	{
		mutable std::mt19937_64 m_generator;

	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="UniformRandom"/> class.
		/// </summary>
		UniformRandom() {}
		
		/// <summary>
		/// Generates a random number in the interval [min,max].
		/// </summary>
		/// <param name="min">The minimum bound.</param>
		/// <param name="max">The maximum bound.</param>
		/// <returns></returns>
		double operator() (double min, double max) const
		{
			assert(min <= max);
			std::uniform_real_distribution<> distribution;
			return distribution(m_generator)*(max-min)+min;
		}
	};
}