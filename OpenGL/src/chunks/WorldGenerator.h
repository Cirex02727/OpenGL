#pragma once

#include <iostream>

#include "utils/FastNoise.h"

class WorldGenerator
{
private:
	std::unique_ptr<FastNoise> m_Noise;

	unsigned int m_Dirt;

public:
	WorldGenerator();
	~WorldGenerator();

	void PrepareGenerate();
	unsigned int Generate(unsigned long long x, unsigned long long y, unsigned long long z) const;
};
