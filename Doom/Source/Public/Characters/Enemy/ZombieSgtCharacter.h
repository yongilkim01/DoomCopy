#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	설명
 */
class AZombieSgtCharacter : public AEnemyCharacter
{
public:
	/** 생성자, 소멸자 */
	AZombieSgtCharacter();
	~AZombieSgtCharacter();

	/** 객체 값 복사 방지 */
	AZombieSgtCharacter(const AZombieSgtCharacter& Other) = delete;
	AZombieSgtCharacter(AZombieSgtCharacter&& Other) noexcept = delete;
	AZombieSgtCharacter& operator=(const AZombieSgtCharacter& Other) = delete;
	AZombieSgtCharacter& operator=(AZombieSgtCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

