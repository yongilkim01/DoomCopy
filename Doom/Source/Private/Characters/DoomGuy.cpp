#include "Doom.h"
#include "Public/Characters/DoomGuy.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Engine/Classes/GameFramework/Actor.h>

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Core/Public/Misc/DirectoryHelper.h>

#include <Platform/Public/Input/EngineInput.h>

#include "Public/Items/Weapons/GunWeapon.h"
#include "Public/Items/Weapons/Bullet.h"

ADoomGuyCharacter::ADoomGuyCharacter()
{
	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("PlayerBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
	ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });
}

ADoomGuyCharacter::~ADoomGuyCharacter()
{
}

void ADoomGuyCharacter::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

	GunActor = GetWorld()->SpawnActor<AGunWeapon>();
	GunActor->AttachToActor(this);
}

void ADoomGuyCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (FVector::ZERO == GetVelocity())
	{
		GunActor->SetMoving(false);
	}
	else
	{
		GunActor->SetMoving(true);
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
		GunActor->Reload();
		std::shared_ptr<ABullet> Bullet = GetWorld()->SpawnActor<ABullet>();
		FVector BulletLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f) + (GetActorUpVector() * 30.0f);
		Bullet->SetActorLocation(BulletLocation);
		Bullet->SetBulletDirection(GetActorForwardVector());
	}

	if (UEngineInput::IsDown('Y'))
	{

		bPlayMode = !bPlayMode;

	}

	if (true == bPlayMode)
	{

		FVector PrevMouseLocation = CurMouseLocation;

		CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

		float RotationX = GetActorRotation().X + CurMouseLocation.Y - PrevMouseLocation.Y;
		float RotationY = GetActorRotation().Y + CurMouseLocation.X - PrevMouseLocation.X;

		if (RotationX < 330.0f)
		{
			RotationX = 330.0f;
		}
		else if (RotationX > 400.0f)
		{
			RotationX = 400.0f;
		}

		SetActorRotation({ RotationX, RotationY, 0.0f });
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