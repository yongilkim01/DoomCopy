#pragma once
#include <Engine/Classes/GameFramework/Actor.h>

class ACameraActor;
class UPaperSpriteComponent;
class UShapeComponent;

/**
 *	설명
 */
class ANavMeshCharacter : public AActor
{
public:
	/** 생성자, 소멸자 */
	ANavMeshCharacter();
	~ANavMeshCharacter();

	/** 객체 값 복사 방지 */
	ANavMeshCharacter(const ANavMeshCharacter& Other) = delete;
	ANavMeshCharacter(ANavMeshCharacter&& Other) noexcept = delete;
	ANavMeshCharacter& operator=(const ANavMeshCharacter& Other) = delete;
	ANavMeshCharacter& operator=(ANavMeshCharacter&& Other) noexcept = delete;

	float GetResult()
	{
		return Result;
	}

protected:
	/** 액터 상속 메소드 */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;

	float Speed = 150.0f;
	float Result = 0.0f;
};

