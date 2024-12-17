#pragma once
#include "GameFramework/Actor.h"

// ���� :
class ATitleGameMode : public AActor
{
public:
	// constrcuter destructer
	ATitleGameMode();
	~ATitleGameMode();

	// delete Function
	ATitleGameMode(const ATitleGameMode& Other) = delete;
	ATitleGameMode(ATitleGameMode&& Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& Other) noexcept = delete;

	void Tick(float DeltaTime);

protected:

private:

};

