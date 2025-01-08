#pragma once
#include <map>
#include <list>

#include "Core/Math/EngineMath.h"
#include "Classes/Components/SceneComponent.h"
#include "EngineEnums.h"

class ULevel;
class ACameraActor;
class UPrimitiveComponent;

/**
 *	ī�޶� ������Ʈ Ŭ����
 */
class UCameraComponent : public USceneComponent
{
	friend ULevel;
	friend ACameraActor;
public:
	/** ������, �Ҹ��� */
	UCameraComponent();
	~UCameraComponent();

	/** ��ü �� ���� ���� */
	UCameraComponent(const UCameraComponent& Other) = delete;
	UCameraComponent(UCameraComponent&& Other) noexcept = delete;
	UCameraComponent& operator=(const UCameraComponent& Other) = delete;
	UCameraComponent& operator=(UCameraComponent&& Other) noexcept = delete;

	/** ���� ������Ʈ ��� �޼ҵ� */
	virtual void BeginPlay() override;

	/** ī�޶� �÷ο� �޼ҵ� */
	void Tick(float DeltaTime);
	void Render(float DeltaTime);
	ENGINE_API void Release(float DeltaTime);

	/** ī�޶� ������Ʈ �޼ҵ� */
	void CalculateViewAndProjection();

	/** ��, �� �޼ҵ� */
	ENGINE_API void SetZSort(int NewOrder, bool bValue);
	ENGINE_API inline float GetFar() { return Far; }
	ENGINE_API inline void SetFar(float NewFar) { Far = NewFar; }
	ENGINE_API inline float GetNear() { return Near; }
	ENGINE_API inline void SetNear(float NewNear) { Near = NewNear; }
	ENGINE_API inline void SetFOV(float NewFOV) { FOV = NewFOV; }
	ENGINE_API inline void SetProjectionType(EProjectionType NewProjectionType) { ProjectionType = NewProjectionType; }

protected:

private:
	void ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer);

	std::map<int, std::list<std::shared_ptr<UPrimitiveComponent>>> RenderComponentMap;
	std::map<int, bool> RenderComponentZSort;

	FVector ProjectionScale = { 0.0f, 0.0f };

	float Near = 1.0f;
	float Far = 5000.0f;

	float FOV = 60.0f;
	EProjectionType ProjectionType = EProjectionType::Perspective;

	D3D11_VIEWPORT ViewPortInfo;

};
