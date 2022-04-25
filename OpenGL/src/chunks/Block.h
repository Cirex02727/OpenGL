#pragma once

#include <string>

class Block
{
private:
	unsigned int m_ID;
	const char* m_Name;

	bool m_IsTransparent;

	unsigned int m_TexIds[6];

public:
	Block(const char* name);
	~Block();

	Block* loadTexture(const char* path);

	inline void setID(unsigned int id) { m_ID = id; }
	inline void setIsTransparent(bool isTransparent) { m_IsTransparent = isTransparent; }

	inline unsigned int GetID() const { return m_ID; };
	inline const char* GetName() const { return m_Name; };

	inline bool IsAir() { return m_ID == -1; }
	inline bool IsTransparent() { return m_IsTransparent; }
	inline bool IsLightPass() { return IsAir() || IsTransparent(); }
};
