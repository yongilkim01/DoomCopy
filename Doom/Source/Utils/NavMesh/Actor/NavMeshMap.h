#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;
class UNavMeshComponent;
class UStaticMeshComponent;

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

	//std::shared_ptr<UNavMeshComponent> GetNavMapComponent()
	//{
	//	return SpriteComponent;
	//}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	//std::shared_ptr<UNavMeshComponent> SpriteComponent = nullptr;
	std::shared_ptr<UStaticMeshComponent> MapComponent = nullptr;
};

