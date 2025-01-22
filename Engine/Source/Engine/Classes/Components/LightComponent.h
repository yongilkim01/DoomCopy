#pragma once
#include "Engine/Classes/Components/SceneComponent.h"

/**
 * ���� ����ϱ� ���� ��� ���ۿ� ����ü
 */
struct FLightData
{
    friend class GameEngineLight;

    float4 LightPos; /** ���� ��ġ (���� ���� ����) */
    float4 LightDir; /** ���� ���� ���� ���� (Forward ����) */
    float4 LightRevDir; /** ���� �ݴ� ���� ���� (Reverse ����, L = -LightDir) */
    /**
     * ���� ���� �� ���� (R, G, B, ����).
     * �ʱⰪ�� ��� (1, 1, 1)���� ����
     */
    float4 LightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
    /**
     * �ֺ���(Ambient Light) ���� �� ���� (R, G, B, ����).
     * �⺻������ ������ ȸ����
     */
    float4 AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 ViewLightPos; /** �� ���������� �� ��ġ (ī�޶� ��ǥ�� ���� ��ȯ�� ��) */
    float4 ViewLightDir; /** �� ���������� �� ���� ���� ���� */
    float4 ViewLightRevDir; /** �� ���������� �� �ݴ� ���� ���� */
    /**
     * ī�޶��� ���� ��ǥ ��ġ.
     * ����ŧ�� ����(�� �ݻ簡 ī�޶� �������� �󸶳� �ݻ�Ǵ���)�� ����ϴ� �� �ʿ�
     */
    float4 CameraPosition;
};


/**
 *	����
 */
class ULightComponent : public USceneComponent
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API ULightComponent();
	ENGINE_API ~ULightComponent();

	/** ��ü �� ���� ���� */
	ULightComponent(const ULightComponent& Other) = delete;
	ULightComponent(ULightComponent&& Other) noexcept = delete;
	ULightComponent& operator=(const ULightComponent& Other) = delete;
	ULightComponent& operator=(ULightComponent&& Other) noexcept = delete;

protected:

private:
    FLightData LightData;

};

