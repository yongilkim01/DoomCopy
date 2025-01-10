#pragma once
#include <GameFramework/Actor.h>

class UPaperSpriteComponent;
class UTimeEventComponent;
class UShapeComponent;

/**
 *	설명
 */
class UEnemyCharacter : public AActor
{
public:
	/** 생성자, 소멸자 */
	UEnemyCharacter();
	~UEnemyCharacter();

	/** 객체 값 복사 방지 */
	UEnemyCharacter(const UEnemyCharacter& Other) = delete;
	UEnemyCharacter(UEnemyCharacter&& Other) noexcept = delete;
	UEnemyCharacter& operator=(const UEnemyCharacter& Other) = delete;
	UEnemyCharacter& operator=(UEnemyCharacter&& Other) noexcept = delete;

	std::shared_ptr<UPaperSpriteComponent> GetSpriteComponent()
	{
		return SpriteComponent;
	}

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UTimeEventComponent> TimeEventComponent = nullptr;
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

};

