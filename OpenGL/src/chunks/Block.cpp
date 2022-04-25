#include "Block.h"

#include <string>

#include "render/Loader.h"

Block::Block(const char* name)
    : m_ID(-1), m_Name(name), m_IsTransparent(false), m_TexIds() {}

Block::~Block()
{
}

Block* Block::loadTexture(const char* path)
{
    std::fill_n(m_TexIds, 6, Loader::loadTexture(path));
    return this;
}
