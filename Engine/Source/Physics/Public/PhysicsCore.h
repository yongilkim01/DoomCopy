#pragma once

class UPrimitiveComponent;

/**
 *	����
 */
class UPhysicsCore
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UPhysicsCore();
	ENGINE_API ~UPhysicsCore();

	/** ��ü �� ���� ���� */
	UPhysicsCore(const UPhysicsCore& Other) = delete;
	UPhysicsCore(UPhysicsCore&& Other) noexcept = delete;
	UPhysicsCore& operator=(const UPhysicsCore& Other) = delete;
	UPhysicsCore& operator=(UPhysicsCore&& Other) noexcept = delete;

	FVector SweepCollision(const FVector& Location, const FVector& Delta);
	FVector NormalComponent(const FVector& Location, const FVector& Delta);

protected:

private:

};

