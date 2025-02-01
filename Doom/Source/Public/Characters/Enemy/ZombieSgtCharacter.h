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

	/** 상태 머신 오버라이드 메소드 */
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

