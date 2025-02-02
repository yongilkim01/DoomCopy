#pragma once
#include <Engine/Classes/GameFramework/Actor.h>
#include "Public/Global/DoomEnums.h"

class ADoomGuyCharacter;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	설명
 */
class ADrumContainer : public AActor
{
public:
	/** 생성자, 소멸자 */
	ADrumContainer();
	~ADrumContainer();

	/** 객체 값 복사 방지 */
	ADrumContainer(const ADrumContainer& _Other) = delete;
	ADrumContainer(ADrumContainer&& _Other) noexcept = delete;
	ADrumContainer& operator=(const ADrumContainer& _Other) = delete;
	ADrumContainer& operator=(ADrumContainer&& _Other) noexcept = delete;

	/** 액터 상속 메소드 */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Bomb();


protected:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
	std::shared_ptr<UShapeComponent> AttackShapeComponent = nullptr;

private:

};

