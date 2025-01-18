#include "pch.h"
#include "Physics/Public/PhysicsCore.h"

#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"

#include "NavigationSystem/Public/NavigationSystem.h"

#include "Core/Public/Debugging/DebugMacros.h"

UPhysicsCore::UPhysicsCore()
{
}

UPhysicsCore::~UPhysicsCore()
{
}

void UPhysicsCore::Tick(float DeltaTime)
{
	if (0 == PrimitiveComponentList.size())
	{
		return;
	}

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponentList)
	{
		if (true == PrimitiveComponent->IsGravityEnabled())
		{
			FVector FinalVector = PrimitiveComponent->GetOwner()->GetActorLocation() + PrimitiveComponent->GetOwner()->GetPerformMovement();

			float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

			if (0 != Distance)
			{
				Distance = Distance - 110.0f;
				FinalVector += FVector{ 0.0f, -Distance * 20.0f * DeltaTime, 0.0f };

				PrimitiveComponent->GetOwner()->SetActorLocation(FinalVector);
			}
			
			PrimitiveComponent->GetOwner()->SetPerformMovement(FVector::ZERO);
		}
	}
}


