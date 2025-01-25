#pragma once
#include <Engine/Classes/GameFramework/Character.h>

class ACameraActor;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	����
 */
class ANavMeshCharacter : public ACharacter
{
public:
	/** ������, �Ҹ��� */
	ANavMeshCharacter();
	~ANavMeshCharacter();

	/** ��ü �� ���� ���� */
	ANavMeshCharacter(const ANavMeshCharacter& Other) = delete;
	ANavMeshCharacter(ANavMeshCharacter&& Other) noexcept = delete;
	ANavMeshCharacter& operator=(const ANavMeshCharacter& Other) = delete;
	ANavMeshCharacter& operator=(ANavMeshCharacter&& Other) noexcept = delete;

	void MoveForward(float Value);
	void MoveRight(float Value);

	float GetResult()
	{
		return Result;
	}

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	FVector CurMouseLocation = FVector::ZERO;

	float Speed = 150.0f;
	float Result = 0.0f;

	float CameraSpeed = 10.0f;
};

