#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	설명
 */
class AImpCharacter : public AEnemyCharacter
{
public:
	/** 생성자, 소멸자 */
	AImpCharacter();
	~AImpCharacter();

	/** 객체 값 복사 방지 */
	AImpCharacter(const AImpCharacter& Other) = delete;
	AImpCharacter(AImpCharacter&& Other) noexcept = delete;
	AImpCharacter& operator=(const AImpCharacter& Other) = delete;
	AImpCharacter& operator=(AImpCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

