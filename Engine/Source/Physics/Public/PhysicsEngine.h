#pragma once

class UPrimitiveComponent;

/**
 *	설명
 */
class UPhysicsEngine
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UPhysicsEngine();
	ENGINE_API ~UPhysicsEngine();

	/** 객체 값 복사 방지 */
	UPhysicsEngine(const UPhysicsEngine& Other) = delete;
	UPhysicsEngine(UPhysicsEngine&& Other) noexcept = delete;
	UPhysicsEngine& operator=(const UPhysicsEngine& Other) = delete;
	UPhysicsEngine& operator=(UPhysicsEngine&& Other) noexcept = delete;

	FVector SweepCollision(const FVector& Location, const FVector& Delta);
	FVector SweepCollision(const FVector& Location, const FVector& Delta, float InHalfHeight, float InRadius);
	FVector NormalComponent(const FVector& Location, const FVector& Delta);

protected:

private:

};

