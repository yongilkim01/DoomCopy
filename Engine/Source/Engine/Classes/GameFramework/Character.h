#pragma once
#include "Pawn.h"

class UCharacterMovementComponent;
class UCapsuleComponent;
/**
 *	설명
 */
class ACharacter : public APawn
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API ACharacter();
	ENGINE_API ~ACharacter();

	/** 객체 값 복사 방지 */
	ACharacter(const ACharacter& Other) = delete;
	ACharacter(ACharacter&& Other) noexcept = delete;
	ACharacter& operator=(const ACharacter& Other) = delete;
	ACharacter& operator=(ACharacter&& Other) noexcept = delete;

	ENGINE_API virtual void Tick(float DeltaTime) override;	

	ENGINE_API void AddMovementInput(const FVector& WorldDirection, float ScaleValue, bool bForce = false);
	void LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ);
	void CheckJumpInput(float DeltaTime);
	bool CanJump() const;

	/** 겟, 셋 메소드 */
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