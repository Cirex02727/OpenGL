#include "Maths.h"

unsigned short* Maths::AddVectors(unsigned int size, const unsigned short* v1, const unsigned short* v2)
{
	unsigned short* res = new unsigned short[size];
	for (unsigned int i = 0; i < size; i++)
	{
		res[i] = v1[i] + v2[i];
	}
	return res;
}

unsigned short* Maths::AddVectors(unsigned int size, const unsigned short* v1, const unsigned short* v2, const unsigned short* v3)
{
	unsigned short* res = new unsigned short[size];
	for (unsigned int i = 0; i < size; i++)
	{
		res[i] = v1[i] + v2[i] + v3[i];
	}
	return res;
}
