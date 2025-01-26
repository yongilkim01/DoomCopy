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
    // 빛의 위치를 설정 (월드 좌표계에서의 위치)
    LightData.LightPos = GetWorldLocation();

    // 빛의 방향을 설정 (월드 좌표계에서의 Forward 벡터)
    LightData.LightDir = GetWorldForwardVector();

    // 빛의 역방향(반대 방향) 벡터를 계산
    LightData.LightRevDir = LightData.LightDir.operator-();

    // 빛의 색상을 기본값(흰색)으로 설정
    LightData.LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

    // 앰비언트 라이트(주변광) 색상 설정
    LightData.AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);

    // 카메라의 뷰 행렬을 가져옴
    float4x4 View = CameraComponent->GetComponentTransformRef().View;

    // 뷰 공간에서의 빛의 위치를 계산
    LightData.ViewLightPos = LightData.LightPos * View;

    // 뷰 공간에서의 빛의 방향 벡터를 계산
    LightData.ViewLightDir = LightData.LightDir * View;

    // 뷰 공간에서의 빛의 역방향 벡터를 계산
    LightData.ViewLightRevDir = LightData.LightRevDir * View;

    // 카메라의 위치를 뷰 공간에서 계산
    LightData.CameraPosition = CameraComponent->GetWorldLocation();
}
