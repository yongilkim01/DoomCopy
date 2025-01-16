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
	OBB2D,
	Sphere,
	AABB,
	OBB,
	Max
};

class ENGINE_API FMath
{
public:
	// 상수 정의
	static inline const double DPI = 3.14159265358979323846264338327950288419716939937510;
	static inline const double DPI2 = DPI * 2.0;

	static inline const float PI = 3.14159265358979323846264f;
	static inline const float PI2 = PI * 2.0f;

	static inline const float D2R = FMath::PI / 180.0f;
	static inline const float R2D = 180.0f / FMath::PI;

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

template<typename ValueType>
class TVector
{
public:
	static const TVector NONE;
	static const TVector ZERO;
	static const TVector LEFT;
	static const TVector RIGHT;
	static const TVector UP;
	static const TVector DOWN;
	static const TVector FORWARD;
	static const TVector BACK;

	static const TVector WHITE;
	static const TVector BLACK;
	static const TVector RED;
	static const TVector BLUE;
	static const TVector GREEN;

public:
	union
	{
		struct
		{
			ValueType X;
			ValueType Y;
			ValueType Z;
			ValueType W;
		};

		ValueType Arr2D[1][4];
		ValueType Arr1D[4];
		DirectX::XMFLOAT3 DirectFloat3;
		DirectX::XMFLOAT4 DirectFloat4;

		/** DirectX Simd 연산 전용 벡터 */
		DirectX::XMVECTOR DirectVector;
	};

