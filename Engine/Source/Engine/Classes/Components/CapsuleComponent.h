#pragma once
#include "Engine/Classes/Components/ShapeComponent.h"

class UCameraComponent;

/**
 *	����
 */
class UCapsuleComponent : public UShapeComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UCapsuleComponent();
	ENGINE_API ~UCapsuleComponent();

	/** ��ü �� ���� ���� */
	UCapsuleComponent(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent(UCapsuleComponent&& Other) noexcept = delete;
	UCapsuleComponent& operator=(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent& operator=(UCapsuleComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;

protected:

private:

};

