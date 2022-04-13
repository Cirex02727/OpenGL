#pragma once

#include <memory>
#include <unordered_map>

#include "chunks/Block.h"
#include "render/Shader.h"
#include "entity/Camera.h"
#include "render/VertexArray.h"
#include "utils/frustum/Frustum.h"

class FastNoise;
class WorldGenerator;

enum class FaceSide
{
	FRONT, BACK, RIGHT, LEFT, TOP, BOTTOM, LENGHT
};

class Chunk
{
public:
	static const unsigned short c_ChunkSize = 15;
	static const unsigned short c_ChunkHeight = 255;
	static const unsigned int c_MaxChunkIndex = c_ChunkSize * c_ChunkHeight * c_ChunkSize;

private:
	unsigned int* m_Blocks;
	std::unique_ptr<VertexArray> m_VAO;

	std::unordered_map<unsigned int, std::size_t> m_MemoryMap;

	WorldGenerator* m_Generator;
	unsigned int m_ChunkX, m_ChunkZ;
	double m_GlobalChunkX, m_GlobalChunkZ;

	AABB m_Aabb;

	void Build();
	unsigned int buildVoxelData(unsigned short x, unsigned short y, unsigned short z, unsigned char side, unsigned char uvx, unsigned char uvy);
	void AddFace(std::vector<unsigned int>* vector, unsigned short x, unsigned short y, unsigned short z, unsigned int* type, FaceSide side, unsigned char* vertices, unsigned char* uvs);

public:
	Chunk(WorldGenerator* generator, unsigned int chunkX, unsigned int chunkZ);
	~Chunk();

	unsigned int* GetBlockType(unsigned short x, unsigned short y, unsigned short z);
	unsigned int GetGlobalBlockType(unsigned short x, unsigned short y, unsigned short z);
	Block* GetBlock(unsigned short x, unsigned short y, unsigned short z);

	void Render(Camera* camera, Shader* shader, int& visibleChunks);

	bool IsVisible(Camera* camera) const;
};
