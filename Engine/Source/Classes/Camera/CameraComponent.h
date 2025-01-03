#pragma once
#include <map>
#include <list>

#include "Core/Math/EngineMath.h"
#include "Classes/Components/SceneComponent.h"
#include "EngineEnums.h"

class ULevel;
class UPrimitiveComponent;

/**
 *	카메라 컴포넌트 클래스
 */
class UCameraComponent : public USceneComponent
{
	friend ULevel;
public:
	/** 생성자, 소멸자 */
	UCameraComponent();
	~UCameraComponent();

	/** 객체 값 복사 방지 */
	UCameraComponent(const UCameraComponent& Other) = delete;
	UCameraComponent(UCameraComponent&& Other) noexcept = delete;
	UCameraComponent& operator=(const UCameraComponent& Other) = delete;
	UCameraComponent& operator=(UCameraComponent&& Other) noexcept = delete;

	/** 액터 컴포넌트 상속 메소드 */
	virtual void BeginPlay() override;

	/** 카메라 플로우 메소드 */
	void Tick(float DeltaTime);
	void Render(float DeltaTime);

	/** 카메라 컴포넌트 메소드 */
	void CalculateViewAndProjection();

	/** 겟, 셋 메소드 */
	inline float GetFar() { return Far; }
	ENGINE_API inline void SetFar(float NewFar) { Far = NewFar; }
	inline float GetNear() { return Near; }
	ENGINE_API inline void SetNear(float NewNear) { Near = NewNear; }
	ENGINE_API inline void SetFOV(float NewFOV)
	{
		FOV = NewFOV;
	}
	ENGINE_API void SetProjectionType(EProjectionType NewProjectionType)
	{
		ProjectionType = NewProjectionType;
	}

protected:

private:
	void ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer);

	std::map<int, std::list<std::shared_ptr<UPrimitiveComponent>>> Rendereres;
	FVector ProjectionScale = { 0.0f, 0.0f };

	float Near = 1.0f;
	float Far = 5000.0f;

	float FOV = 60.0f;
	EProjectionType ProjectionType = EProjectionType::Orthographic;

};
