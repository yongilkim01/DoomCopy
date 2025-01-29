#pragma once

class UPrimitiveComponent;

/**
 *	����
 */
class UPhysicsEngine
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UPhysicsEngine();
	ENGINE_API ~UPhysicsEngine();

	/** ��ü �� ���� ���� */
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

