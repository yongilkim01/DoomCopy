#include "pch.h"
#include "Engine/Classes/Components/LightComponent.h"

#include "Core/Public/Containers/EngineString.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/EngineBlend.h"

ULightComponent::ULightComponent()
{
}

ULightComponent::~ULightComponent()
{
}

void ULightComponent::BeginPlay()
{
	GetWorld()->PushLight(GetThis<ULightComponent>());
}

void ULightComponent::UpdateLight(UCameraComponent* CameraComponent, float DeltaTime)
{
    // �𷺼ų� ����Ʈ�϶��� ��ġ�� �־������ �ǹ̰� ����.
    // �¾��� �ʹ� �ָ��ְ� �ʹ� ���.
    // ����Ʈ ����Ʈ �϶��� �߿������� �𷺼ų� ����Ʈ�϶��� �����ϴ°� �⺻�̴�.
    LightData.LightPos = GetWorldLocation(); // ���� ��ġ
    LightData.LightDir = GetWorldForwardVector(); // ���� Forward
    LightData.LightRevDir = -LightData.LightDir; // ���� �ݴ������ < L!!!!! <= 
    LightData.LightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
    LightData.AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    LightData.ViewLightPos = LightData.LightPos * CameraComponent->GetComponentTransformRef().View;
    LightData.ViewLightDir = LightData.LightDir * CameraComponent->GetComponentTransformRef().View;
    LightData.ViewLightRevDir = LightData.LightRevDir * CameraComponent->GetComponentTransformRef().View;
    LightData.CameraPosition = CameraComponent->GetWorldLocation() * CameraComponent->GetComponentTransformRef().View;
}
