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

	/** ���� �ӽ� �������̵� �޼ҵ� */
	virtual void EntryPatrol() override;
	virtual void EntryAttack() override;
	virtual void EntryTrace() override;
	virtual void EntryDeath() override;
	virtual void EntryExpDeath() override;
	virtual void Patrol(float DeltaTime) override;
	virtual void Attack(float DeltaTime) override;
	virtual void Trace(float DeltaTime) override;
	virtual void Death(float DeltaTime) override;
	virtual void ExpDeath(float DeltaTime) override;

	virtual void ChangeAnimation() override;
	virtual void TakeDamage(int Damage) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

