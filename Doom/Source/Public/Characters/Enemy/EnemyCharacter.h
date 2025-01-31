#pragma once
#include <Engine/Classes/GameFramework/Character.h>

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
};

/**
 *	����
 */
class AEnemyCharacter : public ACharacter
{
public:
	/** ������, �Ҹ��� */
	AEnemyCharacter();
	~AEnemyCharacter();

	/** ��ü �� ���� ���� */
	AEnemyCharacter(const AEnemyCharacter& Other) = delete;
	AEnemyCharacter(AEnemyCharacter&& Other) noexcept = delete;
	AEnemyCharacter& operator=(const AEnemyCharacter& Other) = delete;
	AEnemyCharacter& operator=(AEnemyCharacter&& Other) noexcept = delete;

	/** ���� �ӽ� �޼ҵ� */
	virtual void EntryPatrol() {};
	virtual void EntryAttack() {};
	virtual void EntryTrace() {};
	virtual void EntryDeath() {};
	virtual void Patrol(float DeltaTime) {};
	virtual void Attack(float DeltaTime) {};
	virtual void Trace(float DeltaTime) {};
	virtual void Death(float DeltaTime) {};

	void ChangeState(EEnemyState State);
	bool CheckActorInRange(AActor* TargetActor);
	virtual void ChangeAnimation(FVector Direction) {};
	virtual void ChangeAnimation() {};

	/** �̵� ���� �޼ҵ� */
	void MoveForward(float Value);
	void MoveRight(float Value);

	std::shared_ptr<UPaperSpriteComponent> GetSpriteComponent() { return SpriteComponent; }
	EEnemyState GetCurEnemyState() { return this->CurEnemyState; }
	void SetCurEnemyState(EEnemyState EnemyState) { this->CurEnemyState = EnemyState; }
	void SetSpeed(float InSpeed) { Speed = InSpeed; }
	void SetCurDirection(FVector Direction) { CurDirection = Direction; }
	FVector GetCurDirection() { return CurDirection; }
	FVector GetDirectionToTargetLocation(FVector TargetLocation);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	float Speed = 100.0f;

	EEnemyState CurEnemyState = EEnemyState::NONE;
	EEnemyState PrevEnemyState = EEnemyState::NONE;

	std::vector<FVector> TurningLocations;
	int CurTurningIndex = 0;
	FVector CurDirection = FVector::ZERO;

	float DetectRange = 600.0f;
	float AttackCoolTime = 1.5f;
	float CurAttackCoolTime = 0.0f;
private:

};

