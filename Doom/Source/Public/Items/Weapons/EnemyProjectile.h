#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPrimitiveComponent;
class UShapeComponent;

/**
 *	설명
 */
class AEnemyProjectile : public AActor
{
public:
	/** 생성자, 소멸자 */
	AEnemyProjectile();
	~AEnemyProjectile();

	/** 객체 값 복사 방지 */
	AEnemyProjectile(const AEnemyProjectile& Other) = delete;
	AEnemyProjectile(AEnemyProjectile&& Other) noexcept = delete;
	AEnemyProjectile& operator=(const AEnemyProjectile& Other) = delete;
	AEnemyProjectile& operator=(AEnemyProjectile&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetEnemyProjectileDirection(FVector Value)
	{
		Direction = Value;
	}

private:
	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;
	std::shared_ptr<UPrimitiveComponent> MeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;


	FVector Direction = FVector::ZERO;

	float CurLifeTime = 0.0f;
	float LifeTime = 1.0f;
};