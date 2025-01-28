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

private:
	/** 소유주 */
	std::weak_ptr<ACharacter> Owner;
};