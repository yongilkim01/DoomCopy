#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	설명
 */
class AZombieCharacter : public AEnemyCharacter
{
public:
	/** 생성자, 소멸자 */
	AZombieCharacter();
	~AZombieCharacter();

	/** 객체 값 복사 방지 */
	AZombieCharacter(const AZombieCharacter& Other) = delete;
	AZombieCharacter(AZombieCharacter&& Other) noexcept = delete;
	AZombieCharacter& operator=(const AZombieCharacter& Other) = delete;
	AZombieCharacter& operator=(AZombieCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

