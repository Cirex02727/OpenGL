#include "Chunk.h"

#include <iostream>
#include <memory>

#include "render/Shader.h"
#include "chunks/Blocks.h"
#include "utils/BlockFace.h"
#include "utils/FastNoise.h"
#include "utils/GreedyMesh.h"
#include "utils/Instrumentor.h"
#include "chunks/WorldGenerator.h"
#include "render/VertexBufferLayout.h"

#define INDEX(x, y, z) x + (c_ChunkSize * (z + y * c_ChunkSize))

Chunk::Chunk(WorldGenerator* generator, unsigned int chunkX, unsigned int chunkZ)
	: m_Generator(generator), m_ChunkX(chunkX), m_ChunkZ(chunkZ), m_GlobalChunkX(chunkX * c_ChunkSize), m_GlobalChunkZ(chunkZ * c_ChunkSize)
{
	m_Aabb = { glm::vec3(m_GlobalChunkX + c_ChunkSize / 2, c_ChunkHeight / 2, m_GlobalChunkZ + c_ChunkSize / 2), glm::vec3(c_ChunkSize / 2, c_ChunkHeight / 2, c_ChunkSize / 2) };

	{
		InstrumentationTimer timer("Creating Chunk!");

		m_Blocks = new unsigned int[c_ChunkSize * c_ChunkHeight * c_ChunkSize];
		for (unsigned short z = 0; z < c_ChunkSize; z++)
		{
			for (unsigned short x = 0; x < c_ChunkSize; x++)
			{
				for (unsigned short y = 0; y < c_ChunkHeight; y++)
				{
					m_Blocks[INDEX(x, y, z)] = m_Generator->Generate(x + m_GlobalChunkX, y, z + m_GlobalChunkZ);
				}
			}
		}
	}

	Build();

	//GreedyMesh::Calculate(&m_MemoryMap, this);
}

Chunk::~Chunk()
{
	if (m_VAO) m_VAO.reset();

	m_MemoryMap.clear();
	delete[] m_Blocks;
}

void Chunk::Build()
{
	InstrumentationTimer timer("Building VAO!");

	std::unordered_map<unsigned int, std::vector<unsigned int>> m_VerticesData;

	for (unsigned short z = 0; z < c_ChunkSize; z++)
	{
		for (unsigned short x = 0; x < c_ChunkSize; x++)
		{
			for (unsigned short y = 0; y < c_ChunkHeight; y++)
			{
				unsigned int* current = GetBlockType(x, y, z);
				if (*current == 0) continue;


				auto vector = m_VerticesData.find(*current);
				std::vector<unsigned int>* vect;
				if (vector == m_VerticesData.end())
				{
					m_VerticesData[*current] = std::vector<unsigned int>();
					vect = &m_VerticesData[*current];
				}
				else
					vect = &vector->second;

				if (z == c_ChunkSize - 1 || x == c_ChunkSize - 1)
				{
					int a = 0;
				}

				if (GetGlobalBlockType(x + 1, y, z) == 0) AddFace(vect, x, y, z, current, FaceSide::FRONT,  BlockFace::FRONT );
				if (GetGlobalBlockType(x - 1, y, z) == 0) AddFace(vect, x, y, z, current, FaceSide::BACK,   BlockFace::BACK  );
				if (GetGlobalBlockType(x, y + 1, z) == 0) AddFace(vect, x, y, z, current, FaceSide::TOP,    BlockFace::TOP   );
				if (GetGlobalBlockType(x, y - 1, z) == 0) AddFace(vect, x, y, z, current, FaceSide::BOTTOM, BlockFace::BOTTOM);
				if (GetGlobalBlockType(x, y, z + 1) == 0) AddFace(vect, x, y, z, current, FaceSide::RIGHT,  BlockFace::RIGHT );
				if (GetGlobalBlockType(x, y, z - 1) == 0) AddFace(vect, x, y, z, current, FaceSide::LEFT,   BlockFace::LEFT  );
			}
		}
	}

	/*
	const glm::vec3 points[] {glm::vec3(0.0f, 11.0f, 0.0f), glm::vec3(c_ChunkSize - 1, 11.0f, 0.0f)};

	unsigned int index = 0;
	m_VerticesData[index] = std::vector<unsigned int>();
	std::vector<unsigned int>* vect = &m_VerticesData[index];

	for (const auto& point : points)
	{
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::FRONT, BlockFace::FRONT);
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::BACK, BlockFace::BACK);
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::RIGHT, BlockFace::RIGHT);
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::LEFT, BlockFace::LEFT);
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::TOP, BlockFace::TOP);
		AddFace(vect, point.x, point.y, point.z, &index, FaceSide::BOTTOM, BlockFace::BOTTOM);
	}
	*/
	

	m_VAO = std::make_unique<VertexArray>();
	VertexBufferLayout layout;
	layout.Push<unsigned int>(1);

	std::vector<unsigned int> buffer;
	for (auto& [key, value] : m_VerticesData)
	{
		// key is UUID
		m_MemoryMap[key] = value.size();
		buffer.insert(buffer.end(), value.begin(), value.end());
		value.clear();
	}

	m_VAO->AddBuffer(buffer.data(), static_cast<unsigned int>(buffer.size() * sizeof(unsigned int)), layout);
}

