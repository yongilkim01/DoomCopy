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

//FVector UPhysicsEngine::SweepCollision(const FVector& Location, const FVector& Delta, float InHalfHeight, float InRadius)
//{
//    FVector FinalVector = Location + Delta;
//
//    // ���� �Ÿ� Ȯ��
//    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);
//
//    if (Distance == 0.0f)
//    {
//        return Location;
//    }
//
//    // �������� �̿��� �߰����� �浹 �˻�
//    FVector AdjustedFinalVector = FinalVector;
//    bool bCollisionDetected = false;
//    bool bFall = true;
//
//    // �������� ����� ���� ���� üũ
//    std::vector<FVector> OffsetDirections = {
//        FVector(InRadius, 0, 0), FVector(-InRadius, 0, 0),
//        FVector(0, 0, InRadius), FVector(0, 0, -InRadius)
//    };
//
//    for (const FVector& Offset : OffsetDirections)
//    {
//        FVector CheckVector = FinalVector + Offset;
//        float IntersectResult = UNavigationSystem::GetInstance().DistanceToVector(CheckVector);
//
//        if (IntersectResult == 0.0f) // �浹�� �߻����� ���
//        {
//            AdjustedFinalVector = Location; // �浹 �� ���� ��ġ�� �ǵ���
//            bCollisionDetected = true;
//            break;
//        }
//    }
//
//    if (false == bCollisionDetected)
//    {
//        if (InHalfHeight > Distance)
//        {
//            float CheckDistance = Distance - InHalfHeight;
//            AdjustedFinalVector.Y -= CheckDistance;
//        }
//    }
//
//    return AdjustedFinalVector;
//}

FVector UPhysicsEngine::SweepCollision(const FVector& Location, const FVector& Delta, float InHalfHeight, float InRadius)
{
    FVector FinalVector = Location + Delta;

    // ���� ��ġ���� �ٴڱ����� �Ÿ� ���
    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

    if (Distance == 0.0f)
    {
        return Location;
    }

    // �������� ����� ���� ���� üũ
    std::vector<FVector> OffsetDirections = {
        FVector(InRadius, 0, 0), FVector(-InRadius, 0, 0),
        FVector(0, 0, InRadius), FVector(0, 0, -InRadius)
    };

    FVector AdjustedFinalVector = FinalVector;
    bool bCollisionDetected = false;
    int SafeDirections = 0;

    for (const FVector& Offset : OffsetDirections)
    {
        FVector CheckVector = FinalVector + Offset;
        float IntersectResult = UNavigationSystem::GetInstance().DistanceToVector(CheckVector);

        UEngineDebug::OutPutString("Distance : " + std::to_string(IntersectResult));

        if (IntersectResult == 0.0f) // �浹�� �߻����� ���
        {
            AdjustedFinalVector = Location; // �浹 �� ���� ��ġ�� �ǵ���
            bCollisionDetected = true;
            break;
        }
        else if (IntersectResult >= InHalfHeight)
        {
            SafeDirections++; // �浹�� ������ ����
        }
    }

    UEngineDebug::OutPutString(" ");

    if (false == bCollisionDetected)
    {
        // ��� ������ �����̸� ���� ��� (Delta ��ü�� �߷��� ����Ǿ� ����)
        if (SafeDirections == 4)
        {
            return AdjustedFinalVector; // �״�� �̵� (����)
        }

        // �ٴ��� ������ ���, Y ��ġ ���� (���� �ڵ� ����)
        if (InHalfHeight > Distance)
        {
            float CheckDistance = Distance - InHalfHeight;
            AdjustedFinalVector.Y -= CheckDistance;
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

	return Location;
}