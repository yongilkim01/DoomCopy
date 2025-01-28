#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	����
 */
class AZombieSgtCharacter : public AEnemyCharacter
{
public:
	/** ������, �Ҹ��� */
	AZombieSgtCharacter();
	~AZombieSgtCharacter();

	/** ��ü �� ���� ���� */
	AZombieSgtCharacter(const AZombieSgtCharacter& Other) = delete;
	AZombieSgtCharacter(AZombieSgtCharacter&& Other) noexcept = delete;
	AZombieSgtCharacter& operator=(const AZombieSgtCharacter& Other) = delete;
	AZombieSgtCharacter& operator=(AZombieSgtCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

