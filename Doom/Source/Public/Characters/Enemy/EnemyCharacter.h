#pragma once
#include <Engine/Classes/GameFramework/Character.h>
#include "Public/Global/DoomEnums.h"

class UPaperSpriteComponent;
class UTimeEventComponent;
class UShapeComponent;

enum class EEnemyState
{
	NONE,
	PATROL,
	ATTACK,
	TRACE,
	DEATH,
	EXP_DEATH,
};

/**
 *	설명
 */
class AEnemyCharacter : public ACharacter
{
public:
	/** 생성자, 소멸자 */
	AEnemyCharacter();
	~AEnemyCharacter();

	/** 객체 값 복사 방지 */
	AEnemyCharacter(const AEnemyCharacter& Other) = delete;
	AEnemyCharacter(AEnemyCharacter&& Other) noexcept = delete;
	AEnemyCharacter& operator=(const AEnemyCharacter& Other) = delete;
	AEnemyCharacter& operator=(AEnemyCharacter&& Other) noexcept = delete;

	/** 상태 머신 메소드 */
	virtual void EntryPatrol() {};
	virtual void EntryAttack() {};
	virtual void EntryTrace() {};
	virtual void EntryDeath() {};
	virtual void EntryExpDeath() {};
	virtual void Patrol(float DeltaTime) {};
	virtual void Attack(float DeltaTime) {};
	virtual void Trace(float DeltaTime) {};
	virtual void Death(float DeltaTime) {};
	virtual void ExpDeath(float DeltaTime) {};

	virtual void ChangeAnimation(FVector Direction) {};
	virtual void ChangeAnimation() {};

	void ChangeState(EEnemyState State);
	bool CheckActorInRange(AActor* TargetActor);
	virtual void TakeDamage(int Damage) {};
	virtual void DropItem();

	/** 이동 관련 메소드 */
	void MoveForward(float Value);
	void MoveRight(float Value);

	std::shared_ptr<UPaperSpriteComponent> GetSpriteComponent() { return SpriteComponent; }
	EEnemyState GetCurEnemyState() { return this->CurEnemyState; }
	void SetCurEnemyState(EEnemyState EnemyState) { this->CurEnemyState = EnemyState; }
	void SetSpeed(float InSpeed) { Speed = InSpeed; }
	void SetCurDirection(FVector Direction) { CurDirection = Direction; }
	FVector GetCurDirection() { return CurDirection; }
	FVector GetDirectionToTargetLocation(FVector TargetLocation);
	int GetHP() { return CurHP; }
	void SetHP(int HP) { CurHP = HP; }
	void AddHP(int HP) { CurHP += HP; }
	void AddTurningLocation(FVector TurningLocation) { TurningLocations.push_back(TurningLocation); }
	float GetDetectRange() { return DetectRange; }
	void SetDetectRange(float Range) { DetectRange = Range; }
	void SetDropItemType(EDropItemType ItemType)
	{
		DropItemType = ItemType;
	}

protected:
	/** 액터 상속 메소드 */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	int MaxHP = 100;
	int CurHP = 100;
	float Speed = 100.0f;

	EEnemyState CurEnemyState = EEnemyState::NONE;
	EEnemyState PrevEnemyState = EEnemyState::NONE;
	EDropItemType DropItemType = EDropItemType::NONE;

	std::vector<FVector> TurningLocations;
	int CurTurningIndex = 0;
	FVector CurDirection = FVector::ZERO;

	float DetectRange = 300.0f;
	float CheckTimeLimit = 1.5f;
	float CheckTime = 0.0f;

private:

};