	ENGINE_API TVector()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}
	ENGINE_API TVector(DirectX::XMVECTOR InDirectVector) : DirectVector(InDirectVector)
	{
	}
	ENGINE_API TVector(float InX, float InY)
		: X(InX), Y(InY), Z(0.0f), W(1.0f)
	{

	}
	ENGINE_API TVector(float InX, float InY, float InZ)
		: X(InX), Y(InY), Z(InZ), W(1.0f)
	{

	}
	ENGINE_API TVector(float InX, float InY, float InZ, float InW)
		: X(InX), Y(InY), Z(InZ), W(InW)
	{

	}
	TVector(int InX, int InY)
		: X(static_cast<float>(InX)), Y(static_cast<float>(InY)), Z(0.0f), W(1.0f)
	{

	}
	TVector(long InX, long InY)
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
	 *   X 또는 Y가 0이면 true를 반환하는 함수
	 *
	 *   @return X 또는 Y가 0이면 true, 그렇지 않으면 false
	 */
	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f;
	}
	/**
	 *   벡터의 각 요소를 절반으로 줄인 벡터를 반환하는 함수
	 *
	 *   @return 각 요소가 절반인 벡터
	 */
	TVector Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}
	/**
	 *   벡터의 길이(빗변의 길이)를 계산하는 함수
	 *
	 *   @return 벡터의 길이
	 */
	float Length() const
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z);
	}
	/**
	 *   벡터를 윈도우 좌표계의 POINT로 변환하는 함수
	 *
	 *   @return 변환된 POINT
	 */
	POINT ConvertWindowPOINT()
	{
		return { iX(), iY() };
	}
	/**
	 *   벡터를 정수 좌표계의 FIntPoint로 변환하는 함수
	 *
	 *   @return 변환된 FIntPoint
	 */
	class FIntPoint ConvertToPoint() const;
	/**
	 *   벡터를 정규화하는 함수
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
	 *   벡터를 정규화하는 메소드
	 *
	 *   @param VectorValue 정규화할 벡터
	 *   @return 정규화된 벡터
	 */
	static TVector Normalize(TVector VectorValue)
	{
		VectorValue.Normalize();
		return VectorValue;
	}
	/**
	 *   정규화된 벡터를 반환하는 함수
	 *
	 *   @return 정규화된 벡터
	 */
	TVector NormalizeReturn() const
	{
		TVector Result = *this;
		Result.Normalize();
		return Result;
	}

	/**
	 *   두 벡터 사이의 각도를 도 단위로 계산하는 메소드
	 *
	 *   @param LVector 왼쪽 벡터
	 *   @param RVector 오른쪽 벡터
	 *   @return 두 벡터 사이의 각도 (도 단위)
	 */
	static float GetVectorAngleDeg(const TVector& LVector, const TVector& RVector)
	{
		return GetVectorAngleRad(LVector, RVector) * FMath::R2D;
	}
	/**
	 *   두 벡터 사이의 각도를 라디안 단위로 계산하는 메소드
	 *
	 *   @param LVector 왼쪽 벡터
	 *   @param RVector 오른쪽 벡터
	 *   @return 두 벡터 사이의 각도 (라디안 단위)
	 */
	static float GetVectorAngleRad(const TVector& LVector, const TVector& RVector)
	{
		TVector LCopy = LVector;
		TVector RCopy = RVector;

		LCopy.Normalize();
		RCopy.Normalize();

		float CosRad = Dot(LCopy, RCopy);

		return acos(CosRad);
	}
	/**
	 *   두 벡터의 외적(크로스 제품)을 계산하는 메소드
	 *
	 *   @param LVector 왼쪽 벡터
	 *   @param RVector 오른쪽 벡터
	 *   @return 두 벡터의 외적 결과
	 */
	static TVector Cross(const TVector& LVector, const TVector& RVector)
	{
		TVector Result;

		Result.X = LVector.Y * RVector.Z - LVector.Z * RVector.Y;
		Result.Y = LVector.Z * RVector.X - LVector.X * RVector.Z;
		Result.Z = LVector.X * RVector.Y - LVector.Y * RVector.X;

		return Result;
	}
	/**
	 *   두 벡터의 내적(도트 제품)을 계산하는 메소드
	 *
	 *   @param LVector 왼쪽 벡터
	 *   @param RVector 오른쪽 벡터
	 *   @return 두 벡터의 내적 결과
	 */
	static float Dot(const TVector& LVector, const TVector& RVector)
	{
		float LeftLen = LVector.Length();
		float RightLen = RVector.Length();

		return LVector.X * RVector.X + LVector.Y * RVector.Y + LVector.Z * RVector.Z;
	}
	/**
	 *   두 벡터의 내적을 계산하는 함수
	 *
	 *   @param other 다른 벡터
	 *   @return 두 벡터의 내적 결과
	 */
	float Dot(const TVector& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}
	/**
	 *   주어진 각도를 이용해 단위 벡터를 계산하는 메소드
	 *
	 *   @param Angle 각도(도) 단위로 표현된 값
	 *   @return 주어진 각도에 해당하는 단위 벡터
	 */
	static TVector AngleToVectorDeg(float Angle)
	{
		return AngleToVectorRad(Angle * FMath::D2R);
	}
	/**
	 *   주어진 각도를 이용해 단위 벡터를 계산하는 메소드
	 *
	 *   @param Angle 각도(라디안) 단위로 표현된 값
	 *   @return 주어진 각도에 해당하는 단위 벡터
	 */
	static TVector AngleToVectorRad(float Angle)
	{
		return { cosf(Angle), sinf(Angle) };
	}
	/**
	 *   주어진 두 벡터 사이의 선형 보간을 계산하는 메소드
	 *
	 *   @param LVector 시작 벡터
	 *   @param RVector 종료 벡터
	 *   @param Alpha 보간 계수 (0.0f에서 1.0f 사이의 값)
	 *   @return Alpha에 따라 선형 보간된 벡터
	 */
	static TVector Lerp(TVector LVector, TVector RVector, float Alpha)
	{
		TVector Result;
		Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);
		Result.X = FMath::Lerp(LVector.X, RVector.X, Alpha);
		Result.Y = FMath::Lerp(LVector.Y, RVector.Y, Alpha);
		return Result;
	}
	/**
	 *   벡터와 행렬을 곱하여 트랜스폼을 수행하는 메소드
	 *
	 *   @param InVector 입력 벡터
	 *   @param InMatrix 입력 행렬
	 *   @return 트랜스폼된 벡터
	 */
	static TVector TransformVector(const TVector& InVector, const class FMatrix& InMatrix);
	/**
	 *   벡터에 이동 변환을 적용하여 트랜스폼을 수행하는 메소드
	 *
	 *   @param InVector 입력 벡터
	 *   @param InMatrix 입력 행렬
	 *   @return 이동 변환이 적용된 트랜스폼된 벡터
	 */
	static TVector TransformVectorCoord(const TVector& InVector, const class FMatrix& InMatrix);
	/**
	 *   벡터에 이동 변환을 적용하지 않고 트랜스폼을 수행하는 메소드
	 *
	 *   @param InVector 입력 벡터
	 *   @param InMatrix 입력 행렬
	 *   @return 이동 변환이 적용되지 않은 트랜스폼된 벡터
	 */
	static TVector TransformVectorNormal(const TVector& InVector, const class FMatrix& InMatrix);


	/**
	 *   X 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키는 함수
	 *
	 *   @param Angle 회전 각도 (도 단위)
	 */
	void RotationXDeg(float Angle)
	{
		RotationXRad(Angle * FMath::D2R);
	}
	/**
	 *   X 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키는 함수
	 *
	 *   @param Angle 회전 각도 (라디안 단위)
	 */
	void RotationXRad(float Angle)
	{
		TVector Copy = *this;
		Z = (Copy.Z * cosf(Angle)) - (Copy.Y * sinf(Angle));
		Y = (Copy.Z * sinf(Angle)) + (Copy.Y * cosf(Angle));
	}
	/**
	 *   X 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param Angle 회전 각도 (도 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationXDegReturn(float Angle)
	{
		return RotationXRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   X 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param Angle 회전 각도 (라디안 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationXRadReturn(float Angle)
	{
		TVector Result = *this;
		Result.Z = (Z * cosf(Angle)) - (Y * sinf(Angle));
		Result.Y = (Z * sinf(Angle)) + (Y * cosf(Angle));
		return Result;
	}
	/**
	 *   Y 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키는 함수
	 *
	 *   @param Angle 회전 각도 (도 단위)
	 */
	void RotationYDeg(float Angle)
	{
		RotationYRad(Angle * FMath::D2R);
	}
	/**
	 *   Y 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키는 함수
	 *
	 *   @param _Angle 회전 각도 (라디안 단위)
	 */
	void RotationYRad(float Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(Angle)) - (Copy.Z * sinf(Angle));
		Z = (Copy.X * sinf(Angle)) + (Copy.Z * cosf(Angle));
	}
	/**
	 *   Y 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param _Angle 회전 각도 (도 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationYDegReturn(float Angle)
	{
		return RotationYRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   Y 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param _Angle 회전 각도 (라디안 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationYRadReturn(float Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(Angle)) - (Z * sinf(Angle));
		Result.Z = (X * sinf(Angle)) + (Z * cosf(Angle));
		return Result;
	}
	/**
	 *   Z 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키는 함수
	 *
	 *   @param _Angle 회전 각도 (도 단위)
	 */
	void RotationZDeg(float Angle)
	{
		RotationZRad(Angle * FMath::D2R);
	}

	/**
	 *   Z 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키는 함수
	 *
	 *   @param _Angle 회전 각도 (라디안 단위)
	 */
	void RotationZRad(float Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(Angle)) - (Copy.Y * sinf(Angle));
		Y = (Copy.X * sinf(Angle)) + (Copy.Y * cosf(Angle));
	}
	/**
	 *   Z 축을 기준으로 벡터를 각도(도 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param _Angle 회전 각도 (도 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationZDegReturn(float Angle)
	{
		return RotationZRadReturn(Angle * FMath::D2R);
	}
	/**
	 *   Z 축을 기준으로 벡터를 각도(라디안 단위)만큼 회전시키고, 결과 벡터를 반환하는 함수
	 *
	 *   @param _Angle 회전 각도 (라디안 단위)
	 *   @return 회전된 벡터
	 */
	TVector RotationZRadReturn(float Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(Angle)) - (Y * sinf(Angle));
		Result.Y = (X * sinf(Angle)) + (Y * cosf(Angle));
		return Result;
	}
	TVector ABSVectorReturn()
	{
		return DirectX::XMVectorAbs(DirectVector);
	}
	/**
	 * 오일러 각도를 사용하여 쿼터니언을 생성하는 메소드
	 *
	 * @return 생성된 회전 쿼터니언
	 */
	class FQuat DegAngleToQuaternion();


	/** 오퍼레이션 */
	ENGINE_API TVector operator*(const class FMatrix& InMatrix) const;
	ENGINE_API TVector& operator*=(const class FMatrix& InMatrix);
	TVector operator*(float FloatValue) const
	{
		TVector Result;
		Result.X = X * FloatValue;
		Result.Y = Y * FloatValue;
		Result.Z = Z * FloatValue;
		return Result;
	}
	TVector& operator*=(const TVector& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
		Z *= Other.Z;
		return *this;
	}
	TVector& operator*=(float FloatValue)
	{
		X *= FloatValue;
		Y *= FloatValue;
		Z *= FloatValue;
		return *this;
	}
	TVector operator/(int IntValue) const
	{
		TVector Result;
		Result.X = X / IntValue;
		Result.Y = Y / IntValue;
		return Result;
	}

	TVector operator/(const TVector& Other) const
	{
		TVector Result;
		Result.X = X / Other.X;
		Result.Y = Y / Other.Y;
		return Result;
	}
	TVector operator+(const TVector& Other) const
	{
		TVector Result;
		Result.X = X + Other.X;
		Result.Y = Y + Other.Y;
		Result.Z = Z + Other.Z;
		return Result;
	}
	TVector& operator+=(const TVector& Other) const
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}
	TVector& operator+=(const TVector& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}
	TVector operator-(const TVector& Other) const
	{
		TVector Result;
		Result.X = X - Other.X;
		Result.Y = Y - Other.Y;
		Result.Z = Z - Other.Z;
		return Result;
	}
	TVector operator-() const
	{
		TVector Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}
	TVector& operator-=(const TVector& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		Z -= Other.Z;
		return *this;
	}
	TVector& operator/=(const TVector& Other)
	{
		X /= Other.X;
		Y /= Other.Y;
		Z /= Other.Z;
		return *this;
	}
	bool operator==(const TVector& Other) const
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

