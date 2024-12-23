#pragma once
#include <Windows.h>
#include <string>
#include <functional>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "EngineDefine.h"

enum class ECollisionType
{
	Point,
	Rect,
	CirCle,
	Max
};

class ENGINE_API FMath
{
public:
	// ��� ����
	static const double DPI;
	static const double DPI2;

	static const float PI;
	static const float PI2;

	static const float D2R;
	static const float R2D;

	static float Sqrt(float Value)
	{
		return ::sqrtf(Value);
	}

	template <typename DataType>
	DataType ClampMax(DataType Value, DataType MaxValue)
	{
		return (Value > MaxValue) ? MaxValue : Value;
	}

	template <typename DataType>
	DataType ClampMin(DataType Value, DataType MinValue)
	{
		return (Value < MinValue) ? MinValue : Value;
	}

	template <typename DataType>
	static DataType Clamp(DataType Value, DataType MinValue, DataType MaxValue)
	{
		if (Value < MinValue)
		{
			return MinValue;
		}
		else if (Value > MaxValue)
		{
			return MaxValue;
		}
		else
		{
			return Value;
		}
	}

	template <typename DataType>
	static DataType Lerp(DataType A, DataType B, DataType Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}
};

class FVector
{
public:
	static const FVector ZERO;
	static const FVector LEFT;
	static const FVector RIGHT;
	static const FVector UP;
	static const FVector DOWN;
	static const FVector FORWARD;
	static const FVector BACK;

public:
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};

		float Arr2D[1][4];
		float Arr1D[4];

		/** DirectX Simd ���� ���� ���� */
		DirectX::XMVECTOR DirectVector;
	};

	ENGINE_API FVector()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}
	ENGINE_API FVector(float InX, float InY)
		: X(InX), Y(InY), Z(0.0f), W(1.0f)
	{

	}
	ENGINE_API FVector(float InX, float InY, float InZ)
		: X(InX), Y(InY), Z(InZ), W(1.0f)
	{

	}
	FVector(float InX, float InY, float InZ, float InW)
		: X(InX), Y(InY), Z(InZ), W(InW)
	{

	}
	FVector(int InX, int InY)
		: X(static_cast<float>(InX)), Y(static_cast<float>(InY)), Z(0.0f), W(1.0f)
	{

	}
	FVector(long InX, long InY)
		: X(static_cast<float>(InX)), Y(static_cast<float>(InY)), Z(0.0f), W(1.0f)
	{

	}
	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	/**
	 *   X �Ǵ� Y�� 0�̸� true�� ��ȯ�ϴ� �Լ�
	 *
	 *   @return X �Ǵ� Y�� 0�̸� true, �׷��� ������ false
	 */
	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f;
	}
	/**
	 *   ������ �� ��Ҹ� �������� ���� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @return �� ��Ұ� ������ ����
	 */
	FVector Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}
	/**
	 *   ������ ����(������ ����)�� ����ϴ� �Լ�
	 *
	 *   @return ������ ����
	 */
	float Length() const
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z);
	}
	/**
	 *   ���͸� ������ ��ǥ���� POINT�� ��ȯ�ϴ� �Լ�
	 *
	 *   @return ��ȯ�� POINT
	 */
	POINT ConvertWindowPOINT()
	{
		return { iX(), iY() };
	}
	/**
	 *   ���͸� ���� ��ǥ���� FIntPoint�� ��ȯ�ϴ� �Լ�
	 *
	 *   @return ��ȯ�� FIntPoint
	 */
	class FIntPoint ConvertToPoint() const;
	/**
	 *   ���͸� ����ȭ�ϴ� �Լ�
	 */
	void Normalize()
	{
		float Len = Length();
		if (0.0f < Len && false == isnan(Len))
		{
			X = X / Len;
			Y = Y / Len;
			Z = Z / Len;
		}
		return;
	}
	/**
	 *   ����ȭ�� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @return ����ȭ�� ����
	 */
	FVector NormalizeReturn() const
	{
		FVector Result = *this;
		Result.Normalize();
		return Result;
	}

	/**
	 *   �� ���� ������ ������ �� ������ ����ϴ� �޼ҵ�
	 *
	 *   @param LVector ���� ����
	 *   @param RVector ������ ����
	 *   @return �� ���� ������ ���� (�� ����)
	 */
	static float GetVectorAngleDeg(const FVector& LVector, const FVector& RVector)
	{
		return GetVectorAngleRad(LVector, RVector) * FMath::R2D;
	}
	/**
	 *   �� ���� ������ ������ ���� ������ ����ϴ� �޼ҵ�
	 *
	 *   @param LVector ���� ����
	 *   @param RVector ������ ����
	 *   @return �� ���� ������ ���� (���� ����)
	 */
	static float GetVectorAngleRad(const FVector& LVector, const FVector& RVector)
	{
		FVector LCopy = LVector;
		FVector RCopy = RVector;

		LCopy.Normalize();
		RCopy.Normalize();

		float CosRad = Dot(LCopy, RCopy);

		return acos(CosRad);
	}
	/**
	 *   �� ������ ����(ũ�ν� ��ǰ)�� ����ϴ� �޼ҵ�
	 *
	 *   @param LVector ���� ����
	 *   @param RVector ������ ����
	 *   @return �� ������ ���� ���
	 */
	static FVector Cross(const FVector& LVector, const FVector& RVector)
	{
		FVector Result;

		Result.X = LVector.Y * RVector.Z - LVector.Z * RVector.Y;
		Result.Y = LVector.Z * RVector.X - LVector.X * RVector.Z;
		Result.Z = LVector.X * RVector.Y - LVector.Y * RVector.X;

		return Result;
	}
	/**
	 *   �� ������ ����(��Ʈ ��ǰ)�� ����ϴ� �޼ҵ�
	 *
	 *   @param LVector ���� ����
	 *   @param RVector ������ ����
	 *   @return �� ������ ���� ���
	 */
	static float Dot(const FVector& LVector, const FVector& RVector)
	{
		float LeftLen = LVector.Length();
		float RightLen = RVector.Length();

		return LVector.X * RVector.X + LVector.Y * RVector.Y + LVector.Z * RVector.Z;
	}
	/**
	 *   �� ������ ������ ����ϴ� �Լ�
	 *
	 *   @param other �ٸ� ����
	 *   @return �� ������ ���� ���
	 */
	float Dot(const FVector& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}
	/**
	 *   ���͸� ����ȭ�ϴ� �޼ҵ�
	 *
	 *   @param VectorValue ����ȭ�� ����
	 *   @return ����ȭ�� ����
	 */
	static FVector Normalize(FVector VectorValue)
	{
		VectorValue.Normalize();
		return VectorValue;
	}
	/**
	 *   �־��� ������ �̿��� ���� ���͸� ����ϴ� �޼ҵ�
	 *
	 *   @param Angle ����(��) ������ ǥ���� ��
	 *   @return �־��� ������ �ش��ϴ� ���� ����
	 */
	static FVector AngleToVectorDeg(float Angle)
	{
		return AngleToVectorRad(Angle * FMath::D2R);
	}
	/**
	 *   �־��� ������ �̿��� ���� ���͸� ����ϴ� �޼ҵ�
	 *
	 *   @param Angle ����(����) ������ ǥ���� ��
	 *   @return �־��� ������ �ش��ϴ� ���� ����
	 */
	static FVector AngleToVectorRad(float Angle)
	{
		return { cosf(Angle), sinf(Angle) };
	}
	/**
	 *   �־��� �� ���� ������ ���� ������ ����ϴ� �޼ҵ�
	 *
	 *   @param LVector ���� ����
	 *   @param RVector ���� ����
	 *   @param Alpha ���� ��� (0.0f���� 1.0f ������ ��)
	 *   @return Alpha�� ���� ���� ������ ����
	 */
	static FVector Lerp(FVector LVector, FVector RVector, float Alpha)
	{
		FVector Result;
		Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		Result.X = FMath::Lerp(LVector.X, RVector.X, Alpha);
		Result.Y = FMath::Lerp(LVector.Y, RVector.Y, Alpha);
		return Result;
	}
	/**
	 *   ���Ϳ� ����� ���Ͽ� Ʈ�������� �����ϴ� �޼ҵ�
	 *
	 *   @param InVector �Է� ����
	 *   @param InMatrix �Է� ���
	 *   @return Ʈ�������� ����
	 */
	static FVector TransformVector(const FVector& InVector, const class FMatrix& InMatrix);
	/**
	 *   ���Ϳ� �̵� ��ȯ�� �����Ͽ� Ʈ�������� �����ϴ� �޼ҵ�
	 *
	 *   @param InVector �Է� ����
	 *   @param InMatrix �Է� ���
	 *   @return �̵� ��ȯ�� ����� Ʈ�������� ����
	 */
	static FVector TransformVectorCoord(const FVector& InVector, const class FMatrix& InMatrix);
	/**
	 *   ���Ϳ� �̵� ��ȯ�� �������� �ʰ� Ʈ�������� �����ϴ� �޼ҵ�
	 *
	 *   @param InVector �Է� ����
	 *   @param InMatrix �Է� ���
	 *   @return �̵� ��ȯ�� ������� ���� Ʈ�������� ����
	 */
	static FVector TransformVectorNormal(const FVector& InVector, const class FMatrix& InMatrix);


	/**
	 *   X ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param Angle ȸ�� ���� (�� ����)
	 */
	void RotationXDeg(float Angle)
	{
		RotationXRad(Angle * FMath::D2R);
	}
	/**
	 *   X ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param Angle ȸ�� ���� (���� ����)
	 */
	void RotationXRad(float Angle)
	{
		FVector Copy = *this;
		Z = (Copy.Z * cosf(Angle)) - (Copy.Y * sinf(Angle));
		Y = (Copy.Z * sinf(Angle)) + (Copy.Y * cosf(Angle));
	}
	/**
	 *   X ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param Angle ȸ�� ���� (�� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationXDegReturn(float Angle)
	{
		return RotationXRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   X ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param Angle ȸ�� ���� (���� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationXRadReturn(float Angle)
	{
		FVector Result = *this;
		Result.Z = (Z * cosf(Angle)) - (Y * sinf(Angle));
		Result.Y = (Z * sinf(Angle)) + (Y * cosf(Angle));
		return Result;
	}
	/**
	 *   Y ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param Angle ȸ�� ���� (�� ����)
	 */
	void RotationYDeg(float Angle)
	{
		RotationYRad(Angle * FMath::D2R);
	}
	/**
	 *   Y ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (���� ����)
	 */
	void RotationYRad(float Angle)
	{
		FVector Copy = *this;
		X = (Copy.X * cosf(Angle)) - (Copy.Z * sinf(Angle));
		Z = (Copy.X * sinf(Angle)) + (Copy.Z * cosf(Angle));
	}
	/**
	 *   Y ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (�� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationYDegReturn(float Angle)
	{
		return RotationYRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   Y ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (���� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationYRadReturn(float Angle)
	{
		FVector Result = *this;
		Result.X = (X * cosf(Angle)) - (Z * sinf(Angle));
		Result.Z = (X * sinf(Angle)) + (Z * cosf(Angle));
		return Result;
	}
	/**
	 *   Z ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (�� ����)
	 */
	void RotationZDeg(float Angle)
	{
		RotationZRad(Angle * FMath::D2R);
	}

	/**
	 *   Z ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű�� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (���� ����)
	 */
	void RotationZRad(float Angle)
	{
		FVector Copy = *this;
		X = (Copy.X * cosf(Angle)) - (Copy.Y * sinf(Angle));
		Y = (Copy.X * sinf(Angle)) + (Copy.Y * cosf(Angle));
	}
	/**
	 *   Z ���� �������� ���͸� ����(�� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (�� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationZDegReturn(float Angle)
	{
		return RotationZRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   Z ���� �������� ���͸� ����(���� ����)��ŭ ȸ����Ű��, ��� ���͸� ��ȯ�ϴ� �Լ�
	 *
	 *   @param _Angle ȸ�� ���� (���� ����)
	 *   @return ȸ���� ����
	 */
	FVector RotationZRadReturn(float Angle)
	{
		FVector Result = *this;
		Result.X = (X * cosf(Angle)) - (Y * sinf(Angle));
		Result.Y = (X * sinf(Angle)) + (Y * cosf(Angle));
		return Result;
	}

	/** ���۷��̼� */
	ENGINE_API FVector operator*(const class FMatrix& InMatrix) const;
	ENGINE_API FVector& operator*=(const class FMatrix& InMatrix);
	FVector operator*(float FloatValue) const
	{
		FVector Result;
		Result.X = X * FloatValue;
		Result.Y = Y * FloatValue;
		return Result;
	}
	FVector& operator*=(const FVector& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
		Z *= Other.Z;
		return *this;
	}
	FVector& operator*=(float FloatValue)
	{
		X *= FloatValue;
		Y *= FloatValue;
		Z *= FloatValue;
		return *this;
	}
	FVector operator/(int IntValue) const
	{
		FVector Result;
		Result.X = X / IntValue;
		Result.Y = Y / IntValue;
		return Result;
	}

	FVector operator/(const FVector& Other) const
	{
		FVector Result;
		Result.X = X / Other.X;
		Result.Y = Y / Other.Y;
		return Result;
	}
	FVector operator+(const FVector& Other) const
	{
		FVector Result;
		Result.X = X + Other.X;
		Result.Y = Y + Other.Y;
		return Result;
	}
	FVector& operator+=(const FVector& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}
	FVector operator-(const FVector& Other) const
	{
		FVector Result;
		Result.X = X - Other.X;
		Result.Y = Y - Other.Y;
		return Result;
	}
	FVector operator-() const
	{
		FVector Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}
	FVector& operator-=(const FVector& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		return *this;
	}
	bool operator==(const FVector& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	std::string ToString()
	{
		std::string Stream;

		Stream += "X : [";
		Stream += std::to_string(X);
		Stream += "] Y : [";
		Stream += std::to_string(Y);
		Stream += "] Z : [";
		Stream += std::to_string(Z);
		Stream += "] W : [";
		Stream += std::to_string(W);
		Stream += "]";

		return Stream;
	}
};

