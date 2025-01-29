#include "Engine.h"
#include "Physics/Public/PhysicsEngine.h"

#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"

#include "NavigationSystem/Public/NavigationSystem.h"

#include "Core/Public/Debugging/DebugMacros.h"

UPhysicsEngine::UPhysicsEngine()
{
}

UPhysicsEngine::~UPhysicsEngine()
{
}

FVector UPhysicsEngine::SweepCollision(const FVector& Location, const FVector& Delta)
{
		FVector FinalVector = Location + Delta;

		float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);
		
		if (0.0f == Distance)
		{
			return Location;
		}
		else if (25.0f > Distance)
		{
			float CheckDistance = Distance - 25.0f;
			FinalVector.Y -= CheckDistance;

			return FinalVector;
		}
		else if(25.0f < Distance)
		{
			return FinalVector;
		}

		return Location;
}

FVector UPhysicsEngine::SweepCollision(const FVector& Location, const FVector& Delta, float InHalfHeight, float InRadius)
{
	FVector FinalVector = Location + Delta;

	float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

	if (0.0f == Distance)
	{
		return Location;
	}
	else if (InHalfHeight > Distance)
	{
		float CheckDistance = Distance - InHalfHeight;
		FinalVector.Y -= CheckDistance;

		return FinalVector;
	}
	else if (InHalfHeight < Distance)
	{
		return FinalVector;
	}

	return Location;
}

FVector UPhysicsEngine::NormalComponent(const FVector& Location, const FVector& Delta)
{
	FVector FinalVector = Location + Delta;

	float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

	if (0.0f == Distance)
	{
		return Location;
	}

	return Location;
}