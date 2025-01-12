#pragma once
#include "Engine/Classes/GameFramework/GameMode.h"

class ACameraActor;
class ANavMeshMap;
class ANavMeshCharacter;
class UNaviMeshManager;
class AE1M1Map;

/**
 *	설명
 */
class ANavMeshGameMode : public AGameMode
{
public:
	/** 생성자, 소멸자 */
	ANavMeshGameMode();
	~ANavMeshGameMode();

	/** 객체 값 복사 방지 */
	ANavMeshGameMode(const ANavMeshGameMode& Other) = delete;
	ANavMeshGameMode(ANavMeshGameMode&& Other) noexcept = delete;
	ANavMeshGameMode& operator=(const ANavMeshGameMode& Other) = delete;
	ANavMeshGameMode& operator=(ANavMeshGameMode&& Other) noexcept = delete;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void LevelChangeStart() override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;
	std::shared_ptr<ANavMeshCharacter> PlayerCharacter = nullptr;
	std::shared_ptr<AE1M1Map> TestMap = nullptr;
	std::shared_ptr<ANavMeshMap> NavMap = nullptr;

	UNaviMeshManager* NaviDataManager = nullptr;

	float DistanceToPlayer = 0.0f;
};

