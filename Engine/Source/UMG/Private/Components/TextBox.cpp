#include "pch.h"
#include "UMG/Public/Components/TextBox.h"

#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Camera/CameraActor.h"

UTextBox::UTextBox()
{
}

UTextBox::~UTextBox()
{
}

void UTextBox::Tick(float DeltaTime)
{
	UWidget::Tick(DeltaTime);
}

void UTextBox::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UWidget::Render(CameraComponent, DeltaTime);

	ACameraActor* CameraActor = CameraComponent->GetOwner<ACameraActor>();

	FVector ScreenLocation = CameraActor->WorldLocationToScreenLocation(GetWorldLocation());

	Font->DrawFont(Text.c_str(), Scale, ScreenLocation, Color, Flag);
}

void UTextBox::SetFont(std::string_view Value, float Scale, UColor Color, FW1_TEXT_FLAG TextFlag)
{
	std::shared_ptr<UFont> SFont = UFont::Find<UFont>(Value);
	Font = SFont.get();
	Scale = Scale;
	Color = Color;
	Flag = TextFlag;
}
