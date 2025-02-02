#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class UStaticMeshComponent;
class UShapeComponent;

enum class EDoorState : uint8
{
	NONE,
	STAY,
	OPEN,
	CLOSE,
};

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

	void ChangeState(EDoorState State);

	void EntryStay();
	void EntryOpen();
	void EntryClose();

	void Stay(float DeltaTime);
	void Open(float DeltaTime);
	void Close(float DeltaTime);

	void SetModel(std::string_view ModelName, int ModelNumber);
	void SetShapeComponentLocation(FVector Location);

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UStaticMeshComponent> DoorMeshComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	EDoorState CurDoorState = EDoorState::NONE;

	float DoorMoveSpeed = 100.0f;

	FVector PrevDoorLocation = FVector::ZERO;

	float StayCheckTime = 0.0f;
	float StayTimeLimit = 2.0f;
};

