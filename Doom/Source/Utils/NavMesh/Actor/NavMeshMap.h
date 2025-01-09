#pragma once
#include <GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;
class UNavMeshComponent;

/**
 *	����
 */
class ANavMeshMap : public AActor
{
public:
	/** ������, �Ҹ��� */
	ANavMeshMap();
	~ANavMeshMap();

	/** ��ü �� ���� ���� */
	ANavMeshMap(const ANavMeshMap& Other) = delete;
	ANavMeshMap(ANavMeshMap&& Other) noexcept = delete;
	ANavMeshMap& operator=(const ANavMeshMap& Other) = delete;
	ANavMeshMap& operator=(ANavMeshMap&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UNavMeshComponent> SpriteComponent = nullptr;
};

