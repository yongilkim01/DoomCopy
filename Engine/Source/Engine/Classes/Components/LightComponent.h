#pragma once
#include "Engine/Classes/Components/SceneComponent.h"

class ULevel;
class UCameraComponent;

/**
 * 빛을 계산하기 위한 상수 버퍼용 구조체
 */
struct FLightData
{
    float4 LightPos; /** 빛의 위치 (월드 공간 기준) */
    float4 LightDir; /** 빛의 전방 방향 벡터 (Forward 방향) */
    float4 LightRevDir; /** 빛의 반대 방향 벡터 (Reverse 방향, L = -LightDir) */
    /**
     * 빛의 색상 및 강도 (R, G, B, 강도).
     * 초기값은 흰색 (1, 1, 1)으로 설정
     */
    float4 LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    /**
     * 주변광(Ambient Light) 색상 및 강도 (R, G, B, 강도).
     * 기본적으로 은은한 회색빛
     */
    float4 AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 ViewLightPos; /** 뷰 공간에서의 빛 위치 (카메라 좌표계 기준 변환된 값) */
    float4 ViewLightDir; /** 뷰 공간에서의 빛 전방 방향 벡터 */
    float4 ViewLightRevDir; /** 뷰 공간에서의 빛 반대 방향 벡터 */
    /**
     * 카메라의 월드 좌표 위치.
     * 스펙큘러 조명(빛 반사가 카메라 방향으로 얼마나 반사되는지)을 계산하는 데 필요
     */
    float4 CameraPosition;
};

struct FLightDatas
{
    int Count;
    FLightData LightDataArr[256];
};


/**
 *	설명
 */
class ULightComponent : public USceneComponent
{
    friend ULevel;
public:
	/** 생성자, 소멸자 */
	ENGINE_API ULightComponent();
	ENGINE_API ~ULightComponent();

	/** 객체 값 복사 방지 */
	ULightComponent(const ULightComponent& Other) = delete;
	ULightComponent(ULightComponent&& Other) noexcept = delete;
	ULightComponent& operator=(const ULightComponent& Other) = delete;
	ULightComponent& operator=(ULightComponent&& Other) noexcept = delete;

protected:
    ENGINE_API virtual void BeginPlay() override;

private:
    void UpdateLight(UCameraComponent* CameraComponent, float DeltaTime);

    FLightData LightData;

};

