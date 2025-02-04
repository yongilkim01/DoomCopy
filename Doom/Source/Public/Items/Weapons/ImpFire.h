#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	����
 */
class AImpFire : public AActor
{
public:
	/** ������, �Ҹ��� */
	AImpFire();
	~AImpFire();

	/** ��ü �� ���� ���� */
	AImpFire(const AImpFire& Other) = delete;
	AImpFire(AImpFire&& Other) noexcept = delete;
	AImpFire& operator=(const AImpFire& Other) = delete;
	AImpFire& operator=(AImpFire&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetEnemyProjectileDirection(FVector Value)
	{
		Direction = Value;
	}

private:
	/** ������ */
	std::weak_ptr<ACharacter> Owner;
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;


	FVector Direction = FVector::ZERO;

	float CurLifeTime = 0.0f;
	float LifeTime = 1.0f;
};