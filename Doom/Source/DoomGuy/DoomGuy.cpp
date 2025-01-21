#include "pch.h"
#include "DoomGuy.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Engine/Classes/GameFramework/Actor.h>

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Core/Public/Misc/DirectoryHelper.h>

#include <Input/EngineInput.h>

#include "Public/Items/Weapons/BaseWeapon.h"

ADoomGuyCharacter::ADoomGuyCharacter()
{
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

	BaseWeaponActor = GetWorld()->SpawnActor<ABaseWeapon>();
	BaseWeaponActor->AttachToActor(this);
	BaseWeaponActor->SetMovingStartLocation(GetActorLocation());
}

void ADoomGuyCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	UEngineDebug::OutPutString("DoomGuy Location : " + GetActorLocation().ToString());


	if (FVector::ZERO == GetVelocity())
	{
		BaseWeaponActor->SetMoving(false);
	}
	else
	{
		BaseWeaponActor->SetMoving(true);
	}

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
	if (UEngineInput::IsDown(VK_LBUTTON))
	{
		BaseWeaponActor->Reload();
	}

	if (UEngineInput::IsDown('Y'))
	{

		bPlayMode = !bPlayMode;

	}

	if (true == bPlayMode)
	{

		FVector PrevMouseLocation = CurMouseLocation;

		CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

		//UEngineDebug::OutPutString("PrevMouseLocation : " + PrevMouseLocation.ToString());
		//UEngineDebug::OutPutString("CurMouseLocation : " + CurMouseLocation.ToString());

		AddActorRotation({ CurMouseLocation.Y - PrevMouseLocation.Y, CurMouseLocation.X - PrevMouseLocation.X, 0.0f });

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