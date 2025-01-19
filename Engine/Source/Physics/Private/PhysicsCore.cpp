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

FVector UPhysicsCore::SweepComponent(const FVector& Location, const FVector& Delta)
{
		FVector FinalVector = Location + Delta;

		float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

		if (110.0f > Distance)
		{
			float CheckDistance = Distance - 110.0f;
			FinalVector.Y -= CheckDistance;
			return FinalVector;
		}
		else if(110.0f < Distance)
		{
			return FinalVector;
		}

		return Location;
}


