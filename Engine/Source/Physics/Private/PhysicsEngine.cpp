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

    // 현재 위치에서 바닥까지의 거리 계산
    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

    // 반지름을 고려한 여러 방향 체크
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

        if (IntersectResult == 0.0f) // 충돌 발생
        {
            bCollisionDetected = true;
            PushVector -= Offset; // 충돌한 방향을 반대로 밀어주기
        }
        else if (IntersectResult >= InHalfHeight) // 이동 가능한 방향
        {
            SafeDirections++;
        }
    }

    // 바닥이 감지된 경우, Y 위치 조정 (기존 코드 유지)
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
            AdjustedFinalVector += PushVector * 1.0f; // 충분히 밀어줌
        }
        else
        {
            AdjustedFinalVector = Location; // 모든 방향 막혀있으면 원래 위치 유지
        }
    }
    else
    {
        // 모든 방향이 공중이면 낙하 허용 (Delta 자체에 중력이 적용되어 있음)
        if (SafeDirections == 4)
        {
            return AdjustedFinalVector; // 그대로 이동 (낙하)
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