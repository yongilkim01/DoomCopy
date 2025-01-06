#pragma once
#include <random>

/**
 *	설명
 */
class UEngineRandom
{
public:
	/** 생성자, 소멸자 */
	UEngineRandom();
	~UEngineRandom();

	/** 객체 값 복사 방지 */
	UEngineRandom(const UEngineRandom& Other) = delete;
	UEngineRandom(UEngineRandom&& Other) noexcept = delete;
	UEngineRandom& operator=(const UEngineRandom& Other) = delete;
	UEngineRandom& operator=(UEngineRandom&& Other) noexcept = delete;

	void SetSeed(__int64 Seed)
	{
		MtGen = std::mt19937_64(Seed);
	}
	int RandomInt(int Min, int Max)
	{
		if (Max < Min)
		{
			int SwapValue = Min;
			Min = Max;
			Max = SwapValue;
		}
		// _Min ~ _Max 랜덤값 뽑아줘
		std::uniform_int_distribution<int> RandomCreate(Min, Max);
		// MtGen 제네레이터 써서
		// std::mt19937_64 메르헨 트위스터 알고리즘 써서 만들어줘.
		return RandomCreate.operator()(MtGen);
	}
	float Randomfloat(float Min, float Max)
	{
		if (Max < Min)
		{
			float SwapValue = Min;
			Min = Max;
			Max = SwapValue;
		}
		// _Min ~ _Max 랜덤값 뽑아줘
		std::uniform_real_distribution<float> RandomCreate(Min, Max);
		return RandomCreate.operator()(MtGen);
	}

protected:

private:
	std::mt19937_64 MtGen = std::mt19937_64(time(nullptr));

};