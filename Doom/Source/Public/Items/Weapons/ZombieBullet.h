#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPrimitiveComponent;
class UShapeComponent;

/**
 *	설명
 */
class AZombieBullet : public AActor
{
public:
	/** 생성자, 소멸자 */
	AZombieBullet();
	~AZombieBullet();

	/** 객체 값 복사 방지 */
	AZombieBullet(const AZombieBullet& Other) = delete;
	AZombieBullet(AZombieBullet&& Other) noexcept = delete;
	AZombieBullet& operator=(const AZombieBullet& Other) = delete;
	AZombieBullet& operator=(AZombieBullet&& Other) noexcept = delete;

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