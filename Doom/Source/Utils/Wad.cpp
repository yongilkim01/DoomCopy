#include "pch.h"
#include "Wad.h"

UWad::UWad()
{
}

UWad::~UWad()
{
	for (int i = 0; i < LumpVector.size(); i++)
	{
		delete[] LumpVector[i];
		LumpVector[i] = nullptr;
	}
}
