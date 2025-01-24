#pragma once

class UPrimitiveComponent;

/**
 *	설명
 */
class UPhysicsCore
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UPhysicsCore();
	ENGINE_API ~UPhysicsCore();

	/** 객체 값 복사 방지 */
	UPhysicsCore(const UPhysicsCore& Other) = delete;
	UPhysicsCore(UPhysicsCore&& Other) noexcept = delete;
	UPhysicsCore& operator=(const UPhysicsCore& Other) = delete;
	UPhysicsCore& operator=(UPhysicsCore&& Other) noexcept = delete;

	FVector SweepCollision(const FVector& Location, const FVector& Delta);
	FVector NormalComponent(const FVector& Location, const FVector& Delta);

protected:

private:

};

