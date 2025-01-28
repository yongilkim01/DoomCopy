#pragma once
#include "Public/Items/Weapons/GunWeapon.h"

class UPaperSpriteComponent;
class ACharacter;

/**
 *	����
 */
class AShotgunWeapon : public AGunWeapon
{
public:
	/** ������, �Ҹ��� */
	AShotgunWeapon();
	~AShotgunWeapon();

	/** ��ü �� ���� ���� */
	AShotgunWeapon(const AShotgunWeapon& Other) = delete;
	AShotgunWeapon(AShotgunWeapon&& Other) noexcept = delete;
	AShotgunWeapon& operator=(const AShotgunWeapon& Other) = delete;
	AShotgunWeapon& operator=(AShotgunWeapon&& Other) noexcept = delete;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;
	virtual void Reload() override;


private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	
	FVector BulletStartLocation = FVector::ZERO;

};