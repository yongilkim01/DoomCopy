#include "pch.h"
#include "EngineMath.h"

const double FMath::DPI = 3.14159265358979323846264338327950288419716939937510;
const double FMath::DPI2 = DPI * 2.0;

const float FMath::PI = 3.14159265358979323846264f;
const float FMath::PI2 = PI * 2.0f;

// ��׸��� �������� �ٲٴ� ���� �ȴ�.
const float FMath::D2R = FMath::PI / 180.0f;
const float FMath::R2D = 180.0f / FMath::PI;

const FVector FVector::ZERO = { 0.0f, 0.0f };
const FVector FVector::LEFT = { -1.0f, 0.0f };
const FVector FVector::RIGHT = { 1.0f, 0.0f };
const FVector FVector::UP = { 0.0f, 1.0f };
const FVector FVector::DOWN = { 0.0f, -1.0f };
const FVector FVector::FORWARD = { 0.0f, 0.0f, 1.0f };
const FVector FVector::BACK = { 0.0f, 0.0f , -1.0f };

const FIntPoint FIntPoint::LEFT = { -1, 0 };
const FIntPoint FIntPoint::RIGHT = { 1, 0 };
const FIntPoint FIntPoint::UP = { 0, -1 };
const FIntPoint FIntPoint::DOWN = { 0, 1 };


const UColor UColor::WHITE = { 255, 255, 255, 0 };
const UColor UColor::BLACK = { 0, 0, 0, 0 };

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
		// ������ ������ �ʱ�ȭ �ɶ� �ʱ�ȭ�ϴ� ���� �ڵ����� �����Ҽ� �ִ�.
		// ������ ������ ��������� �� �۾��� �ڵ����� ����ȴ�.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::CirCle)] = FTransform::RectToCirCle;

		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::Rect)] = FTransform::CirCleToRect;

	}
};

// ������ ����
CollisionFunctionInit Inst = CollisionFunctionInit();


void FTransform::TransformUpdate()
{
	ScaleMat.Scale(Scale);
	RotationMat.RotationDeg(Rotation);
	LocationMat.Position(Location);

	World = ScaleMat * RotationMat * LocationMat;
}

bool FTransform::Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right)
{
	return FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)](_Left, _Right);
}

bool FTransform::PointToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return CirCleToCirCle(LeftTrans, _Right);
}

// �� vs �簢��
bool FTransform::PointToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return RectToRect(LeftTrans, _Right);
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FVector Len = _Left.Location - _Right.Location;

	// Ʈ�������� ������ ������ �������� x�� ����ũ�⸦ ���������� ���ڽ��ϴ�.
	// �ο��� �������� ���� ������ ���̺��� ũ�ٸ� 
	if (Len.Length() < _Left.Scale.hX() + _Right.Scale.hX())
	{
		return true;
	}

	return false;
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{

	if (_Left.ZAxisCenterLeft() > _Right.ZAxisCenterRight())
	{
		return false;
	}

	if (_Left.ZAxisCenterRight() < _Right.ZAxisCenterLeft())
	{
		return false;
	}

	if (_Left.ZAxisCenterTop() > _Right.ZAxisCenterBottom())
	{
		return false;
	}

	if (_Left.ZAxisCenterBottom() < _Right.ZAxisCenterTop())
	{
		return false;
	}
	// ���� ����� �ȴ�.
	return true;
}

bool FTransform::RectToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	return CirCleToRect(_Right, _Left);
}


bool FTransform::CirCleToRect(const FTransform& _Left, const FTransform& _Right)
{
	// �¿�� ������ Ȯ���� Ʈ������
	FTransform WTransform = _Right;
	WTransform.Scale.X += _Left.Scale.X;

	// ���Ʒ��� ������ ��ŭ Ȯ���� Ʈ������
	FTransform HTransform = _Right;
	HTransform.Scale.Y += _Left.Scale.X;

	if (true == PointToRect(_Left, WTransform) || true == PointToRect(_Left, HTransform))
	{
		return true;
	}

	// ��� ������ ���ؼ� static���� ����� ����� �ִµ�.
	// static FVector ArrPoint[4];
	// �����忡���� ������.
	FVector ArrPoint[4];

	ArrPoint[0] = _Right.ZAxisCenterLeftTop();
	ArrPoint[1] = _Right.ZAxisCenterLeftBottom();
	ArrPoint[2] = _Right.ZAxisCenterRightTop();
	ArrPoint[3] = _Right.ZAxisCenterRightBottom();

	FTransform PointCirCle;
	PointCirCle.Scale = _Left.Scale;
	for (size_t i = 0; i < 4; i++)
	{
		PointCirCle.Location = ArrPoint[i];
		if (true == PointToCirCle(_Left, PointCirCle))
		{
			return true;
		}
	}

	return false;
}

