#pragma once
#include <GameFramework/Actor.h>

class UStaticMeshComponent;
class UShapeComponent;

/**
 *	����
 */
class AE1M1Map : public AActor
{
public:
	/** ������, �Ҹ��� */
	AE1M1Map();
	~AE1M1Map();

	/** ��ü �� ���� ���� */
	AE1M1Map(const AE1M1Map& Other) = delete;
	AE1M1Map(AE1M1Map&& Other) noexcept = delete;
	AE1M1Map& operator=(const AE1M1Map& Other) = delete;
	AE1M1Map& operator=(AE1M1Map&& Other) noexcept = delete;

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UStaticMeshComponent> MeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
};

