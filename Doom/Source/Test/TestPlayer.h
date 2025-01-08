#pragma once
#include <GameFramework/Actor.h>

class ACameraActor;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	����
 */
class ATestPlayer : public AActor
{
public:
	/** ������, �Ҹ��� */
	ATestPlayer();
	~ATestPlayer();

	/** ��ü �� ���� ���� */
	ATestPlayer(const ATestPlayer& Other) = delete;
	ATestPlayer(ATestPlayer&& Other) noexcept = delete;
	ATestPlayer& operator=(const ATestPlayer& Other) = delete;
	ATestPlayer& operator=(ATestPlayer&& Other) noexcept = delete;

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

	float Speed = 50.0f;
	float Result = 0.0f;
};

