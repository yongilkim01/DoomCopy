#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UPaperSpriteComponent;
class UTimeEventComponent;
class UShapeComponent;

/**
 *	����
 */
class AEnemyCharacter : public AActor
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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

private:

};

