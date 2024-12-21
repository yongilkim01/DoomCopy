#pragma once
#include <map>
#include <list>

#include "Math/EngineMath.h"
#include "Classes/Components/SceneComponent.h"

class ULevel;
class URenderer;

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
	inline void SetFar(float NewFar) { Far = NewFar; }
	inline float GetNear() { return Near; }
	inline void SetNear(float NewNear) { Near = NewNear; }

protected:

private:
	void ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<URenderer> Renderer);

	std::map<int, std::list<std::shared_ptr<URenderer>>> Rendereres;
	FVector ProjectionScale = { 0.0f, 0.0f };

	float Near = 1.0f;
	float Far = 5000.0f;

};
