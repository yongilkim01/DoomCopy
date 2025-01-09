#pragma once
#include <GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;
class UNavMeshComponent;

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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UNavMeshComponent> SpriteComponent = nullptr;
};