FVector FVector::TransformVector(const FVector& InVector, const class FMatrix& InMatrix)
{
	return InVector * InMatrix;
}

FVector FVector::TransformVectorCoord(const FVector& InVector, const class FMatrix& InMatrix)
{
	FVector Copy = InVector;
	Copy.W = 1.0f;
	return Copy * InMatrix;
}
FVector FVector::TransformVectorNormal(const FVector& InVector, const class FMatrix& InMatrix)
{
	FVector Copy = InVector;
	Copy.W = 0.0f;
	return Copy * InMatrix;
}

FVector FVector::operator*(const class FMatrix& InMatrixValue) const
{
	FVector Result;

	Result.X = Arr2D[0][0] * InMatrixValue.Arr2D[0][0] + Arr2D[0][1] * InMatrixValue.Arr2D[1][0] + Arr2D[0][2] * InMatrixValue.Arr2D[2][0] + Arr2D[0][3] * InMatrixValue.Arr2D[3][0];
	Result.Y = Arr2D[0][0] * InMatrixValue.Arr2D[0][1] + Arr2D[0][1] * InMatrixValue.Arr2D[1][1] + Arr2D[0][2] * InMatrixValue.Arr2D[2][1] + Arr2D[0][3] * InMatrixValue.Arr2D[3][1];
	Result.Z = Arr2D[0][0] * InMatrixValue.Arr2D[0][2] + Arr2D[0][1] * InMatrixValue.Arr2D[1][2] + Arr2D[0][2] * InMatrixValue.Arr2D[2][2] + Arr2D[0][3] * InMatrixValue.Arr2D[3][2];
	Result.W = Arr2D[0][0] * InMatrixValue.Arr2D[0][3] + Arr2D[0][1] * InMatrixValue.Arr2D[1][3] + Arr2D[0][2] * InMatrixValue.Arr2D[2][3] + Arr2D[0][3] * InMatrixValue.Arr2D[3][3];

	return Result;
}

FVector& FVector::operator*=(const FMatrix& InMatrixValue)
{
	FVector Copy = *this;

	this->X = Copy.X * InMatrixValue.Arr2D[0][0] + Copy.Y * InMatrixValue.Arr2D[1][0] + Copy.Z * InMatrixValue.Arr2D[2][0] + Copy.W * InMatrixValue.Arr2D[3][0];
	this->Y = Copy.X * InMatrixValue.Arr2D[0][1] + Copy.Y * InMatrixValue.Arr2D[1][1] + Copy.Z * InMatrixValue.Arr2D[2][1] + Copy.W * InMatrixValue.Arr2D[3][1];
	this->Z = Copy.X * InMatrixValue.Arr2D[0][2] + Copy.Y * InMatrixValue.Arr2D[1][2] + Copy.Z * InMatrixValue.Arr2D[2][2] + Copy.W * InMatrixValue.Arr2D[3][2];
	this->W = Copy.X * InMatrixValue.Arr2D[0][3] + Copy.Y * InMatrixValue.Arr2D[1][3] + Copy.Z * InMatrixValue.Arr2D[2][3] + Copy.W * InMatrixValue.Arr2D[3][3];


	return *this;
}

