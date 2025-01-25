#pragma once
#include "Engine/Classes/GameFramework/GameMode.h"

class ACameraActor;
class ANavMeshMap;
class ADoomGuyCharacter;
class UNaviMeshManager;
class AE1M1Map;

/**
 *	����
 */
class ANavMeshGameMode : public AGameMode
{
public:
	/** ������, �Ҹ��� */
	ANavMeshGameMode();
	~ANavMeshGameMode();

	/** ��ü �� ���� ���� */
	ANavMeshGameMode(const ANavMeshGameMode& Other) = delete;
	ANavMeshGameMode(ANavMeshGameMode&& Other) noexcept = delete;
	ANavMeshGameMode& operator=(const ANavMeshGameMode& Other) = delete;
	ANavMeshGameMode& operator=(ANavMeshGameMode&& Other) noexcept = delete;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void LevelChangeStart() override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;
	std::shared_ptr<ADoomGuyCharacter> DoomGuyCharacter = nullptr;
	std::shared_ptr<AE1M1Map> TestMap = nullptr;
	std::shared_ptr<ANavMeshMap> NavMap = nullptr;

	UNaviMeshManager* NaviDataManager = nullptr;

	float DistanceToPlayer = 0.0f;
};

