#pragma once
#include "Pawn.h"

class UCharacterMovementComponent;
class UCapsuleComponent;
/**
 *	����
 */
class ACharacter : public APawn
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API ACharacter();
	ENGINE_API ~ACharacter();

	/** ��ü �� ���� ���� */
	ACharacter(const ACharacter& Other) = delete;
	ACharacter(ACharacter&& Other) noexcept = delete;
	ACharacter& operator=(const ACharacter& Other) = delete;
	ACharacter& operator=(ACharacter&& Other) noexcept = delete;

	ENGINE_API virtual void Tick(float DeltaTime) override;	

	ENGINE_API void AddMovementInput(const FVector& WorldDirection, float ScaleValue, bool bForce = false);
	void LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ);
	void CheckJumpInput(float DeltaTime);
	bool CanJump() const;

	/** ��, �� �޼ҵ� */
	ENGINE_API UCharacterMovementComponent* GetCharacterMovement()
	{
		return CharacterMovement.get();
	}
	ENGINE_API FVector GetVelocity() const;

protected:
	std::shared_ptr<UCapsuleComponent> CapsuleComponent;
	std::shared_ptr<UCharacterMovementComponent> CharacterMovement;

	bool bPressedJump = false;

private:
	int JumpCurrentCount = 0;

};