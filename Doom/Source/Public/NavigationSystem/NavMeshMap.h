#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;
class UNavMeshComponent;
class UStaticMeshComponent;

/**
 *	설명
 */
class ANavMeshMap : public AActor
{
public:
	/** 생성자, 소멸자 */
	ANavMeshMap();
	~ANavMeshMap();

	/** 객체 값 복사 방지 */
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

