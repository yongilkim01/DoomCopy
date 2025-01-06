#pragma once
#include <random>

/**
 *	����
 */
class UEngineRandom
{
public:
	/** ������, �Ҹ��� */
	UEngineRandom();
	~UEngineRandom();

	/** ��ü �� ���� ���� */
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
		// _Min ~ _Max ������ �̾���
		std::uniform_int_distribution<int> RandomCreate(Min, Max);
		// MtGen ���׷����� �Ἥ
		// std::mt19937_64 �޸��� Ʈ������ �˰��� �Ἥ �������.
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
		// _Min ~ _Max ������ �̾���
		std::uniform_real_distribution<float> RandomCreate(Min, Max);
		return RandomCreate.operator()(MtGen);
	}

protected:

private:
	std::mt19937_64 MtGen = std::mt19937_64(time(nullptr));

};