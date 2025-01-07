#pragma once
#include "GameFramework/Actor.h"

class ACameraActor;
class AE1M1Map;

// Ό³Έν :
class AE1M1GameMode : public AActor
{
public:
	// constrcuter destructer
	AE1M1GameMode();
	~AE1M1GameMode();

	// delete Function
	AE1M1GameMode(const AE1M1GameMode& Other) = delete;
	AE1M1GameMode(AE1M1GameMode&& Other) noexcept = delete;
	AE1M1GameMode& operator=(const AE1M1GameMode& Other) = delete;
	AE1M1GameMode& operator=(AE1M1GameMode&& Other) noexcept = delete;

	void Tick(float DeltaTime);

protected:

private:
	std::shared_ptr<ACameraActor> Camera;
	std::shared_ptr<AE1M1Map> E1M1Map;
};

