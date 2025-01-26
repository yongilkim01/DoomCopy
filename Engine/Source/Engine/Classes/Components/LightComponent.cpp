#include "Engine.h"
#include "Engine/Classes/Components/LightComponent.h"

#include "Core/Public/Containers/EngineString.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Rendering/Public/Buffer/EngineVertex.h"
#include "Rendering/Public/State/BlendState.h"

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
    // ���� ��ġ�� ���� (���� ��ǥ�迡���� ��ġ)
    LightData.LightPos = GetWorldLocation();

    // ���� ������ ���� (���� ��ǥ�迡���� Forward ����)
    LightData.LightDir = GetWorldForwardVector();

    // ���� ������(�ݴ� ����) ���͸� ���
    LightData.LightRevDir = LightData.LightDir.operator-();

    // ���� ������ �⺻��(���)���� ����
    LightData.LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // �ں��Ʈ ����Ʈ(�ֺ���) ���� ����
    LightData.AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);

    // ī�޶��� �� ����� ������
    float4x4 View = CameraComponent->GetComponentTransformRef().View;

    // �� ���������� ���� ��ġ�� ���
    LightData.ViewLightPos = LightData.LightPos * View;

    // �� ���������� ���� ���� ���͸� ���
    LightData.ViewLightDir = LightData.LightDir * View;

    // �� ���������� ���� ������ ���͸� ���
    LightData.ViewLightRevDir = LightData.LightRevDir * View;

    // ī�޶��� ��ġ�� �� �������� ���
    LightData.CameraPosition = CameraComponent->GetWorldLocation();
}
