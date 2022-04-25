#pragma once

#include <iostream>

#include "utils/FastNoise.h"

class WorldGenerator
{
private:
	std::unique_ptr<FastNoise> m_Noise;

	unsigned int m_Dirt, m_Grass;

public:
	WorldGenerator();
	~WorldGenerator();

	void PrepareGenerate();
	unsigned int Generate(double x, double y, double z) const;
};