void Chunk::AddFace(std::vector<unsigned int>* vector, unsigned short x, unsigned short y, unsigned short z, unsigned int* type, FaceSide side, unsigned char* vertices)
{
	for(unsigned char i = 0; i < 18;)
	{
		vector->push_back(buildVoxelData(x + vertices[i++], y + vertices[i++], z + vertices[i++], (unsigned char) side));
	}
}

unsigned int Chunk::buildVoxelData(unsigned short x, unsigned short y, unsigned short z, unsigned char side)
{
	return (x << 0) | (y << 5) | (z << 13) | (side << 18); // x: 5 - y: 8 - z: 5 - side: 3  |  32-256-32-8
}

unsigned int* Chunk::GetBlockType(unsigned short x, unsigned short y, unsigned short z)
{
	if (x < 0 || x >= c_ChunkSize ||
		y < 0 || y >= c_ChunkHeight ||
		z < 0 || z >= c_ChunkSize)
		return nullptr;

	return &m_Blocks[INDEX(x, y, z)];
}

unsigned int Chunk::GetGlobalBlockType(unsigned short x, unsigned short y, unsigned short z)
{
	if (x < 0 || x >= c_ChunkSize ||
		y < 0 || y >= c_ChunkHeight ||
		z < 0 || z >= c_ChunkSize)
		return m_Generator->Generate(m_GlobalChunkX + static_cast<short>(x), static_cast<short>(y), m_GlobalChunkZ + static_cast<short>(z));

	return m_Blocks[INDEX(x, y, z)];
}

Block* Chunk::GetBlock(unsigned short x, unsigned short y, unsigned short z)
{
	return Blocks::GetBlock(*GetBlockType(x, y, z));
}

void Chunk::Render(Camera* camera, Shader* shader, int& visibleChunks)
{
	//glm::vec3 chunkPos = camera->GetPosition();
	//chunkPos.y = 0;
	//if (glm::distance(chunkPos, glm::vec3(m_GlobalChunkX, 0, m_GlobalChunkZ)) > c_ChunkSize * 500)
	//	return;

	if (!IsVisible(camera))
		return;

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_ChunkX * c_ChunkSize, 0.0f, m_ChunkZ * c_ChunkSize));
	glm::mat4 mvp = camera->GetProjMatrix() * camera->GetViewMatrix() * model;
	shader->Bind();
	shader->SetUniformMat4f("u_MVP", mvp);

	m_VAO->Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_VAO->GetElementsCount());

	visibleChunks++;
}

bool Chunk::IsVisible(Camera* camera) const
{
	return camera->GetFrustum()->isChunkOnFrustum(m_Aabb);
}
