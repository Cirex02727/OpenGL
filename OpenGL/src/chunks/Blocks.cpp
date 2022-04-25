#include "Blocks.h"

std::vector<Block> Blocks::m_Blocks;
std::unique_ptr<Block> Blocks::m_Air(std::make_unique<Block>("air"));

void Blocks::Init()
{
	Block dirt("dirt");
	RegisterBlock(*dirt.loadTexture("res/textures/dirt.png"));

	Block glass("glass");
	glass.setIsTransparent(true);
	RegisterBlock(*glass.loadTexture("res/textures/glass.png"));
}

void Blocks::Dellocate()
{
	m_Blocks.clear();

	if(m_Air)
		m_Air.reset();
}

void Blocks::RegisterBlock(Block block)
{
	block.setID(static_cast<unsigned int>(m_Blocks.size()));
	m_Blocks.push_back(block);
}

Block* Blocks::GetBlock(const unsigned int index)
{
	if (index <= 0 || index > m_Blocks.size())
		return m_Air.get();

	return &m_Blocks.at(index - 1);
}

Block* Blocks::FindBlock(const std::string name)
{
	for (unsigned int i = 0; i < m_Blocks.size(); i++)
	{
		if (m_Blocks.at(i).GetName() == name)
			return &m_Blocks.at(i);
			
	}
	return m_Air.get();
}
