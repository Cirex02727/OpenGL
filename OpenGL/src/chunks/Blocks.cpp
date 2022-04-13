#include "Blocks.h"

std::vector<Block> Blocks::m_Blocks;

void Blocks::Init()
{
	RegisterBlock(*Block("dirt").loadTexture("res/textures/dirt.png"));
}

void Blocks::RegisterBlock(Block block)
{
	block.setID(static_cast<unsigned int>(m_Blocks.size()));
	m_Blocks.push_back(block);
}

Block* Blocks::GetBlock(const unsigned int index)
{
	if (index <= 0 || index > m_Blocks.size())
		return nullptr;

	return &m_Blocks.at(index - 1);
}

Block* Blocks::FindBlock(const std::string name)
{
	for (unsigned int i = 0; i < m_Blocks.size(); i++)
	{
		if (m_Blocks.at(i).GetName() == name)
			return &m_Blocks.at(i);
			
	}
	return nullptr;
}
