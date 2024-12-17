#pragma once
#include "GameFramework/Actor.h"

// Ό³Έν :
class ARound1GameMode : public AActor
{
public:
	// constrcuter destructer
	ARound1GameMode();
	~ARound1GameMode();

	// delete Function
	ARound1GameMode(const ARound1GameMode& Other) = delete;
	ARound1GameMode(ARound1GameMode&& Other) noexcept = delete;
	ARound1GameMode& operator=(const ARound1GameMode& Other) = delete;
	ARound1GameMode& operator=(ARound1GameMode&& Other) noexcept = delete;

	void Tick(float DeltaTime);

protected:

private:

};

