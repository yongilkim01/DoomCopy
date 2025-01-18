#include "pch.h"
#include "Engine/Classes/GameFramework/Character.h"

#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

ACharacter::ACharacter()
{
	CharacterMovement = CreateDefaultSubObject<UCharacterMovementComponent>();
    RootComponent = CreateDefaultSubObject<UCapsuleComponent>();
    CharacterMovement->UpdatedComponent = dynamic_cast<UPrimitiveComponent*>(RootComponent.get());
}

ACharacter::~ACharacter()
{

}

void ACharacter::Tick(float DeltaTime)
{
    APawn::Tick(DeltaTime);

    if (nullptr != CharacterMovement)
    {
        CharacterMovement->PerformMovement(DeltaTime);
    }
}

void ACharacter::AddMovementInput(const FVector& WorldDirection, float ScaleValue, bool bForce  /*= false*/)
{
    if (nullptr != CharacterMovement && 0.0f != ScaleValue) // TODO: Controller nullptr üũ �ڵ� �߰�
    {
        GetCharacterMovement()->AddInputVector(WorldDirection * ScaleValue, bForce);
    }
}

void ACharacter::LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ)
{
    if (nullptr != CharacterMovement)
    {
        FVector NewVelocity = CharacterMovement->Velocity;

        // Z�� �ӵ��� ���� �ӵ��� �����
        if (bOverrideZ)
        {
            NewVelocity.Z = LaunchVelocity.Z;
        }

        CharacterMovement->Velocity = NewVelocity;
    }
}

void ACharacter::CheckJumpInput(float DeltaTime)
{
	if (true == CanJump())
	{
		JumpCurrentCount++;
        LaunchCharacter(FVector(0.0f, 0.0f, GetCharacterMovement()->JumpZVelocity), false, true);
	}
}

bool ACharacter::CanJump() const
{
	return false;
}
