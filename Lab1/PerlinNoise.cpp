#include "PerlinNoise.h"
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

PerlinNoise::PerlinNoise()
{
	// fill the vector with numbers form 0 to 255
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);
	
	// randomly shuffle the permutation vector
	std::default_random_engine engine(std::random_device{}());
	std::shuffle(p.begin(), p.end(), engine);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}

float PerlinNoise::fade(float t) const
{
	// smooth the input value using a fade function
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float a, float b, float t) const
{
	// linearly interpolate between a and b using t
	return a + t * (b - a);
}

float PerlinNoise::gradient(int hash, float x, float y) const
{
	// convert low 4 bits of hash code into 8 gradient directions
	int h = hash & 3;

	// calculate the dot product between the gradient vector and the distance vector
	float u = h < 2 ? x : y;
	float v = h < 2 ? y : x;

	// return the dot product with the appropriate sign
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::noise(float x, float y) const
{
	// Find unit grid cell containing point
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;

	// Get local x and y of point in cell
	x -= std::floor(x);
	y -= std::floor(y);

	// Compute fade curves for x and y
	float u = fade(x);
	float v = fade(y);

	// look up gradient indices for the corners of the square
	int aa = p[p[X] + Y];
	int ab = p[p[X] + Y + 1];
	int ba = p[p[X + 1] + Y];
	int bb = p[p[X + 1] + Y + 1];

	// calculate dot product between gradient and distance vectors, and interpolate the results
	float res = lerp(v, lerp(u, gradient(aa, x, y), gradient(ba, x - 1, y)),lerp(u, gradient(ab, x, y - 1), gradient(bb, x - 1, y - 1)));

	return (res + 1.0f) / 2.0f; // Normalize to [0, 1]
}