using float4 = FVector;

/**
 *	Matrix Ŭ����
 */
class FMatrix
{

public:
	union
	{
		float Arr2D[4][4] = { 0, };
		FVector ArrVector[4];
		float Arr1D[16];
		DirectX::XMMATRIX DirectMatrix;

		struct
		{
			float _00;
			float _01;
			float _02;
			float _03;
			float _10;
			float _11;
			float _12;
			float _13;
			float _20;
			float _21;
			float _22;
			float _23;
			float _30;
			float _31;
			float _32;
			float _33;
		};

	};

	FMatrix()
	{
		Identity();
	}

	FVector GetFoward()
	{
		FVector Dir = ArrVector[2];
		Dir.Normalize();
		return Dir;
	}

	FVector GetRight()
	{
		FVector Dir = ArrVector[0];
		Dir.Normalize();
		return Dir;
	}

	FVector GetUp()
	{
		FVector Dir = ArrVector[1];
		Dir.Normalize();
		return Dir;
	}

	ENGINE_API FMatrix operator*(const FMatrix& InMatrixValue);

	/**
	 *   ����� ���� ��ķ� �ʱ�ȭ�ϴ� �Լ�
	 */
	void Identity()
	{
		DirectMatrix = DirectX::XMMatrixIdentity();
	}
	/**
	 *   �־��� ���� ���� ����Ͽ� ��Ŀ� ������ ��ȯ�� �����ϴ� �Լ�
	 *
	 *   @param InVectorValue ������ ���� ���Ե� ����
	 */
	void Scale(const FVector& InVectorValue)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(InVectorValue.DirectVector);
	}
	/**
	 *   �־��� ���� ���� ����Ͽ� ��Ŀ� ��ġ ��ȯ�� �����ϴ� �Լ�
	 *
	 *   @param InVectorValue ��ġ ���� ���Ե� ����
	 */
	void Position(const FVector& InVectorValue)
	{
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(InVectorValue.DirectVector);
	}
	/**
	 *   �־��� ���� ���� ����Ͽ� ��Ŀ� ����(�� ����)�� ȸ�� ��ȯ�� �����ϴ� �Լ�
	 *
	 *   @param RotationValue ȸ�� ���� (�� ����)
	 */
	void RotationDeg(const FVector& RotationValue)
	{
		RotationRad(RotationValue * FMath::D2R);
	}
	/**
	 *   �־��� ���� ���� ����Ͽ� ��Ŀ� ����(���� ����)�� ȸ�� ��ȯ�� �����ϴ� �Լ�
	 *
	 *   @param RotationValue ȸ�� ���� (���� ����)
	 */
	void RotationRad(const FVector& RotationValue)
	{
		DirectMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(RotationValue.DirectVector);
	}
	/**
	 *   �־��� ��ġ, ����, ���� ���� ���� ����Ͽ� �� ����� �����ϴ� �Լ�
	 *
	 *   @param Position ��ġ ����
	 *   @param Direction ���� ����
	 *   @param Up ���� ����
	 */
	void View(const FVector& Position, const FVector& Direction, const FVector& Up)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixLookToLH(Position.DirectVector, Direction.DirectVector, Up.DirectVector);
	}
	/**
	 *   �־��� �Ķ���� ���� ����Ͽ� ���� ���� ����� �����ϴ� �Լ�
	 *
	 *   @param Width ���� �ʺ�
	 *   @param Height ���� ����
	 *   @param Near ����� �Ÿ�
	 *   @param Far ����� �Ÿ�
	 */
	void OrthographicLH(float Width, float Height, float Near, float Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixOrthographicLH(Width, Height, Near, Far);
	}
	/**
	 *   �־��� �Ķ���� ���� ����Ͽ� ���� ���� ����� ����(�� ����)�� �����ϴ� �Լ�
	 *
	 *   @param FovAngle �þ߰� (�� ����)
	 *   @param Width ȭ�� �ʺ�
	 *   @param Height ȭ�� ����
	 *   @param Near ����� �Ÿ�
	 *   @param Far ����� �Ÿ�
	 */
	void PerspectiveFovDeg(float FovAngle, float Width, float Height, float Near, float Far)
	{
		PerspectiveFovRad(FovAngle * FMath::D2R, Width, Height, Near, Far);
	}
	/**
	 *   �־��� �Ķ���� ���� ����Ͽ� ���� ���� ����� ����(���� ����)�� �����ϴ� �Լ�
	 *
	 *   @param FovAngle �þ߰� (���� ����)
	 *   @param Width ȭ�� �ʺ�
	 *   @param Height ȭ�� ����
	 *   @param Near ����� �Ÿ�
	 *   @param Far ����� �Ÿ�
	 */
	void PerspectiveFovRad(float FovAngle, float Width, float Height, float Near, float Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(FovAngle, Width / Height, Near, Far);
	}













	/*
	void Transpose()
	{
		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = y; x < 4; x++)
			{
				float Swap = Arr2D[y][x];
				Arr2D[y][x] = Arr2D[x][y];
				Arr2D[x][y] = Swap;
			}
		}

	}

	// View����� �����Դϴ�.
	void MyView(const FVector& _Pos, const FVector& _Dir, const FVector& _Up)
	{
		// _Pos ī�޶� ��� �ٶ󺸰� �ֳ���?
		// _Dir ������� �ֳ���?
		// _Up �ٶ󺸴� ����� �������� �����ϴ� ���͸� �־��ּ���.

		// -90
		FVector Forward = _Dir.NormalizeReturn();
		FVector Up = _Up.NormalizeReturn();
		FVector Right = FVector::Cross(Up, Forward);
		Right.Normalize();


		ArrVector[2] = Forward;
		ArrVector[1] = Up;
		ArrVector[0] = Right;

		ArrVector[2].W = 0.0f;
		ArrVector[1].W = 0.0f;
		ArrVector[0].W = 0.0f;

		// ���� ȸ����� ������?
		// 90
		Transpose();

		FMatrix OrginRot = *this;

		FVector NPos = -_Pos;

		ArrVector[3].X = FVector::Dot(Right, NPos);
		ArrVector[3].Y = FVector::Dot(Up, NPos);
		ArrVector[3].Z = FVector::Dot(Forward, NPos);

		FVector Move = ArrVector[3];
		FVector OriginMove = NPos * OrginRot;

		return;
	}

	// ���⼭ �޼� ���� �� ��ǥ�赵 �ٲܼ� �ֽ��ϴ�.

	// _Widht �ʺ�� <= ������ ũ�� �ִ°� �Ϲ���
	// _Height ���� ȭ����
	//
	// �� �տ��ִ� _Far�Ÿ� �ȿ� �ִ� �ֵ���� ���ڴ�.
	// �� �տ��ִ� _Near���� ���ڴ�.

	//
	void MyOrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		Identity();

		// 1000;
		// 250 * (2 / 1000);
		// ũ�⸦ �ٲٴ� ����̴�
		// ������ ������

		//                      11      1
		float fRange = 1.0f / (_Far - _Near);

		// [*][ ][ ][ ]
		// [ ][*][ ][ ]
		// [ ][ ][*][ ]
		// [ ][ ][*][ ]

		Arr2D[0][0] = 2.0f / _Width;
		Arr2D[1][1] = 2.0f / _Height;
		Arr2D[2][2] = fRange;

		// Camera�� z�� near�� ���̿� �ִ� ������� z -������ ������ ���ؼ� �̴�.
		// ū�ǹ̴� ����. ���ڰ� �ʹ� �۾�
		Arr2D[3][2] = -fRange * _Near;
		// �׸�ŭ ������ ���ܼ� ����Ϳ� �� �ٿ��ַ��� �ϴ°�.
		// ���� ������ -1~1������ ���� �ǰ� ����� �̴ٰϴ�.

	}

	// �������� �ϳ���

	// ȭ���� ũ�⸦ �����ϱ� ���� _Width, _Height X
	// ȭ���� ������ �����ϱ� ���� _Width, _Height O
	// _FovAngle => x�࿡�� �ٶ�������� ������ �˷��޶�.
	void MyPerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		MyPerspectiveFovRad(_FovAngle * FMath::D2R, _Width, _Height, _Near, _Far);
	}

	void MyPerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();

		float ScreenRatio = _Width / _Height;
		float DivFov = _FovAngle / 2.0f;

		// / z�� �ؾ��ϴϱ�.
		// / z�� �ϱ� �������� ���� ������ ���� �ִ�.

		Arr2D[2][3] = 1.0f;
		Arr2D[3][3] = 0.0f;

		// x * 1.0f / (tanf(DivFov) * ScreenRatio) / z
		Arr2D[0][0] = 1.0f / (tanf(DivFov) * ScreenRatio);
		// y * 1.0f / (tanf(DivFov) * ScreenRatio)
		Arr2D[1][1] = 1.0f / tanf(DivFov);
		// z���� 0, 1������ ������ ����� ���� �����̴�.
		// ���⿡�� z * �� �Ǵ� ���̴�.
		Arr2D[2][2] = (_Far + _Near) / (_Far - _Near);
		// 0~ 1������ ������ ������� �ֳ���?

		Arr2D[3][2] = -2 * (_Near * _Far) / (_Far - _Near);
	}

	// ȭ�� Ȯ�� -1~1������ ���� �����ϱ�
	// +��ǥ�� ���� ���� ����
	// ȭ���� ���߾��� 0,0���� ����ϴ�
	// Y�� ������ ���⼭ �մϴ�.
	// ����Ʈ�� directx������ ���� ������ �ʿ䰡 ����. ���̷��� �־��ָ� ���̷�Ʈ�� �ڵ����� ���ִ� ���̴�.
	// directx::viewportsetting(ViewPort_desc);

	// ��ġ�� ũ�� ���ʿ����� �ִ� ����̴�.
	// �װ������� ��������� ���Ҽ� �־�.

	//                 1280          720        640           360            ���� �տ� ������ ���� �ڿ� ���ðų�
	void ViewPort(float _Width, float _Height, float _Left, float _Top, float _ZMin, float _ZMax)
	{
		Identity();
		Arr2D[0][0] = _Width * 0.5f;
		// Y�� ����
		Arr2D[1][1] = -_Height * 0.5f;

		// ȭ�� 2~3�Ѹ��ǵ� ���߿��� ���� �տ����� �ڿ������� �����Ϸ���
		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = -Arr2D[1][1] + _Top;
		Arr2D[3][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;
	}


	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * FMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		Identity();
		Arr2D[1][1] = cosf(_Angle);
		Arr2D[1][2] = -sinf(_Angle);
		Arr2D[2][1] = sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * FMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][2] = sinf(_Angle);
		Arr2D[2][0] = -sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * FMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][1] = -sinf(_Angle);
		Arr2D[1][0] = sinf(_Angle);
		Arr2D[1][1] = cosf(_Angle);
	}
	*/
};

