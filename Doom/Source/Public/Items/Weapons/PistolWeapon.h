#pragma once
#include "Public/Items/Weapons/GunWeapon.h"

class UPaperSpriteComponent;
class ACharacter;

/**
 *	설명
 */
class APistolWeapon : public AGunWeapon
{
public:
	/** 생성자, 소멸자 */
	APistolWeapon();
	~APistolWeapon();

	/** 객체 값 복사 방지 */
	APistolWeapon(const APistolWeapon& Other) = delete;
	APistolWeapon(APistolWeapon&& Other) noexcept = delete;
	APistolWeapon& operator=(const APistolWeapon& Other) = delete;
	APistolWeapon& operator=(APistolWeapon&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void FireGun() override;

	virtual void EntryMove() override;
	virtual void EntryFire() override;
	virtual void EntryReload() override;

	virtual void Move(float DeltaTime) override;
	virtual void Fire(float DeltaTime) override;
	virtual void Reload(float DeltaTime) override;


private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;

	FVector BulletStartLocation = FVector::ZERO;

};