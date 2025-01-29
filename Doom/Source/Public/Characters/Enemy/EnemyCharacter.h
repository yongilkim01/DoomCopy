#pragma once
#include <Engine/Classes/GameFramework/Character.h>

class UPaperSpriteComponent;
class UTimeEventComponent;
class UShapeComponent;

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