using float4x4 = FMatrix;

struct FTransform
{
	float4 Scale;
	float4 Rotation;
	float4 Location;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;

	FTransform()
		: Scale({ 1.0f, 1.0f, 1.0f, 1.0f })
	{

	}

public:
	ENGINE_API void TransformUpdate();

private:
	friend class CollisionFunctionInit;

	static std::function<bool(const FTransform&, const FTransform&)> AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

public:
	static bool Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right);

	// ������ ���� ���� �Լ���?
	static bool PointToCirCle(const FTransform& _Left, const FTransform& _Right);
	static bool PointToRect(const FTransform& _Left, const FTransform& _Right);

	static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	static bool RectToCirCle(const FTransform& _Left, const FTransform& _Right);

	static bool CirCleToCirCle(const FTransform& _Left, const FTransform& _Right);
	static bool CirCleToRect(const FTransform& _Left, const FTransform& _Right);

	FVector ZAxisCenterLeftTop() const
	{
		return Location - Scale.Half();
	}

	FVector ZAxisCenterLeftBottom() const
	{
		FVector Result;
		Result.X = Location.X - Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	float ZAxisCenterLeft() const
	{
		return Location.X - Scale.hX();
	}

	float ZAxisCenterTop() const
	{
		return Location.Y - Scale.hY();
	}

	FVector ZAxisCenterRightTop() const
	{
		FVector Result;
		Result.X = Location.X + Scale.hX();
		Result.Y = Location.Y - Scale.hY();
		return Result;
	}

	FVector ZAxisCenterRightBottom() const
	{
		return Location + Scale.Half();
	}

	float ZAxisCenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float ZAxisCenterBottom() const
	{
		return Location.Y + Scale.hY();
	}
};

class FIntPoint
{
public:
	int X = 0;
	int Y = 0;

	static const FIntPoint LEFT;
	static const FIntPoint RIGHT;
	static const FIntPoint UP;
	static const FIntPoint DOWN;

	FIntPoint()
	{

	}

	FIntPoint(int _X, int _Y) : X(_X), Y(_Y)
	{

	}

	FIntPoint operator+(FIntPoint _Other) const
	{
		FIntPoint Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FIntPoint operator/(int _Value) const
	{
		FIntPoint Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}


	bool operator==(FIntPoint _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FIntPoint& operator+=(FIntPoint _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}


};


class UColor
{
public:
	static const UColor WHITE;
	static const UColor BLACK;

	union
	{
		int Color;
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
	};

	UColor(unsigned long _Value)
		:Color(_Value)
	{

	}

	bool operator==(const UColor& _Other)
	{
		return R == _Other.R && G == _Other.G && B == _Other.B;
	}


	UColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		:R(_R), G(_G), B(_B), A(_A)
	{

	}
};