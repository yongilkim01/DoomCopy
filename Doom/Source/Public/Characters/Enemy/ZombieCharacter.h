#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	����
 */
class AZombieCharacter : public AEnemyCharacter
{
public:
	/** ������, �Ҹ��� */
	AZombieCharacter();
	~AZombieCharacter();

	/** ��ü �� ���� ���� */
	AZombieCharacter(const AZombieCharacter& Other) = delete;
	AZombieCharacter(AZombieCharacter&& Other) noexcept = delete;
	AZombieCharacter& operator=(const AZombieCharacter& Other) = delete;
	AZombieCharacter& operator=(AZombieCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

