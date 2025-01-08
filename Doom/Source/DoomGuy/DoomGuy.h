#pragma once
#include <GameFramework/Actor.h>

class ACameraActor;
class UStaticMeshComponent;
class UShapeComponent;
class UPaperSpriteComponent;

/**
 *	����
 */
class ADoomGuy : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADoomGuy();
	~ADoomGuy();

	/** ��ü �� ���� ���� */
	ADoomGuy(const ADoomGuy& Other) = delete;
	ADoomGuy(ADoomGuy&& Other) noexcept = delete;
	ADoomGuy& operator=(const ADoomGuy& Other) = delete;
	ADoomGuy& operator=(ADoomGuy&& Other) noexcept = delete;

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UStaticMeshComponent> MeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
	
	FVector PrevMouseLocation = FVector::ZERO;

	float Speed = 100.0f;
};

