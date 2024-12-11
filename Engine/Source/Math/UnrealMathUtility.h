#pragma once
#include <Windows.h>
#include <string>
#include <functional>

class UEngineMath
{
public:
	// ��� ����
	static const double DPI;
	static const double DPI2;

	static const float PI;
	static const float PI2;

	static const float D2R;
	static const float R2D;

	static float Sqrt(float _Value)
	{
		return ::sqrtf(_Value);
	}

	template <typename DataType>
	DataType ClampMax(DataType value, DataType maxValue)
	{
		return (value > maxValue) ? maxValue : value;
	}

	template <typename DataType>
	DataType ClampMin(DataType value, DataType minValue)
	{
		return (value < minValue) ? minValue : value;
	}

	template <typename DataType>
	static DataType Clamp(DataType value, DataType minValue, DataType maxValue)
	{
		if (value < minValue)
			return minValue;
		else if (value > maxValue)
			return maxValue;
		else
			return value;
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
	};


	FVector()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}

	FVector(float _X, float _Y) : X(_X), Y(_Y), Z(0.0f), W(1.0f)
	{

	}

	FVector(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z), W(1.0f)
	{

	}

	FVector(float _X, float _Y, float _Z, float _W) : X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}


	FVector(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	FVector(long _X, long _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	static float GetVectorAngleDeg(const FVector& _Left, const FVector& _Right)
	{
		return GetVectorAngleRad(_Left, _Right) * UEngineMath::R2D;
	}

	static float GetVectorAngleRad(const FVector& _Left, const FVector& _Right)
	{
		FVector LCopy = _Left;
		FVector RCopy = _Right;
		LCopy.Normalize();
		RCopy.Normalize();

		// Cos�� �����ΰ���?
		// cos(����) => CosRad
		// cos(����)
		float CosRad = Dot(LCopy, RCopy);

		// cos �� ���Լ� 
		// cos(����) => ���
		// acos(���) => ����
		// cos�Լ��� ���Լ�
		return acos(CosRad);
	}

	static FVector Cross(const FVector& _Left, const FVector& _Right)
	{
		FVector Result;
		Result.X = _Left.Y * _Right.Z - _Left.Z * _Right.Y;
		Result.Y = _Left.Z * _Right.X - _Left.X * _Right.Z;
		Result.Z = _Left.X * _Right.Y - _Left.Y * _Right.X;
		return Result;
	}

	static float Dot(const FVector& _Left, const FVector& _Right)
	{
		float LeftLen = _Left.Length();
		float RightLen = _Right.Length();

		// LeftLen* RightLen* cosf(angle);

		return _Left.X * _Right.X + _Left.Y * _Right.Y + _Left.Z * _Right.Z;
	}

	static FVector Normalize(FVector _Value)
	{
		_Value.Normalize();
		return _Value;
	}

	// 360�� �������� �־����.
	static FVector AngleToVectorDeg(float _Angle)
	{
		// 360�й��� => �������� �ٲٴ� ���� ������ �Ѵ�.
		// 360 => 6.28

		// ���� ����ü�踦 ������� sinf(_Angle) cosf

		// �ٺ��Լ��� ���� �������� �����
		return AngleToVectorRad(_Angle * UEngineMath::D2R);
	}


	static FVector Lerp(FVector _A, FVector _B, float _Alpha)
	{
		FVector Result;
		_Alpha = UEngineMath::Clamp(_Alpha, 0.0f, 1.0f);
		Result.X = UEngineMath::Lerp(_A.X, _B.X, _Alpha);
		Result.Y = UEngineMath::Lerp(_A.Y, _B.Y, _Alpha);
		return Result;
	}

	//          Rad ������ �־��ָ� 
	// ���⿡�� ���� ������� �������ټ� �ִ°�
	// ���̰� 1�� �����̴�.
	// static�Դϱ�?
	static FVector AngleToVectorRad(float _Angle)
	{
		// Ư�� ������ ����Ű�� ���͸� ����� �ִٰ� ����?
		// ���� ���̿� ������ �����ض�.
		// ������ �������µ� ���̴� 1�� ���͸� ������ �̴ϴ�.

		// 0���϶��� �غ�      0���ϴ��� ����

		// cosf(_Angle) = �غ�
		return { cosf(_Angle), sinf(_Angle) };
	}

	// �Ϲ������� ���Ϳ� ����� �������� ���� Ʈ�������̶�� �θ��ϴ�.
	// Ȥ�� Ʈ�������̼��̶�� �Լ����� �ִ�.
	static FVector Transform(const FVector& _Vector, const class FMatrix& _Matrix);

	// �̵� �����Ҳ�
	static FVector TransformCoord(const FVector& _Vector, const class FMatrix& _Matrix);

	// �̵� �������� ������.
	static FVector TransformNormal(const FVector& _Vector, const class FMatrix& _Matrix);

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

	// X�� Y�� 0�������� ��Ʈ���� �Լ�.
	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f;
	}

	FVector Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	// ������ �����Դϴ�.
	float Length() const
	{
		return UEngineMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	POINT ConvertWindowPOINT()
	{
		return { iX(), iY() };
	}

	class FIntPoint ConvertToPoint() const;

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

	FVector NormalizeReturn() const
	{
		FVector Result = *this;
		Result.Normalize();
		return Result;
	}

	// 
	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		FVector Copy = *this;
		Z = (Copy.Z * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.Z * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	FVector RotationXDegReturn(float _Angle)
	{
		return RotationXRadReturn(_Angle * UEngineMath::D2R);
	}

	FVector RotationXRadReturn(float _Angle)
	{
		FVector Result = *this;
		Result.Z = (Z * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (Z * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}


	// 
	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		FVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Z * sinf(_Angle));
		Z = (Copy.X * sinf(_Angle)) + (Copy.Z * cosf(_Angle));
	}

	FVector RotationYDegReturn(float _Angle)
	{
		return RotationYRadReturn(_Angle * UEngineMath::D2R);
	}

	FVector RotationYRadReturn(float _Angle)
	{
		FVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Z * sinf(_Angle));
		Result.Z = (X * sinf(_Angle)) + (Z * cosf(_Angle));
		return Result;
	}

	// 
	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		FVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.X * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	FVector RotationZDegReturn(float _Angle)
	{
		return RotationZRadReturn(_Angle * UEngineMath::D2R);
	}

	FVector RotationZRadReturn(float _Angle)
	{
		FVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (X * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}

	float Dot(const FVector& other) const
	{
		return X * other.X + Y * other.Y;
	}

	FVector operator*(float _Value) const
	{
		FVector Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		return Result;
	}

	FVector operator+(const FVector& _Other) const
	{
		FVector Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FVector operator*(const class FMatrix& _Matrix) const;
	FVector& operator*=(const class FMatrix& _Matrix);

	FVector& operator-=(const FVector& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		return *this;
	}


	FVector operator-(const FVector& _Other) const
	{
		FVector Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
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

	FVector operator/(int _Value) const
	{
		FVector Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}

	FVector operator/(const FVector& Other) const
	{
		FVector Result;
		Result.X = X / Other.X;
		Result.Y = Y / Other.Y;
		return Result;
	}

	// ture�� ������ 
	bool operator==(const FVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	// float�� �񱳰� ������ ����
	// const�� ���� �Լ������� const�� ���� �Լ� ȣ���Ҽ� ����.
	bool EqualToInt(FVector _Other) const
	{
		// const FVector* const Ptr;
		// this = nullptr;
		return iX() == _Other.iX() && iY() == _Other.iY();
	}

	//bool Compare(FVector _Other, float _limite = 0.0f) const
	//{
	//	return X == _Other.X && Y == _Other.Y;
	//}

	FVector& operator+=(const FVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;
		return *this;
	}

	FVector& operator*=(const FVector& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;
		return *this;
	}

	FVector& operator*=(float _Other)
	{
		X *= _Other;
		Y *= _Other;
		Z *= _Other;
		return *this;
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

// ��� �� ���� ��Ʈ���� ��� �մϴ�.
class FMatrix
{
public:
	union
	{
		float Arr2D[4][4] = { 0, };
		FVector ArrVector[4];
		float Arr1D[16];

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

	// �׷��Ƚ� ���α׷��� ��� ��ĵ��� ���������
	// �ϴ� �׵���ķ� ����ϴ�.

	// ����ȭ �׵���� ����� �Լ�
	void Identity()
	{
		Arr2D[0][0] = 1.0f;
		Arr2D[1][1] = 1.0f;
		Arr2D[2][2] = 1.0f;
		Arr2D[3][3] = 1.0f;
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

	FMatrix operator*(const FMatrix& _Value);

	void Scale(const FVector& _Value)
	{
		Arr2D[0][0] = _Value.X;
		Arr2D[1][1] = _Value.Y;
		Arr2D[2][2] = _Value.Z;
	}

	void Position(const FVector& _Value)
	{
		Arr2D[3][0] = _Value.X;
		Arr2D[3][1] = _Value.Y;
		Arr2D[3][2] = _Value.Z;
	}

	void RotationDeg(const FVector& _Angle)
	{
		FMatrix RotX;
		FMatrix RotY;
		FMatrix RotZ;

		// �Ʒ��� ���� ����°� �νŴ� ���������� ���մϴ�.
		/*Arr2D[1][1] = cosf(_Angle.X) * ;
		Arr2D[1][2] = -sinf(_Angle.X);
		Arr2D[2][1] = sinf(_Angle.X);
		Arr2D[2][2] = cosf(_Angle.X) * cosf(_Angle.Y);*/

		RotX.RotationXDeg(_Angle.X);
		RotY.RotationYDeg(_Angle.Y);
		RotZ.RotationZDeg(_Angle.Z);

		// ������ �ٲ���� �Ҷ��� �ֽ��ϴ�.
		// �������̶�� ������ �߻��ϱ� ������
		// RotY * RotZ * RotX;
		*this = RotX * RotY * RotZ;
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

	// View����� �����Դϴ�.
	void View(const FVector& _Pos, const FVector& _Dir, const FVector& _Up)
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
	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
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
	void PerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		PerspectiveFovRad(_FovAngle * UEngineMath::D2R, _Width, _Height, _Near, _Far);
	}

	void PerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
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

		// ȭ�� 2~3�Ѹ��ǵ� ���߿��� ���� �տ����� �ڿ����� �����Ϸ��� 
		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = -Arr2D[1][1] + _Top;
		Arr2D[3][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;
	}


	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
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
		RotationYRad(_Angle * UEngineMath::D2R);
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
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][1] = -sinf(_Angle);
		Arr2D[1][0] = sinf(_Angle);
		Arr2D[1][1] = cosf(_Angle);
	}

};



enum class ECollisionType
{
	Point,
	Rect,
	CirCle, // Ÿ���� �ƴ� ����� 
	Max

	//AABB,
	//OBB,
};

// ��κ� ������Ʈ���� ũ��� ��ġ�� �ѽ��Դϴ�.
// �׷��� �� 2������ ��� ���� �ڷ����� ���� �װ� ���.
class FTransform
{
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


	FVector Scale;
	FVector Location;


	FVector CenterLeftTop() const
	{
		return Location - Scale.Half();
	}

	FVector CenterLeftBottom() const
	{
		FVector Result;
		Result.X = Location.X - Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	float CenterLeft() const
	{
		return Location.X - Scale.hX();
	}

	float CenterTop() const
	{
		return Location.Y - Scale.hY();
	}

	FVector CenterRightTop() const
	{
		FVector Result;
		Result.X = Location.X + Scale.hX();
		Result.Y = Location.Y - Scale.hY();
		return Result;
	}

	FVector CenterRightBottom() const
	{
		return Location + Scale.Half();
	}

	float CenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float CenterBottom() const
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

using float4 = FVector;