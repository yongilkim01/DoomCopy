#include "Doom.h"
#include "Public/Characters/Enemy/EnemyCharacter.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

AEnemyCharacter::AEnemyCharacter()
{
}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::BeginPlay()
{
	ACharacter::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

}

void AEnemyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorForwardVector();

		AddMovementInput(Direction, Value);
	}
}

void AEnemyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FVector Direction = GetActorRightVector();

		AddMovementInput(Direction, Value);
	}
}