#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UPaperSpriteComponent;
class ACharacter;

/**
 *	설명
 */
class ABaseWeapon : public AActor
{
public:
	/** 생성자, 소멸자 */
	ABaseWeapon();
	~ABaseWeapon();

	/** 객체 값 복사 방지 */
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

	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;

	/** 이동 관련 멤버 변수 */
	bool bMoving = false;
	FVector MovingStartLocation = FVector::ZERO;
	float RunningTime = 0.0f;
};