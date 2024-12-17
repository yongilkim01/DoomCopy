#pragma once
#include "Actor.h"

class APawn : public AActor
{
public:
	// constrcuter destructer
	ENGINE_API APawn();
	ENGINE_API ~APawn();

	// delete Function
	APawn(const APawn& _Other) = delete;
	APawn(APawn&& _Other) noexcept = delete;
	APawn& operator=(const APawn& _Other) = delete;
	APawn& operator=(APawn&& _Other) noexcept = delete;

protected:

private:

};