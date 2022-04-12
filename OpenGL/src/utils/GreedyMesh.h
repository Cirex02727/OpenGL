#pragma once

#include "glm/glm.hpp"

#include <unordered_map>

class Chunk;
class Block;

class GreedyMesh
{
public:
	static void Calculate(std::unordered_map<unsigned int, std::size_t>* memoryMap, Chunk* chunk);

	static bool IsBlockFaceVisible(Chunk* chunk, unsigned short* blockPosition, int axis, bool backFace);
	static bool CompareStep(Chunk* chunk, unsigned short* a, unsigned short* b, int direction, bool backFace);

	static void AddFace(std::unordered_map<unsigned int, std::size_t>* memoryMap, unsigned short* v1, unsigned short* v2, unsigned short* v3, unsigned short* v4, Block* block, bool backFace);
};
