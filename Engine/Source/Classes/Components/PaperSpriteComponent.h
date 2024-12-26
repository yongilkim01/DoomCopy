#pragma once
#include "Classes/Components/PrimitiveComponent.h"

/**
 *	����
 */
class UPaperSpriteComponent : public UPrimitiveComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UPaperSpriteComponent();
	ENGINE_API ~UPaperSpriteComponent();

	/** ��ü �� ���� ���� */
	UPaperSpriteComponent(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent(UPaperSpriteComponent&& Other) noexcept = delete;
	UPaperSpriteComponent& operator=(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent& operator=(UPaperSpriteComponent&& Other) noexcept = delete;

	ENGINE_API void SetSprite(std::string_view SpriteName);
	ENGINE_API void SetSprite(std::string_view SpriteName, size_t Index);

protected:
	/** USceneComponent method */
	void BeginPlay() override;

private:

};

