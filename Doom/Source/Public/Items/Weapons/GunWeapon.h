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

	virtual void Fire();
	virtual void Reload();

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
	/** �̵� ���� ��� ���� */
	bool bMoving = false;
	float RunningTime = 0.0f;
	FVector BulletStartLocation = FVector::ZERO;

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	/** AGunWeapon ���� ��� ���� */
	EGunType GunType = EGunType::NONE;

	/** ������ */
	std::weak_ptr<ACharacter> Owner;

};