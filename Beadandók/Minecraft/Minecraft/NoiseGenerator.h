#pragma once

#include "Includes/PerlinNoise.hpp"
class NoiseGenerator
{
public:
	NoiseGenerator(int seed, int min, int max, int size);
	int getNoise(int x, int z);
private:
	static const double frequency;
	static const int octaves = 8;
	int seed;
	int min;
	int max;
	int size;
	double fs;
	siv::PerlinNoise perlin;
};

