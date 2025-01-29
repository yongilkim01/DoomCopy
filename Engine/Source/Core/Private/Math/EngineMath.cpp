#include "Engine.h"
#include "Core/Public/Math/EngineMath.h"

const FIntPoint FIntPoint::LEFT = { -1, 0 };
const FIntPoint FIntPoint::RIGHT = { 1, 0 };
const FIntPoint FIntPoint::UP = { 0, -1 };
const FIntPoint FIntPoint::DOWN = { 0, 1 };


FIntPoint FVector::ConvertToPoint() const
{
	return { iX(), iY() };
}


std::function<bool(const FTransform&, const FTransform&)> FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		// 데이터 영역이 초기화 될때 초기화하는 일을 자동으로 수행할수 있다.
		// 데이터 영역이 만들어질때 이 작업은 자동으로 실행된다.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::CirCle)] = FTransform::RectToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::Rect)] = FTransform::CirCleToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::OBB2D)] = FTransform::OBB2DToOBB2D;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Rect)] = FTransform::OBB2DToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::CirCle)] = FTransform::OBB2DToCircle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Point)] = FTransform::OBB2DToPoint;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::Sphere)] = FTransform::OBBToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::AABB)] = FTransform::OBBToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB)][static_cast<int>(ECollisionType::OBB)] = FTransform::OBBToOBB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::Sphere)] = FTransform::SphereToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::AABB)] = FTransform::SphereToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Sphere)][static_cast<int>(ECollisionType::OBB)] = FTransform::SphereToOBB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::Sphere)] = FTransform::AABBToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::AABB)] = FTransform::AABBToAABB;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::AABB)][static_cast<int>(ECollisionType::OBB)] = FTransform::AABBToOBB;

	}
};

// 데이터 영역
CollisionFunctionInit Inst = CollisionFunctionInit();


void FTransform::TransformUpdate(bool bAbsolute /* = false*/)
{
	ScaleMat.Scale(Scale);
	RotationMat.RotationDeg(Rotation);
	LocationMat.Position(Location);

	FMatrix CheckWorld = ScaleMat * RotationMat * LocationMat;

	if (true == bAbsolute)
	{
		World = CheckWorld;
		LocalWorld = CheckWorld * ParentMat.InverseReturn();
	}
	else
	{
		LocalWorld = CheckWorld;
		World = CheckWorld * RevolveMat * ParentMat;
	}

	Decompose();
}

void FTransform::Decompose()
{
	World.Decompose(WorldScale, WorldQuat, WorldLocation);

	LocalWorld.Decompose(RelativeScale, RelativeQuat, RelativeLocation);

	//Scale = RelativeScale;
	//Quat = RelativeQuat;
	//Rotation = RelativeQuat.QuaternionToEulerDeg();
	//Location = RelativeLocation;
}

bool FTransform::Collision(ECollisionType LeftType, const FTransform& LeftTransform, ECollisionType RightType, const FTransform& RightTransform)
{
	if (nullptr == FTransform::AllCollisionFunction[static_cast<int>(LeftType)][static_cast<int>(RightType)])
	{
		MSGASSERT("아직 구현하지 않은 충돌 타입간의 충돌 체크를 하려고 했습니다");
		return false;
	}

	return FTransform::AllCollisionFunction[static_cast<int>(LeftType)][static_cast<int>(RightType)](LeftTransform, RightTransform);
}

bool FTransform::PointToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return CirCleToCirCle(LeftTrans, _Right);
}

// 점 vs 사각형
bool FTransform::PointToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return RectToRect(LeftTrans, _Right);
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.Sphere.Intersects(RightCol.Sphere);
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{

	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.AABB.Intersects(RightCol.AABB);
}

bool FTransform::RectToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	return CirCleToRect(_Right, _Left);
}


bool FTransform::CirCleToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.Sphere.Intersects(RightCol.AABB);
}

bool FTransform::OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.OBB.Intersects(RightCol.OBB);
}

bool FTransform::OBB2DToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.OBB.Intersects(RightCol.AABB);
}

bool FTransform::OBB2DToCircle(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;

	return LeftCol.OBB.Intersects(RightCol.Sphere);
}

bool FTransform::OBBToSphere(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();

	return LeftCol.OBB.Intersects(RightCol.Sphere);
}

bool FTransform::OBBToOBB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.OBB.Intersects(RightCol.OBB);
}

bool FTransform::OBBToAABB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.OBB.Intersects(RightCol.AABB);
}

bool FTransform::SphereToSphere(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.Sphere);
}

bool FTransform::SphereToOBB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.OBB);
}

bool FTransform::SphereToAABB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.Sphere.Intersects(RightCol.AABB);
}

bool FTransform::AABBToSphere(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.Sphere);
}

bool FTransform::AABBToOBB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.OBB);
}

bool FTransform::AABBToAABB(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();
	return LeftCol.AABB.Intersects(RightCol.AABB);
}

