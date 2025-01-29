#include "Engine.h"
#include "Engine/Classes/Components/CapsuleComponent.h"

UCapsuleComponent::UCapsuleComponent()
{
}

UCapsuleComponent::~UCapsuleComponent()
{
}

void UCapsuleComponent::BeginPlay()
{
	UShapeComponent::BeginPlay();
}

bool UCapsuleComponent::MoveComponent(const FVector& Delta, const FVector& NewRotation/* = FVector::ZERO */, bool bSweep/* = true */)
{
	if (false == NewRotation.IsZero())
	{
		SetWorldRotation(NewRotation);
	}

	if (true == bSweep)
	{
		SetWorldLocation(SweepCollision(GetWorldLocation(), Delta));
		return true;
	}
	else
	{
		FVector NewLocation = GetWorldLocation() + Delta;
		SetWorldLocation(NormalComponent(GetWorldLocation(), Delta));

		return true;
	}
}

FVector UCapsuleComponent::SweepCollision(const FVector& Location, const FVector& Delta)
{
	return GEngine->GetPhysicsSubSystem()->SweepCollision(Location, Delta, CapsuleHalfHeight, CapsuleRadius);
}