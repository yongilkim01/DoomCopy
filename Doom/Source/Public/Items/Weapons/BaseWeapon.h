#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UPaperSpriteComponent;
class ACharacter;

/**
 *	����
 */
class ABaseWeapon : public AActor
{
public:
	/** ������, �Ҹ��� */
	ABaseWeapon();
	~ABaseWeapon();

	/** ��ü �� ���� ���� */
	ABaseWeapon(const ABaseWeapon& Other) = delete;
	ABaseWeapon(ABaseWeapon&& Other) noexcept = delete;
	ABaseWeapon& operator=(const ABaseWeapon& Other) = delete;
	ABaseWeapon& operator=(ABaseWeapon&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Reload();

	bool IsMoving() const
	{
		return bMoving;
	}
	void SetMoving(bool Value)
	{
		bMoving = Value;
	}
	void SetMovingStartLocation(FVector Value)
	{
		MovingStartLocation = Value;
		//SetActorLocation(MovingStartLocation);
	}

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	/** ������ */
	std::weak_ptr<ACharacter> Owner;

	/** �̵� ���� ��� ���� */
	bool bMoving = false;
	FVector MovingStartLocation = FVector::ZERO;
	float RunningTime = 0.0f;
};