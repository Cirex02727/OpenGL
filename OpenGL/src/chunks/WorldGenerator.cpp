#include "WorldGenerator.h"

#include "chunks/Chunk.h"
#include "chunks/Blocks.h"
#include "glm/gtc/matrix_transform.hpp"

WorldGenerator::WorldGenerator()
{
	srand((unsigned int) time(NULL));

	int seed = std::rand();
	m_Noise = std::make_unique<FastNoise>(seed); // For seed: std::make_unique<FastNoise>(934865);
	m_Noise->SetFrequency(0.01f);
	m_Noise->SetFractalType(FastNoise::FractalType::FBM);
	m_Noise->SetInterp(FastNoise::Interp::Linear);
	m_Noise->SetFractalOctaves(4);
	m_Noise->SetFractalLacunarity(0.001f);

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

unsigned int WorldGenerator::Generate(double x, double y, double z) const
{
	double noiseV = m_Noise->GetSimplex(x, z) * Chunk::c_ChunkHeight / 8 + 20;

	if (y <= std::max(noiseV, 0.0))
		return m_Dirt + 1;
	else
		return 0;
}
