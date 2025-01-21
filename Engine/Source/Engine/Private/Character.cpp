#include "pch.h"
#include "Engine/Classes/GameFramework/Character.h"

#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

ACharacter::ACharacter()
{
    std::shared_ptr<UCapsuleComponent> CapsuleComponent = CreateDefaultSubObject<UCapsuleComponent>();
    RootComponent = CapsuleComponent;

	CharacterMovement = CreateDefaultSubObject<UCharacterMovementComponent>();
    CharacterMovement->UpdatedComponent = CapsuleComponent.get();
}

ACharacter::~ACharacter()
{

}

void ACharacter::Tick(float DeltaTime)
{
    APawn::Tick(DeltaTime);

}

void ACharacter::AddMovementInput(const FVector& WorldDirection, float ScaleValue, bool bForce  /*= false*/)
{
    if (nullptr != CharacterMovement && 0.0f != ScaleValue) // TODO: Controller nullptr 체크 코드 추가
    {
        GetCharacterMovement()->AddInputVector(WorldDirection * ScaleValue, bForce);
    }
}

void ACharacter::LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ)
{
    if (nullptr != CharacterMovement)
    {
        FVector NewVelocity = CharacterMovement->Velocity;

        // Z축 속도를 점프 속도로 덮어씌움
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

FVector ACharacter::GetVelocity() const
{
    return CharacterMovement->Velocity;
}
