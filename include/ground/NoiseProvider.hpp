
#ifndef NOISE_PROVIDER_HPP
#define NOISE_PROVIDER_HPP

#include "Height.hpp"

class NoiseProvider 
{
    public:
		static void randomizeHMap(Height*);
	private :
		static double frand_a_b(double a, double b);
		static float Get2DPerlinNoiseValue(float x, float y, float res);
};

#endif
