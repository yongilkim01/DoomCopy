#pragma once
#include <GameFramework/Actor.h>

class UStaticMeshComponent;
class UShapeComponent;

/**
 *	설명
 */
class AE1M1Map : public AActor
{
public:
	/** 생성자, 소멸자 */
	AE1M1Map();
	~AE1M1Map();

	/** 객체 값 복사 방지 */
	AE1M1Map(const AE1M1Map& Other) = delete;
	AE1M1Map(AE1M1Map&& Other) noexcept = delete;
	AE1M1Map& operator=(const AE1M1Map& Other) = delete;
	AE1M1Map& operator=(AE1M1Map&& Other) noexcept = delete;

protected:
	/** 액터 상속 메소드 */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UStaticMeshComponent> GroundMeshComponent = nullptr;
	std::shared_ptr<UStaticMeshComponent> WallMeshComponent = nullptr;
	std::shared_ptr<UStaticMeshComponent> DoorMeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
};

