#include "Block.h"

#include <string>

#include "render/Loader.h"

Block::Block(const std::string& name)
    : m_ID(-1), m_Name(name), m_TexIds() {}

Block::~Block()
{
}

Block* Block::loadTexture(const std::string& path)
{
    std::fill_n(m_TexIds, 6, Loader::loadTexture(path));
    return this;
}