template<>
const TVector<float> TVector<float>::NONE = TVector<float>(0.0f, 0.0f, 0.0f, 0.0f);
template<>
const TVector<float> TVector<float>::ZERO = TVector<float>(0.0f, 0.0f, 0.0f, 1.0f);
template<>
const TVector<float> TVector<float>::LEFT = TVector<float>(-1.0f, 0.0f, 0.0f, 0.0f);;
template<>
const TVector<float> TVector<float>::RIGHT = TVector<float>(1.0f, 0.0f, 0.0f, 0.0f);;
template<>
const TVector<float> TVector<float>::UP = TVector<float>(0.0f, 1.0f, 0.0f, 0.0f);;
template<>
const TVector<float> TVector<float>::DOWN = TVector<float>(0.0f, -1.0f, 0.0f, 0.0f);;
template<>
const TVector<float> TVector<float>::FORWARD = TVector<float>(0.0f, 0.0f, 1.0f, 0.0f);;
template<>
const TVector<float> TVector<float>::BACK = TVector<float>(0.0f, 0.0f, -1.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::WHITE = TVector<float>(1.0f, 1.0f, 1.0f, 1.0f);
template<>
const TVector<float> TVector<float>::BLACK = TVector<float>(0.0f, 0.0f, 0.0f, 1.0f);
template<>
const TVector<float> TVector<float>::RED = TVector<float>(1.0f, 0.0f, 0.0f, 1.0f);
template<>
const TVector<float> TVector<float>::BLUE = TVector<float>(0.0f, 0.0f, 1.0f, 1.0f);
template<>
const TVector<float> TVector<float>::GREEN = TVector<float>(0.0f, 1.0f, 0.0f, 1.0f);;

class FQuat
{
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
		// 다이렉트 simd 연산 전용 벡터.
		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;
	};
	TVector<float> QuaternionToEulerDeg() const;
	TVector<float> QuaternionToEulerRad() const;
};

