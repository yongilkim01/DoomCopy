#pragma once
#include "Public/Items/Weapons/BaseWeapon.h"
#include "Public/Global/DoomEnums.h"

class UPaperSpriteComponent;
class ACharacter;

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

	virtual void FireGun();

	virtual void EntryMove();
	virtual void EntryFire();
	virtual void EntryReload();

	virtual void Move(float DeltaTime);
	virtual void Fire(float DeltaTime);
	virtual void Reload(float DeltaTime);

	void ChangeState(EGunState GunState);

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

protected:
	/** AGunWeapon 세팅 멤버 변수 */
	EGunType GunType = EGunType::NONE;
	EGunState CurGunState = EGunState::NONE;

	/** 이동 관련 멤버 변수 */
	bool bMoving = false;
	float RunningTime = 0.0f;

	float GunMoveAmplitude = 0.01f;  // 진폭
	float GunMoveFrequency = 1.0f;    // 주파수

	FVector BulletStartLocation = FVector::ZERO;

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;

};