#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UStaticMeshComponent;
class UShapeComponent;

/**
 *	����
 */
class ADoor : public AActor
{
public:
	/** ������, �Ҹ��� */
	ADoor();
	~ADoor();

	/** ��ü �� ���� ���� */
	ADoor(const ADoor& Other) = delete;
	ADoor(ADoor&& Other) noexcept = delete;
	ADoor& operator=(const ADoor& Other) = delete;
	ADoor& operator=(ADoor&& Other) noexcept = delete;

	void SetModel(std::string_view ModelName, int ModelNumber);

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UStaticMeshComponent> DoorMeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
};