bool FTransform::CapsuleToCapsule(const FTransform& Left, const FTransform& Right)
{
	FCollisionData LeftCol = Left.GetCollisionData();
	FCollisionData RightCol = Right.GetCollisionData();

	BoundingCapsule LeftCapsule = LeftCol.Capsule;
	BoundingCapsule RightCapsule = RightCol.Capsule;

	// 캡슐의 중심선 (P1, P2) 계산
	FVector P1A = LeftCapsule.Center - FVector(0, LeftCapsule.HalfHeight, 0);
	FVector P2A = LeftCapsule.Center + FVector(0, LeftCapsule.HalfHeight, 0);

	FVector P1B = RightCapsule.Center - FVector(0, RightCapsule.HalfHeight, 0);
	FVector P2B = RightCapsule.Center + FVector(0, RightCapsule.HalfHeight, 0);

	// 두 선분 간 최단 거리 계산
	float distanceSquared = FVector::ClosestPointBetweenSegments(P1A, P2A, P1B, P2B).Length();

	// 캡슐 반지름의 합
	float radiusSum = LeftCapsule.Radius + RightCapsule.Radius;

	// 충돌 여부 판정
	return distanceSquared <= (radiusSum * radiusSum);
}

bool FTransform::CapsuleToSphere(const FTransform& Left, const FTransform& Right)
{
	return false;
}

bool FTransform::SphereToCapsule(const FTransform& Left, const FTransform& Right)
{
	return false;
}

bool FTransform::OBB2DToPoint(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();

	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Extents = { 0.0f, 0.0f, 0.0f };

	return LeftCol.OBB.Intersects(RightCol.AABB);
}

template<>
FVector FVector::operator*(const class FMatrix& InMatrixValue) const
{
	FVector Result;

	Result.DirectVector = DirectX::XMVector4Transform(DirectVector, InMatrixValue.DirectMatrix);

	return Result;
}

FMatrix FMatrix::operator*(const FMatrix& InMatrixValue)
{
	FMatrix Result;

	Result.DirectMatrix = DirectX::XMMatrixMultiply(DirectMatrix, InMatrixValue.DirectMatrix);

	return Result;

}

FVector FVector::TransformVector(const FVector& InVector, const FMatrix& InMatrix)
{
	return InVector * InMatrix;
}

FVector FVector::TransformVectorCoord(const TVector& InVector, const FMatrix& InMatrix)
{
	FVector CopyVector = InVector;
	CopyVector.W = 1.0f;

	return CopyVector * InMatrix;
}

FVector FVector::TransformVectorNormal(const TVector& InVector, const FMatrix& InMatrix)
{
	FVector CopyVector = InVector;
	CopyVector.W = 0.0f;

	return CopyVector * InMatrix;
}

template<>
FVector TVector<float>::ClosestPointBetweenSegments(TVector P1, TVector P2, TVector Q1, TVector Q2)
{
	FVector d1 = P2 - P1; // 첫 번째 선분 방향 벡터
	FVector d2 = Q2 - Q1; // 두 번째 선분 방향 벡터
	FVector r = P1 - Q1;  // 시작점 간 차이 벡터

	float a = d1.Dot(d1); // ||d1||^2
	float e = d2.Dot(d2); // ||d2||^2
	float f = d2.Dot(r);

	float s, t;

	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		return P1; // 두 선분이 사실상 점이라면, P1 반환
	}

	if (a <= FLT_EPSILON)
	{
		s = 0.0f;
		t = f / e;
	}
	else
	{
		float c = d1.Dot(r);
		if (e <= FLT_EPSILON)
		{
			t = 0.0f;
			s = c / a;
		}
		else
		{
			float b = d1.Dot(d2);
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				s = (b * f - c * e) / denom;
			}
			else
			{
				s = 0.0f;
			}
		}
	}

	// s, t를 0~1 범위로 클램프
	s = std::clamp(s, 0.0f, 1.0f);
	t = std::clamp(t, 0.0f, 1.0f);

	FVector closestPoint1 = P1 + d1 * s;
	FVector closestPoint2 = Q1 + d2 * t;

	return closestPoint1 - closestPoint2; // 두 점 간 거리 벡터 반환
}

template<>
FQuat TVector<float>::DegAngleToQuaternion()
{
	FQuat Result;
	Result.DirectVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectVector);
	return Result;
}

template<>
FVector& FVector::operator*=(const FMatrix& InMatrix)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, InMatrix.DirectMatrix);

	return *this;
}

FVector FQuat::QuaternionToEulerDeg() const
{
	return QuaternionToEulerRad() * FMath::R2D;
}

FVector FQuat::QuaternionToEulerRad() const
{
	FVector Result;

	float sinrCosp = 2.0f * (W * Z + X * Y);
	float cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);

	Result.Z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = W * X - Y * Z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		Result.X = -(0.5f * FMath::PI);
	}
	else if (pitchTest > asinThreshold)
	{
		Result.X = (0.5f * FMath::PI);
	}
	else
	{
		Result.X = asinf(sinp);
	}
	// 이걸 사용했을대 
	float sinyCosp = 2.0f * (W * Y + X * Z);
	float cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
	Result.Y = atan2f(sinyCosp, cosyCosp);
	// 0, 45 0 => 쿼터니온으로 바꾼다.
	// SetActorRoation(쿼터니온);
	// FQuat GetActorRoation();
	// FQuat => 각도 -180 , 275, - 180
	return Result;
}
