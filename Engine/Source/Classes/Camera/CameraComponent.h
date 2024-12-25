#pragma once
#include <map>
#include <list>

#include "Core/Math/EngineMath.h"
#include "Classes/Components/SceneComponent.h"

class ULevel;
class UPrimitiveComponent;

/**
 *	ī�޶� ������Ʈ Ŭ����
 */
class UCameraComponent : public USceneComponent
{
	friend ULevel;
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

	/** ī�޶� ������Ʈ �޼ҵ� */
	void CalculateViewAndProjection();

	/** ��, �� �޼ҵ� */
	inline float GetFar() { return Far; }
	inline void SetFar(float NewFar) { Far = NewFar; }
	inline float GetNear() { return Near; }
	inline void SetNear(float NewNear) { Near = NewNear; }

protected:

private:
	void ChangeRenderGroup(int PrevGroupOrder, std::shared_ptr<UPrimitiveComponent> Renderer);

	std::map<int, std::list<std::shared_ptr<UPrimitiveComponent>>> Rendereres;
	FVector ProjectionScale = { 0.0f, 0.0f };

	float Near = 1.0f;
	float Far = 5000.0f;

};
