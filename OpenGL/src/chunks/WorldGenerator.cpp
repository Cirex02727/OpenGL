#include "WorldGenerator.h"

#include "chunks/Chunk.h"
#include "chunks/Blocks.h"
#include "glm/gtc/matrix_transform.hpp"

WorldGenerator::WorldGenerator()
{
	srand((unsigned int) time(NULL));

	int seed = std::rand();
	m_Noise = std::make_unique<FastNoise>(seed); // For seed: std::make_unique<FastNoise>(934865);
	m_Noise->SetFrequency(0.03f);
	m_Noise->SetFractalType(FastNoise::FractalType::FBM);
	m_Noise->SetFractalOctaves(8);
	m_Noise->SetFractalLacunarity(10.0f);

	std::cout << "Generating World - Seed: " << seed << std::endl;
}

WorldGenerator::~WorldGenerator()
{
	m_Noise.reset();
}

void WorldGenerator::PrepareGenerate()
{
	m_Dirt = Blocks::FindBlock("dirt")->GetID();
}

unsigned int WorldGenerator::Generate(unsigned long long x, unsigned long long y, unsigned long long z) const
{
	double noiseV = ((m_Noise->GetSimplex((double) x, (double) z)) + 1.0) / 2.0;

	if (noiseV > glm::smoothstep(0.25f, 1.0f, (float)y / (float)Chunk::c_ChunkSize))
		return m_Dirt + 1;
	else
		return 0;
}
