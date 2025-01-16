#include "pch.h"
#include "UMG/Public/Components/TextBox.h"

UTextBox::UTextBox()
{
}

UTextBox::~UTextBox()
{
}

void UTextBox::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	Font->DrawFont(Text.c_str(), Scale, GetWorldLocation(), Color, Flag);
}

void UTextBox::SetFont(std::string_view Value, float Scale, UColor Color, FW1_TEXT_FLAG TextFlag)
{
	std::shared_ptr<UFont> SFont = UFont::Find<UFont>(Value);
	Font = SFont.get();
	Scale = Scale;
	Color = Color;
	Flag = TextFlag;
}
