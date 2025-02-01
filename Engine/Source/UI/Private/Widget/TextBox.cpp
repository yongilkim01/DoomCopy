#include "Engine.h"
#include "UI/Public/Widget/TextBox.h"

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

void UTextBox::InitText(FVector Size, int Count /* = 10*/)
{
	TextImageVector.reserve(Count);

	// 일시적으로 10개 갯수 제한 지정
	for (size_t i = 0; i < Count; i++)
	{
		CreateRenderAsset();
		//USpriteRenderer* Renderer = CreateDefaultSubObject<USpriteRenderer>();
		//Renderer->SetSprite("HUDText.png", 0);
		//FVector2D TextScale = Renderer->SetSpriteScale(1.0f);
		//Renderer->SetComponentScale(TextScale);
		//Renderer->SetOrder(ERenderOrder::UI);
		//Renderer->SetCameraEffect(false);
		//Renderer->SetComponentLocation(FVector2D::ZERO + (Size * static_cast<float>(i)));
		//TextSprites.push_back(Renderer);
	}

	float Padding = 0.0f;

	switch (Count)
	{
	case 1:
		Padding = 0.0f;
		break;
	case 2:
		Padding = 12.0f;
		break;
	case 3:
		Padding = 24.0f;
		break;
	default:
		break;
	}

	for (size_t i = 0; i < Count; i++)
	{
		//TextImageVector[i]-
	}
}

void UTextBox::ShowText(float Time)
{
}

void UTextBox::Reserve(int Count)
{
}

int UTextBox::ConvertCharInteger(char c)
{
	return 0;
}

void UTextBox::SetTextCount(int Count)
{
}

void UTextBox::SetValue(int StrValue)
{
}

URenderAsset& UTextBox::CreateRenderAsset()
{
	URenderAsset& NewUnit = TextImageVector.emplace_back();
	return NewUnit;
}
void UTextBox::SetFont(std::string_view Value, float Scale, UColor Color, FW1_TEXT_FLAG TextFlag)
{
	std::shared_ptr<UFont> SFont = UFont::Find<UFont>(Value);
	Font = SFont.get();
	Scale = Scale;
	Color = Color;
	Flag = TextFlag;
}
