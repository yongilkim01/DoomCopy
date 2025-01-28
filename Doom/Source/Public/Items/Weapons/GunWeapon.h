#pragma once
#include "Public/Items/Weapons/BaseWeapon.h"

class UPaperSpriteComponent;
class ACharacter;

enum class EGunType : uint8
{
	NONE,
	PISTOL,
	SHOTGUN,
};

/**
 *	설명
 */
class AGunWeapon : public ABaseWeapon
{
public:
	/** 생성자, 소멸자 */
	AGunWeapon();
	~AGunWeapon();

	/** 객체 값 복사 방지 */
	AGunWeapon(const AGunWeapon& Other) = delete;
	AGunWeapon(AGunWeapon&& Other) noexcept = delete;
	AGunWeapon& operator=(const AGunWeapon& Other) = delete;
	AGunWeapon& operator=(AGunWeapon&& Other) noexcept = delete;

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
	FVector GetBulletStartLocation()
	{
		return BulletStartLocation;
	}
	void SetBulletStartLocation(FVector Value)
	{
		BulletStartLocation = Value;
	}
	EGunType GetGunType() const
	{
		return GunType;
	}
	void SetGunType(EGunType NewGunType)
	{
		GunType = NewGunType;
	}

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	/** AGunWeapon 세팅 멤버 변수 */
	FVector BulletStartLocation = FVector::ZERO;
	EGunType GunType = EGunType::NONE;

	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;

	/** 이동 관련 멤버 변수 */
	bool bMoving = false;
	float RunningTime = 0.0f;
};