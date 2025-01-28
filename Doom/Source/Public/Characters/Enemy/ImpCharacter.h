#pragma once
#include "Public/Characters/Enemy/EnemyCharacter.h"

/**
 *	����
 */
class AImpCharacter : public AEnemyCharacter
{
public:
	/** ������, �Ҹ��� */
	AImpCharacter();
	~AImpCharacter();

	/** ��ü �� ���� ���� */
	AImpCharacter(const AImpCharacter& Other) = delete;
	AImpCharacter(AImpCharacter&& Other) noexcept = delete;
	AImpCharacter& operator=(const AImpCharacter& Other) = delete;
	AImpCharacter& operator=(AImpCharacter&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};

