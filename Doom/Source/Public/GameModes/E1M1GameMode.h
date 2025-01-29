#pragma once
#include "Engine/Classes/GameFramework/GameMode.h"

class ADoomGuyCharacter;
class ACameraActor;
class AE1M1Map;
class ANavMeshMap;

// Ό³Έν :
class AE1M1GameMode : public AGameMode
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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void LevelChangeStart() override;

private:
	std::shared_ptr<ADoomGuyCharacter> DoomGuyCharacter = nullptr;
	std::shared_ptr<ACameraActor> Camera = nullptr;
	std::shared_ptr<AE1M1Map> E1M1Map = nullptr;
	std::shared_ptr<ANavMeshMap> NavMap = nullptr;

};

