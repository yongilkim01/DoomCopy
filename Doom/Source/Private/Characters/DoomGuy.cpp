#include "Doom.h"
#include "Public/Characters/DoomGuy.h"

#include <Engine/Classes/Camera/CameraActor.h>
#include <Engine/Classes/Camera/CameraComponent.h>

#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>

#include <Platform/Public/Input/EngineInput.h>

#include "Public/Items/Weapons/GunWeapon.h"
#include "Public/Items/Weapons/PistolWeapon.h"
#include "Public/Items/Weapons/ShotgunWeapon.h"
#include "Public/Items/Weapons/Bullet.h"

#include "Public/Global/GameInstance/DoomGameInstance.h"

ADoomGuyCharacter::ADoomGuyCharacter()
{
	AimingPointSpriteComponent = CreateDefaultSubObject<UPaperSpriteComponent>();
	AimingPointSpriteComponent->SetupAttachment(RootComponent);
	AimingPointSpriteComponent->SetTexture("AimingPoint.png");
	AimingPointSpriteComponent->SetRelativeLocation({ 0.0f, 30.0f, 20.0f });
	AimingPointSpriteComponent->SetRelativeScale3D({ 1.0f, 1.0f });
	AimingPointSpriteComponent->SetAutoScale(false);

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("PlayerBody");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
	ShapeComponent->SetRelativeScale3D({ 30.0f, 30.0f, 30.0f });

	CapsuleComponent->SetCapsuleSize(27.0f, 25.0f);
}

ADoomGuyCharacter::~ADoomGuyCharacter()
{
}

void ADoomGuyCharacter::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();
	Camera->AddActorRelativeLocation({ 0.0f, 30.0f, -10.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
	Camera->AttachToActor(this);

	CurMouseLocation = UGameEngine::GetMainWindow().GetMousePos();

	PistolActor = GetWorld()->SpawnActor<APistolWeapon>();
	ShotgunActor = GetWorld()->SpawnActor<AShotgunWeapon>();

	PistolActor->AttachToActor(this);
	ShotgunActor->AttachToActor(this);

	ShotgunActor->SetActive(false);

	GunActor = PistolActor.get();

	SetActorLocation(FVector{ -1042.0f, 14.0f, 3548.0f });
	AddActorRotation(FVector{ 0.0f, 180.0f, 0.0f });
	//SetActorLocation(FVector{ -1755.0f, 14.0f, 2497.0f });
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
		GunActor->FireGun();
	}
	if (UEngineInput::IsDown('Y'))
	{
		bPlayMode = !bPlayMode;
	}
	if (UEngineInput::IsDown('2'))
	{
		ChangeWeapon(2);
	}
	if (UEngineInput::IsDown('3'))
	{
		ChangeWeapon(3);
	}
	if (UEngineInput::IsDown(VK_UP))
	{
		GetGameInstance<UDoomGameInstance>()->AddDoomGuyHealth(1);
	}
	if (UEngineInput::IsDown(VK_DOWN))
	{
		GetGameInstance<UDoomGameInstance>()->AddDoomGuyHealth(-1);
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

void ADoomGuyCharacter::ChangeWeapon(int WeaponIndex)
{
	int DoomGuyWeaponCount = GetGameInstance<UDoomGameInstance>()->GetDoomGuyWeaponCount();

	if (DoomGuyWeaponCount + 1 < WeaponIndex)
	{
		return;
	}

	if (2 == WeaponIndex)
	{
		ShotgunActor->SetActive(false);
		PistolActor->SetActive(true);

		GunActor = PistolActor.get();
	}
	else if (3 == WeaponIndex)
	{
		ShotgunActor->SetActive(true);
		PistolActor->SetActive(false);

		GunActor = ShotgunActor.get();
	}
}
