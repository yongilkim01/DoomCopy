#pragma once
#include <Engine/Classes/GameFramework/Character.h>

class UPaperSpriteComponent;
class UTimeEventComponent;
class UShapeComponent;

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

	std::shared_ptr<UPaperSpriteComponent> GetSpriteComponent()
	{
		return SpriteComponent;
	}

	void MoveForward(float Value);
	void MoveRight(float Value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	float Speed = 100.0f;

private:

};

