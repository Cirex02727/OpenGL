#pragma once

#include <string>
#include <vector>

#include "chunks/Block.h"

class Blocks
{
private:
	static std::vector<Block> m_Blocks;

public:
	static void Init();

	static void RegisterBlock(Block block);

	static Block* GetBlock(const unsigned int index);

	static Block* FindBlock(const std::string name);
};
