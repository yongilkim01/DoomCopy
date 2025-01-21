#include "pch.h"
#include "DoomGuy.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Core/Public/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

ADoomGuyCharacter::ADoomGuyCharacter()
{
	SpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	SpriteComponent->SetupAttachment(RootComponent);
	SpriteComponent->SetTexture("Shotgun.png");
	SpriteComponent->SetRelativeLocation({ 0.0f, 24.5f, 5.0f });
	SpriteComponent->SetWorldScale3D({ 5.0f, 5.0f });
	//SpriteComponent->OnBillboard();

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("Player");
	ShapeComponent->SetWorldScale3D({ 30.0f, 30.0f });

	//ShapeComponent->SetCollisionEnter([](UShapeComponent* _This, UShapeComponent* _Other)
	//	{
	//		_Other->GetActor()->Destroy();
	//		// _Other->Destroy();
	//		UEngineDebug::OutPutString("Enter");
	//	});
}

ADoomGuyCharacter::~ADoomGuyCharacter()
{
}

void ADoomGuyCharacter::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();
}

void ADoomGuyCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (UEngineInput::IsPress('A'))
	{
		MoveRight(-Speed);
	}
	if (UEngineInput::IsPress('D'))
	{
		MoveRight(Speed);
	}
	if (UEngineInput::IsPress('W'))
	{
		MoveForward(Speed);
	}
	if (UEngineInput::IsPress('S'))
	{
		MoveForward(-Speed);
	}

	if (UEngineInput::IsDown('Y'))
	{

		bPlayMode = !bPlayMode;

	}

	if (true == bPlayMode)
	{

		FVector PrevMouseLocation = CurMouseLocation;

		CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

		UEngineDebug::OutPutString("PrevMouseLocation : " + PrevMouseLocation.ToString());
		UEngineDebug::OutPutString("CurMouseLocation : " + CurMouseLocation.ToString());

		AddActorRotation({ 0.0f, CurMouseLocation.X - PrevMouseLocation.X, 0.0f });

		//SetCursorPos(960, 540);

		//CurMouseLocation = FVector{ 960, 540 };

	}
}

void ADoomGuyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorForwardVector();

		AddMovementInput(Direction, Value);
	}
}

void ADoomGuyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorRightVector();

		AddMovementInput(Direction, Value);
	}
}