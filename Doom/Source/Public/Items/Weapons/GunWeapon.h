#pragma once
#include "Public/Items/Weapons/BaseWeapon.h"
#include "Public/Global/DoomEnums.h"

class UPaperSpriteComponent;
class ACharacter;

/**
 *	����
 */
class AGunWeapon : public ABaseWeapon
{
public:
	/** ������, �Ҹ��� */
	AGunWeapon();
	~AGunWeapon();

	/** ��ü �� ���� ���� */
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
	/** AGunWeapon ���� ��� ���� */
	EGunType GunType = EGunType::NONE;
	EGunState CurGunState = EGunState::NONE;

	/** �̵� ���� ��� ���� */
	bool bMoving = false;
	float RunningTime = 0.0f;

	float GunMoveAmplitude = 0.01f;  // ����
	float GunMoveFrequency = 1.0f;    // ���ļ�

	FVector BulletStartLocation = FVector::ZERO;

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	/** ������ */
	std::weak_ptr<ACharacter> Owner;

};