using FVector = TVector<float>;
using float4 = TVector<float>;

/**
 *	Matrix 클래스
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
	 *   행렬을 단위 행렬로 초기화하는 함수
	 */
	void Identity()
	{
		DirectMatrix = DirectX::XMMatrixIdentity();
	}
	/**
	 *   주어진 벡터 값을 사용하여 행렬에 스케일 변환을 적용하는 함수
	 *
	 *   @param InVectorValue 스케일 값이 포함된 벡터
	 */
	void Scale(const FVector& InVectorValue)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(InVectorValue.DirectVector);
	}
	/**
	 *   주어진 벡터 값을 사용하여 행렬에 위치 변환을 적용하는 함수
	 *
	 *   @param InVectorValue 위치 값이 포함된 벡터
	 */
	void Position(const FVector& InVectorValue)
	{
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(InVectorValue.DirectVector);
	}
	/**
	 *   주어진 벡터 값을 사용하여 행렬에 각도(도 단위)로 회전 변환을 적용하는 함수
	 *
	 *   @param RotationValue 회전 각도 (도 단위)
	 */
	void RotationDeg(const FVector& RotationValue)
	{
		RotationRad(RotationValue * FMath::D2R);
	}
	/**
	 *   주어진 벡터 값을 사용하여 행렬에 각도(라디안 단위)로 회전 변환을 적용하는 함수
	 *
	 *   @param RotationValue 회전 각도 (라디안 단위)
	 */
	void RotationRad(const FVector& RotationValue)
	{
		DirectMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(RotationValue.DirectVector);
	}
	/**
	 *   주어진 위치, 방향, 위쪽 벡터 값을 사용하여 뷰 행렬을 설정하는 함수
	 *
	 *   @param Position 위치 벡터
	 *   @param Direction 방향 벡터
	 *   @param Up 위쪽 벡터
	 */
	void View(const FVector& Position, const FVector& Direction, const FVector& Up)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixLookToLH(Position.DirectVector, Direction.DirectVector, Up.DirectVector);
	}
	/**
	 *   주어진 파라미터 값을 사용하여 원근 투영 행렬을 설정하는 함수
	 *
	 *   @param FieldOfView 수직 시야각 (라디안 단위)
	 *   @param AspectRatio 종횡비 (너비 / 높이)
	 *   @param Near 근평면 거리
	 *   @param Far 원평면 거리
	 */
	void PerspectiveFovLH(float FieldOfView, float AspectRatio, float Near, float Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(FieldOfView, AspectRatio, Near, Far);
	}

	/**
	 *   주어진 파라미터 값을 사용하여 직교 투영 행렬을 설정하는 함수
	 *
	 *   @param Width 투영 너비
	 *   @param Height 투영 높이
	 *   @param Near 근평면 거리
	 *   @param Far 원평면 거리
	 */
	void OrthographicLH(float Width, float Height, float Near, float Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixOrthographicLH(Width, Height, Near, Far);
	}
	/**
	 *   주어진 파라미터 값을 사용하여 원근 투영 행렬을 각도(도 단위)로 설정하는 함수
	 *
	 *   @param FovAngle 시야각 (도 단위)
	 *   @param Width 화면 너비
	 *   @param Height 화면 높이
	 *   @param Near 근평면 거리
	 *   @param Far 원평면 거리
	 */
	void PerspectiveFovDeg(float FovAngle, float Width, float Height, float Near, float Far)
	{
		PerspectiveFovRad(FovAngle * FMath::D2R, Width, Height, Near, Far);
	}
	/**
	 *   주어진 파라미터 값을 사용하여 원근 투영 행렬을 각도(라디안 단위)로 설정하는 함수
	 *
	 *   @param FovAngle 시야각 (라디안 단위)
	 *   @param Width 화면 너비
	 *   @param Height 화면 높이
	 *   @param Near 근평면 거리
	 *   @param Far 원평면 거리
	 */
	void PerspectiveFovRad(float FovAngle, float Width, float Height, float Near, float Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(FovAngle, Width / Height, Near, Far);
	}
	/**
	 * FMatrix의 역행렬을 계산하여 반환하는 메소드
	 *
	 * @return 역행렬이 계산된 FMatrix 객체
	 */
	FMatrix InverseReturn()
	{
		FMatrix Result;

		// DirectX 함수 XMMatrixInverse를 사용하여 역행렬을 계산
		// 첫 번째 매개변수는 선택적 출력 매개변수로 nullptr을 전달
		// 두 번째 매개변수는 원래의 DirectMatrix
		Result.DirectMatrix = DirectX::XMMatrixInverse(nullptr, DirectMatrix);

		return Result;
	}
	/**
	 * 행렬을 분해하여 스케일 벡터, 회전 쿼터니언, 위치 벡터를 반환하는 메소드
	 *
	 * @param ScaleVector - 분해된 스케일 벡터를 저장할 변수
	 * @param RotQuat - 분해된 회전 쿼터니언을 저장할 변수
	 * @param Location - 분해된 위치 벡터를 저장할 변수
	 */
	void Decompose(FVector& ScaleVector, FQuat& RotQuat, FVector& Location)
	{
		/**
		 *	DirectX의 XMMatrixDecompose 함수를 사용하여 행렬을 분해
		 *	첫 번째 매개변수는 스케일 벡터를 저장할 주소
		 *	두 번째 매개변수는 회전 쿼터니언을 저장할 주소
		 *	세 번째 매개변수는 위치 벡터를 저장할 주소
		 *	네 번째 매개변수는 분해할 행렬
		 */
		DirectX::XMMatrixDecompose(&ScaleVector.DirectVector,
			&RotQuat.DirectVector,
			&Location.DirectVector,
			DirectMatrix);
	}
	void ViewPort(float Width, float Height, float Left, float Top, float ZMin, float ZMax)
	{
		Identity();
		Arr2D[0][0] = Width * 0.5f;
		// Y축 반전
		Arr2D[1][1] = -Height * 0.5f;

		// 화면 2~3뿌릴건데 그중에서 누가 앞에오고 뒤에오고를 결정하려면 
		Arr2D[2][2] = ZMax != 0.0f ? 1.0f : ZMin / ZMax;

		Arr2D[3][0] = Arr2D[0][0] + Left;
		Arr2D[3][1] = -Arr2D[1][1] + Top;
		Arr2D[3][2] = ZMax != 0.0f ? 1.0f : ZMin / ZMax;
	}
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

	/*
	// View행렬의 인자입니다.
	void MyView(const FVector& _Pos, const FVector& _Dir, const FVector& _Up)
	{
		// _Pos 카메라가 어디서 바라보고 있나요?
		// _Dir 어딜보고 있나요?
		// _Up 바라보는 방향과 수직으로 직교하는 벡터를 넣어주세요.

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

		// 나의 회전행렬 구했죠?
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

	// 여기서 왼손 오른 손 좌표계도 바꿀수 있습니다.

	// _Widht 너비와 <= 윈도우 크기 넣는게 일반적
	// _Height 높의 화면을
	//
	// 내 앞에있는 _Far거리 안에 있는 애들까지 보겠다.
	// 내 앞에있는 _Near부터 보겠다.

	//
	void MyOrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		Identity();

		// 1000;
		// 250 * (2 / 1000);
		// 크기를 바꾸는 행렬이다
		// 직교는 더더욱

		//                      11      1
		float fRange = 1.0f / (_Far - _Near);

		// [*][ ][ ][ ]
		// [ ][*][ ][ ]
		// [ ][ ][*][ ]
		// [ ][ ][*][ ]

		Arr2D[0][0] = 2.0f / _Width;
		Arr2D[1][1] = 2.0f / _Height;
		Arr2D[2][2] = fRange;

		// Camera의 z와 near와 사이에 있는 존재들을 z -축으로 보내기 위해서 이다.
		// 큰의미는 없다. 숫자가 너무 작어
		Arr2D[3][2] = -fRange * _Near;
		// 그만큼 앞으로 땡겨서 모니터에 딱 붙여주려고 하는것.
		// 직교 투영은 -1~1사이의 값이 되게 만들어 줄겁니다.

	}

	// 인자중의 하나는

	// 화면의 크기를 정의하기 위한 _Width, _Height X
	// 화면의 비율을 정의하기 위한 _Width, _Height O
	// _FovAngle => x축에서 바라봤을대의 각도를 알려달라.
	void MyPerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		MyPerspectiveFovRad(_FovAngle * FMath::D2R, _Width, _Height, _Near, _Far);
	}

	void MyPerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();

		float ScreenRatio = _Width / _Height;
		float DivFov = _FovAngle / 2.0f;

		// / z를 해야하니까.
		// / z를 하기 전까지의 값은 추출해 낼수 있다.

		Arr2D[2][3] = 1.0f;
		Arr2D[3][3] = 0.0f;

		// x * 1.0f / (tanf(DivFov) * ScreenRatio) / z
		Arr2D[0][0] = 1.0f / (tanf(DivFov) * ScreenRatio);
		// y * 1.0f / (tanf(DivFov) * ScreenRatio)
		Arr2D[1][1] = 1.0f / tanf(DivFov);
		// z값을 0, 1사이의 값으로 만드는 것이 목적이다.
		// 여기에서 z * 가 되는 값이다.
		Arr2D[2][2] = (_Far + _Near) / (_Far - _Near);
		// 0~ 1사이의 값으로 만들수가 있나요?

		Arr2D[3][2] = -2 * (_Near * _Far) / (_Far - _Near);
	}

	// 화면 확대 -1~1사이의 값이 됐으니까
	// +좌표축 변경 중점 변경
	// 화면의 정중앙을 0,0으로 만듭니다
	// Y축 반전도 여기서 합니다.
	// 뷰포트는 directx에서는 내가 곱해줄 필요가 없다. 다이렉에 넣어주면 다이렉트가 자동으로 해주는 것이다.
	// directx::viewportsetting(ViewPort_desc);

	// 위치와 크기 양쪽영향을 주는 행렬이다.
	// 그것조차도 내마음대로 정할수 있어.

	//                 1280          720        640           360            누가 앞에 나오고 누가 뒤에 나올거냐
	void ViewPort(float _Width, float _Height, float _Left, float _Top, float _ZMin, float _ZMax)
	{
		Identity();
		Arr2D[0][0] = _Width * 0.5f;
		// Y축 반전
		Arr2D[1][1] = -_Height * 0.5f;

		// 화면 2~3뿌릴건데 그중에서 누가 앞에오고 뒤에오고를 결정하려면
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

struct FCollisionData
{
	union
	{
		// 정방원
		DirectX::BoundingSphere Sphere;
		DirectX::BoundingBox AABB;
		DirectX::BoundingOrientedBox OBB;
	};

	FCollisionData()
	{
	}
};

struct FTransform
{
	float4 Scale;
	float4 Rotation;
	FQuat Quat;
	float4 Location;

	// 릴리에티브 로컬
	float4 RelativeScale;
	float4 RelativeRotation;
	FQuat RelativeQuat;
	float4 RelativeLocation;

	// 월드
	float4 WorldScale;
	float4 WorldRotation;
	FQuat WorldQuat;
	float4 WorldLocation;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 RevolveMat;
	float4x4 ParentMat;
	float4x4 LocalWorld;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;

	FTransform()
		: Scale(FVector{ 1.0f, 1.0f, 1.0f, 1.0f })
	{

	}

public:
	ENGINE_API void TransformUpdate(bool bAbsolute = false);
	ENGINE_API void Decompose();

	FVector GetWorldFoward()
	{
		return World.GetFoward();;
	}
	FVector GetWorldRight()
	{
		return World.GetRight();
	}
	FVector GetWorldUp()
	{
		return World.GetUp();
	}
	FVector GetLocalFoward()
	{
		return LocalWorld.GetFoward();;
	}
	FVector GetLocalRight()
	{
		return LocalWorld.GetRight();
	}
	FVector GetLocalUp()
	{
		return LocalWorld.GetUp();
	}

private:
	friend class CollisionFunctionInit;

	static std::function<bool(const FTransform&, const FTransform&)> AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

public:
	ENGINE_API static bool Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right);

	// 완전히 같은 형의 함수죠?
	ENGINE_API static bool PointToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool PointToRect(const FTransform& _Left, const FTransform& _Right);

	ENGINE_API static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool RectToCirCle(const FTransform& _Left, const FTransform& _Right);
 
	ENGINE_API static bool CirCleToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool CirCleToRect(const FTransform& _Left, const FTransform& _Right);

	ENGINE_API static bool OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool OBB2DToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool OBB2DToPoint(const FTransform& _Left, const FTransform& _Right);
	ENGINE_API static bool OBB2DToCircle(const FTransform& _Left, const FTransform& _Right);

	ENGINE_API static bool OBBToSphere(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool OBBToOBB(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool OBBToAABB(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool SphereToSphere(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool SphereToOBB(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool SphereToAABB(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool AABBToSphere(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool AABBToOBB(const FTransform& Left, const FTransform& Right);
	ENGINE_API static bool AABBToAABB(const FTransform& Left, const FTransform& Right);

	FCollisionData GetCollisionData() const
	{
		FCollisionData Result;
		// OBB를 세팅해준거 같지만 모든 애들을 다 세팅해준 것입니다.
		// Sphere와 AABB전체를 다 세팅해준겁니다.
		Result.OBB.Center = WorldLocation.DirectFloat3;
		Result.OBB.Extents = (WorldScale * 0.5f).ABSVectorReturn().DirectFloat3;
		Result.OBB.Orientation = WorldQuat.DirectFloat4;
		return Result;
	}

	FVector ZAxisCenterLeftTop() const
	{
		return FVector(Location.X - Scale.Half().X, Location.Y + Scale.Half().Y);
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
		return Location.Y + Scale.hY();
	}

	FVector ZAxisCenterRightTop() const
	{
		FVector Result;
		Result.X = Location.X + Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	FVector ZAxisCenterRightBottom() const
	{
		return FVector(Location.X + Scale.Half().X, Location.Y - Scale.Half().Y);
	}

	float ZAxisCenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float ZAxisCenterBottom() const
	{
		return Location.Y - Scale.hY();
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

template<typename ValueType>
class TColor
{
public:
	static const TColor WHITE;
	static const TColor BLACK;

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

	TColor(unsigned long _Value)
		:Color(_Value)
	{

	}

	bool operator==(const TColor& _Other)
	{
		return R == _Other.R && G == _Other.G && B == _Other.B;
	}


	TColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		:R(_R), G(_G), B(_B), A(_A)
	{

	}
};

using UColor = TColor<unsigned char>;

template<>
const TColor<unsigned char> TColor<unsigned char>::WHITE = TColor<unsigned char>(255, 255, 255, 0);

template<>
const TColor<unsigned char> TColor<unsigned char>::BLACK = TColor<unsigned char>(0, 0, 0, 0);