#pragma once
#include <vector>

class PerlinNoise
{
public:
	PerlinNoise();

	float noise(float x, float y) const;

private:

	// Calculation helper functions
	float fade(float t) const;
	float lerp(float a, float b, float t) const;
	float gradient(int hash, float x, float y) const;


	std::vector<int> p; 
};
