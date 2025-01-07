#pragma once
#include <GameFramework/Actor.h>

class UStaticMeshComponent;
class UShapeComponent;

/**
 *	설명
 */
class ADoomGuy : public AActor
{
public:
	/** 생성자, 소멸자 */
	ADoomGuy();
	~ADoomGuy();

	/** 객체 값 복사 방지 */
	ADoomGuy(const ADoomGuy& Other) = delete;
	ADoomGuy(ADoomGuy&& Other) noexcept = delete;
	ADoomGuy& operator=(const ADoomGuy& Other) = delete;
	ADoomGuy& operator=(ADoomGuy&& Other) noexcept = delete;

protected:
	/** 액터 상속 메소드 */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UStaticMeshComponent> MeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
};

