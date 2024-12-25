#include "pch.h"
#include "PaperSpriteComponent.h"

UPaperSpriteComponent::UPaperSpriteComponent()
{
}

UPaperSpriteComponent::~UPaperSpriteComponent()
{
}

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName)
{
    SetTexture(SpriteName);
}

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName, size_t Index)
{
    SetTexture(SpriteName);
    SetSpriteData(Index);
}

