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

private:
	/** ������ */
	std::weak_ptr<ACharacter> Owner;
};