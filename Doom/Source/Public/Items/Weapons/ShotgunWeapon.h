#pragma once
#include "Public/Items/Weapons/GunWeapon.h"

class UPaperSpriteComponent;
class ACharacter;

/**
 *	설명
 */
class AShotgunWeapon : public AGunWeapon
{
public:
	/** 생성자, 소멸자 */
	AShotgunWeapon();
	~AShotgunWeapon();

	/** 객체 값 복사 방지 */
	AShotgunWeapon(const AShotgunWeapon& Other) = delete;
	AShotgunWeapon(AShotgunWeapon&& Other) noexcept = delete;
	AShotgunWeapon& operator=(const AShotgunWeapon& Other) = delete;
	AShotgunWeapon& operator=(AShotgunWeapon&& Other) noexcept = delete;

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