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

    // ���� ��ġ���� �ٴڱ����� �Ÿ� ���
    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

    // �������� ����� ���� ���� üũ
    std::vector<FVector> OffsetDirections = {
        FVector(InRadius, 0, 0), FVector(-InRadius, 0, 0),
        FVector(0, 0, InRadius), FVector(0, 0, -InRadius)
    };

    FVector AdjustedFinalVector = FinalVector;
    bool bCollisionDetected = false;
    FVector PushVector = FVector::ZERO;
    int SafeDirections = 0;

    for (const FVector& Offset : OffsetDirections)
    {
        FVector CheckVector = FinalVector + Offset;
        float IntersectResult = UNavigationSystem::GetInstance().DistanceToVector(CheckVector);

        if (IntersectResult == 0.0f) // �浹 �߻�
        {
            bCollisionDetected = true;
            PushVector -= Offset; // �浹�� ������ �ݴ�� �о��ֱ�
        }
        else if (IntersectResult >= InHalfHeight) // �̵� ������ ����
        {
            SafeDirections++;
        }
    }

    // �ٴ��� ������ ���, Y ��ġ ���� (���� �ڵ� ����)
    if (InHalfHeight > Distance)
    {
        float CheckDistance = Distance - InHalfHeight;
        AdjustedFinalVector.Y -= CheckDistance;
    }

    if (true == bCollisionDetected)
    {
        if (!PushVector.IsZero())
        {
            PushVector.Normalize();
            AdjustedFinalVector += PushVector * 1.0f; // ����� �о���
        }
        else
        {
            AdjustedFinalVector = Location; // ��� ���� ���������� ���� ��ġ ����
        }
    }
    else
    {
        // ��� ������ �����̸� ���� ��� (Delta ��ü�� �߷��� ����Ǿ� ����)
        if (SafeDirections == 4)
        {
            return AdjustedFinalVector; // �״�� �̵� (����)
        }
    }

    return AdjustedFinalVector;
}


FVector UPhysicsEngine::NormalComponent(const FVector& Location, const FVector& Delta)
{
	FVector FinalVector = Location + Delta;

	float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

	if (0.0f == Distance)
	{
		return Location;
	}

	return FinalVector;
}