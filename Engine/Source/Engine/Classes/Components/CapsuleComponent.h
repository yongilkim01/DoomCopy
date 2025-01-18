#pragma once
#include "Engine/Classes/Components/ShapeComponent.h"

class UCameraComponent;

/**
 *	설명
 */
class UCapsuleComponent : public UShapeComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UCapsuleComponent();
	ENGINE_API ~UCapsuleComponent();

	/** 객체 값 복사 방지 */
	UCapsuleComponent(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent(UCapsuleComponent&& Other) noexcept = delete;
	UCapsuleComponent& operator=(const UCapsuleComponent& Other) = delete;
	UCapsuleComponent& operator=(UCapsuleComponent&& Other) noexcept = delete;

	ENGINE_API virtual void BeginPlay() override;

protected:

private:

};

