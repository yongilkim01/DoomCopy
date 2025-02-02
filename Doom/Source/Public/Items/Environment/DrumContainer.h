#pragma once
#include <Engine/Classes/GameFramework/Actor.h>
#include "Public/Global/DoomEnums.h"

class ADoomGuyCharacter;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	����
 */
class ADrumContainer : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADrumContainer();
	~ADrumContainer();

	/** ��ü �� ���� ���� */
	ADrumContainer(const ADrumContainer& _Other) = delete;
	ADrumContainer(ADrumContainer&& _Other) noexcept = delete;
	ADrumContainer& operator=(const ADrumContainer& _Other) = delete;
	ADrumContainer& operator=(ADrumContainer&& _Other) noexcept = delete;

	/** ���� ��� �޼ҵ� */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Bomb();


protected:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
	std::shared_ptr<UShapeComponent> AttackShapeComponent = nullptr;

private:

};

