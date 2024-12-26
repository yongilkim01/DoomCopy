#pragma once
#include "Classes/Components/PrimitiveComponent.h"

/**
 *	설명
 */
class UPaperSpriteComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UPaperSpriteComponent();
	ENGINE_API ~UPaperSpriteComponent();

	/** 객체 값 복사 방지 */
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

