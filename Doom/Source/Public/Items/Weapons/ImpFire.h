#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	설명
 */
class AImpFire : public AActor
{
public:
	/** 생성자, 소멸자 */
	AImpFire();
	~AImpFire();

	/** 객체 값 복사 방지 */
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
	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;


	FVector Direction = FVector::ZERO;

	float CurLifeTime = 0.0f;
	float LifeTime = 1.0f;
};