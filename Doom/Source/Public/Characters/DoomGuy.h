#pragma once
#include <Engine/Classes/GameFramework/Character.h>

class ACameraActor;
class UStaticMeshComponent;
class UShapeComponent;
class UPaperSpriteComponent;
class AGunWeapon;
class APistolWeapon;
class AShotgunWeapon;


/**
 *	����
 */
class ADoomGuyCharacter : public ACharacter
{
public:
	/** ������, �Ҹ��� */
	ADoomGuyCharacter();
	~ADoomGuyCharacter();

	/** ��ü �� ���� ���� */
	ADoomGuyCharacter(const ADoomGuyCharacter& Other) = delete;
	ADoomGuyCharacter(ADoomGuyCharacter&& Other) noexcept = delete;
	ADoomGuyCharacter& operator=(const ADoomGuyCharacter& Other) = delete;
	ADoomGuyCharacter& operator=(ADoomGuyCharacter&& Other) noexcept = delete;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void ChangeWeapon(int WeaponIndex);

protected:
	/** ���� ��� �޼ҵ� */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;

	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
	std::shared_ptr<UPaperSpriteComponent> AimingPointSpriteComponent = nullptr;
	std::shared_ptr<UShapeComponent> ShapeComponent = nullptr;
	AGunWeapon* GunActor = nullptr;
	std::shared_ptr<APistolWeapon> PistolActor = nullptr;
	std::shared_ptr<AShotgunWeapon> ShotgunActor = nullptr;
	
	FVector CurMouseLocation = FVector::ZERO;

	float Speed = 150.0f;
	float Result = 0.0f;

	float CameraSpeed = 10.0f;

	bool bPlayMode = false;
};

