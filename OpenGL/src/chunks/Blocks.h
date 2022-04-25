#pragma once

#include <string>
#include <vector>
#include <memory>

#include "chunks/Block.h"

class Blocks
{
private:
	static std::vector<Block> m_Blocks;

	static std::unique_ptr<Block> m_Air;

public:
	static void Init();
	static void Dellocate();

	static void RegisterBlock(Block block);

	static Block* GetBlock(const unsigned int index);

	static Block* FindBlock(const std::string name);
};
