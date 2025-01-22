#pragma once
#include "Engine/Classes/GameFramework/Actor.h"
#include "EngineEnums.h"

class ULightComponent;
class UCameraComponent;

/**
 *	����
 */
class ALight : public AActor
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API ALight();
	ENGINE_API ~ALight();

	/** ��ü �� ���� ���� */
	ALight(const ALight& Other) = delete;
	ALight(ALight&& Other) noexcept = delete;
	ALight& operator=(const ALight& Other) = delete;
	ALight& operator=(ALight&& Other) noexcept = delete;

protected:

private:
	std::shared_ptr<ULightComponent> LightComponent = nullptr;

};

