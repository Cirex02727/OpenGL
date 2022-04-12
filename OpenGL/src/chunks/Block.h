#pragma once

#include <string>

class Block
{
private:
	unsigned int m_ID;
	const std::string m_Name;

public:
	Block(const std::string name);
	~Block();

	inline void setID(unsigned int id) { m_ID = id; }
	inline unsigned int GetID() const { return m_ID; };

	inline const std::string GetName() const { return m_Name; };

	inline bool IsAir() { return m_ID == -1; }
};
