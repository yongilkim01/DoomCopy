#pragma once
#include <Engine/Classes/GameFramework/Character.h>

class ACameraActor;
class UStaticMeshComponent;
class UShapeComponent;
class UPaperSpriteComponent;

/**
 *	설명
 */
class ADoomGuyCharacter : public ACharacter
{
public:
	/** 생성자, 소멸자 */
	ADoomGuyCharacter();
	~ADoomGuyCharacter();

	/** 객체 값 복사 방지 */
	ADoomGuyCharacter(const ADoomGuyCharacter& Other) = delete;
	ADoomGuyCharacter(ADoomGuyCharacter&& Other) noexcept = delete;
	ADoomGuyCharacter& operator=(const ADoomGuyCharacter& Other) = delete;
	ADoomGuyCharacter& operator=(ADoomGuyCharacter&& Other) noexcept = delete;

	void MoveForward(float Value);
	void MoveRight(float Value);

protected:
	/** 액터 상속 메소드 */
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

