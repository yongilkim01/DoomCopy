#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"

class UStaticMesh;

/**
 *	설명
 */
class UTestComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UTestComponent();
	ENGINE_API ~UTestComponent();

	/** 객체 값 복사 방지 */
	UTestComponent(const UTestComponent& Other) = delete;
	UTestComponent(UTestComponent&& Other) noexcept = delete;
	UTestComponent& operator=(const UTestComponent& Other) = delete;
	UTestComponent& operator=(UTestComponent&& Other) noexcept = delete;

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent 상속 메소드 */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:

};