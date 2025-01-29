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
//    // 수직 거리 확인
//    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);
//
//    if (Distance == 0.0f)
//    {
//        return Location;
//    }
//
//    // 반지름을 이용한 추가적인 충돌 검사
//    FVector AdjustedFinalVector = FinalVector;
//    bool bCollisionDetected = false;
//    bool bFall = true;
//
//    // 반지름을 고려한 여러 방향 체크
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
//        if (IntersectResult == 0.0f) // 충돌이 발생했을 경우
//        {
//            AdjustedFinalVector = Location; // 충돌 시 원래 위치로 되돌림
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

    // 현재 위치에서 바닥까지의 거리 계산
    float Distance = UNavigationSystem::GetInstance().DistanceToVector(FinalVector);

    if (Distance == 0.0f)
    {
        return Location;
    }

    // 반지름을 고려한 여러 방향 체크
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

        if (IntersectResult == 0.0f) // 충돌이 발생했을 경우
        {
            AdjustedFinalVector = Location; // 충돌 시 원래 위치로 되돌림
            bCollisionDetected = true;
            break;
        }
        else if (IntersectResult >= InHalfHeight)
        {
            SafeDirections++; // 충돌이 감지된 방향
        }
    }

    UEngineDebug::OutPutString(" ");

    if (false == bCollisionDetected)
    {
        // 모든 방향이 공중이면 낙하 허용 (Delta 자체에 중력이 적용되어 있음)
        if (SafeDirections == 4)
        {
            return AdjustedFinalVector; // 그대로 이동 (낙하)
        }

        // 바닥이 감지된 경우, Y 위치 조정 (기존 코드 유지)
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