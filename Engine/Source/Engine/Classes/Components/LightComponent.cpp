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
    // 디렉셔널 라이트일때는 위치를 넣어줘봐야 의미가 없다.
    // 태양은 너무 멀리있고 너무 밝아.
    // 포인트 라이트 일때는 중요하지만 디렉셔널 라이트일때는 무시하는게 기본이다.
    LightData.LightPos = GetWorldLocation(); // 빛의 위치
    LightData.LightDir = GetWorldForwardVector(); // 빛의 Forward
    LightData.LightRevDir = -LightData.LightDir; // 빛의 반대방향이 < L!!!!! <= 
    LightData.LightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
    LightData.AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    LightData.ViewLightPos = LightData.LightPos * CameraComponent->GetComponentTransformRef().View;
    LightData.ViewLightDir = LightData.LightDir * CameraComponent->GetComponentTransformRef().View;
    LightData.ViewLightRevDir = LightData.LightRevDir * CameraComponent->GetComponentTransformRef().View;
    LightData.CameraPosition = CameraComponent->GetWorldLocation() * CameraComponent->GetComponentTransformRef().View;
}
