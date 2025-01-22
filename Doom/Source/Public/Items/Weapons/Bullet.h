#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACharacter;
class UPrimitiveComponent;
class UPaperSpriteComponent;

/**
 *	����
 */
class ABullet : public AActor
{
public:
	/** ������, �Ҹ��� */
	ABullet();
	~ABullet();

	/** ��ü �� ���� ���� */
	ABullet(const ABullet& Other) = delete;
	ABullet(ABullet&& Other) noexcept = delete;
	ABullet& operator=(const ABullet& Other) = delete;
	ABullet& operator=(ABullet&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetBulletDirection(FVector Value)
	{
		Direction = Value;
	}

private:
	/** ������ */
	std::weak_ptr<ACharacter> Owner;
	std::shared_ptr<UPrimitiveComponent> MeshComponent = nullptr;
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	FVector Direction = FVector::ZERO;

};