#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPrimitiveComponent;
class UShapeComponent;

/**
 *	����
 */
class AZombieBullet : public AActor
{
public:
	/** ������, �Ҹ��� */
	AZombieBullet();
	~AZombieBullet();

	/** ��ü �� ���� ���� */
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
	/** ������ */
	std::weak_ptr<ACharacter> Owner;
	std::shared_ptr<UPrimitiveComponent> MeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;


	FVector Direction = FVector::ZERO;

	float CurLifeTime = 0.0f;
	float LifeTime = 1.0f;
};