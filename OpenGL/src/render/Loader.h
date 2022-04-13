#pragma once

#include <string>
#include <vector>

class Loader
{
public:
	static std::vector<unsigned int> m_Textures;

	static unsigned int loadTexture(const std::string& path);

	static void unloadResources();
};