FMatrix FMatrix::operator*(const FMatrix& InMatrixValue)
{
	FMatrix Result;

	Result.Arr2D[0][0] = Arr2D[0][0] * InMatrixValue.Arr2D[0][0] + Arr2D[0][1] * InMatrixValue.Arr2D[1][0] + Arr2D[0][2] * InMatrixValue.Arr2D[2][0] + Arr2D[0][3] * InMatrixValue.Arr2D[3][0];
	Result.Arr2D[0][1] = Arr2D[0][0] * InMatrixValue.Arr2D[0][1] + Arr2D[0][1] * InMatrixValue.Arr2D[1][1] + Arr2D[0][2] * InMatrixValue.Arr2D[2][1] + Arr2D[0][3] * InMatrixValue.Arr2D[3][1];
	Result.Arr2D[0][2] = Arr2D[0][0] * InMatrixValue.Arr2D[0][2] + Arr2D[0][1] * InMatrixValue.Arr2D[1][2] + Arr2D[0][2] * InMatrixValue.Arr2D[2][2] + Arr2D[0][3] * InMatrixValue.Arr2D[3][2];
	Result.Arr2D[0][3] = Arr2D[0][0] * InMatrixValue.Arr2D[0][3] + Arr2D[0][1] * InMatrixValue.Arr2D[1][3] + Arr2D[0][2] * InMatrixValue.Arr2D[2][3] + Arr2D[0][3] * InMatrixValue.Arr2D[3][3];

	Result.Arr2D[1][0] = Arr2D[1][0] * InMatrixValue.Arr2D[0][0] + Arr2D[1][1] * InMatrixValue.Arr2D[1][0] + Arr2D[1][2] * InMatrixValue.Arr2D[2][0] + Arr2D[1][3] * InMatrixValue.Arr2D[3][0];
	Result.Arr2D[1][1] = Arr2D[1][0] * InMatrixValue.Arr2D[0][1] + Arr2D[1][1] * InMatrixValue.Arr2D[1][1] + Arr2D[1][2] * InMatrixValue.Arr2D[2][1] + Arr2D[1][3] * InMatrixValue.Arr2D[3][1];
	Result.Arr2D[1][2] = Arr2D[1][0] * InMatrixValue.Arr2D[0][2] + Arr2D[1][1] * InMatrixValue.Arr2D[1][2] + Arr2D[1][2] * InMatrixValue.Arr2D[2][2] + Arr2D[1][3] * InMatrixValue.Arr2D[3][2];
	Result.Arr2D[1][3] = Arr2D[1][0] * InMatrixValue.Arr2D[0][3] + Arr2D[1][1] * InMatrixValue.Arr2D[1][3] + Arr2D[1][2] * InMatrixValue.Arr2D[2][3] + Arr2D[1][3] * InMatrixValue.Arr2D[3][3];

	Result.Arr2D[2][0] = Arr2D[2][0] * InMatrixValue.Arr2D[0][0] + Arr2D[2][1] * InMatrixValue.Arr2D[1][0] + Arr2D[2][2] * InMatrixValue.Arr2D[2][0] + Arr2D[2][3] * InMatrixValue.Arr2D[3][0];
	Result.Arr2D[2][1] = Arr2D[2][0] * InMatrixValue.Arr2D[0][1] + Arr2D[2][1] * InMatrixValue.Arr2D[1][1] + Arr2D[2][2] * InMatrixValue.Arr2D[2][1] + Arr2D[2][3] * InMatrixValue.Arr2D[3][1];
	Result.Arr2D[2][2] = Arr2D[2][0] * InMatrixValue.Arr2D[0][2] + Arr2D[2][1] * InMatrixValue.Arr2D[1][2] + Arr2D[2][2] * InMatrixValue.Arr2D[2][2] + Arr2D[2][3] * InMatrixValue.Arr2D[3][2];
	Result.Arr2D[2][3] = Arr2D[2][0] * InMatrixValue.Arr2D[0][3] + Arr2D[2][1] * InMatrixValue.Arr2D[1][3] + Arr2D[2][2] * InMatrixValue.Arr2D[2][3] + Arr2D[2][3] * InMatrixValue.Arr2D[3][3];

	Result.Arr2D[3][0] = Arr2D[3][0] * InMatrixValue.Arr2D[0][0] + Arr2D[3][1] * InMatrixValue.Arr2D[1][0] + Arr2D[3][2] * InMatrixValue.Arr2D[2][0] + Arr2D[3][3] * InMatrixValue.Arr2D[3][0];
	Result.Arr2D[3][1] = Arr2D[3][0] * InMatrixValue.Arr2D[0][1] + Arr2D[3][1] * InMatrixValue.Arr2D[1][1] + Arr2D[3][2] * InMatrixValue.Arr2D[2][1] + Arr2D[3][3] * InMatrixValue.Arr2D[3][1];
	Result.Arr2D[3][2] = Arr2D[3][0] * InMatrixValue.Arr2D[0][2] + Arr2D[3][1] * InMatrixValue.Arr2D[1][2] + Arr2D[3][2] * InMatrixValue.Arr2D[2][2] + Arr2D[3][3] * InMatrixValue.Arr2D[3][2];
	Result.Arr2D[3][3] = Arr2D[3][0] * InMatrixValue.Arr2D[0][3] + Arr2D[3][1] * InMatrixValue.Arr2D[1][3] + Arr2D[3][2] * InMatrixValue.Arr2D[2][3] + Arr2D[3][3] * InMatrixValue.Arr2D[3][3];

	return Result;

}