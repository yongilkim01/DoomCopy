#pragma once
#include "Engine/Classes/GameFramework/Actor.h"
#include "EngineEnums.h"

class ULightComponent;
class UCameraComponent;

/**
 *	설명
 */
class ALight : public AActor
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API ALight();
	ENGINE_API ~ALight();

	/** 객체 값 복사 방지 */
	ALight(const ALight& Other) = delete;
	ALight(ALight&& Other) noexcept = delete;
	ALight& operator=(const ALight& Other) = delete;
	ALight& operator=(ALight&& Other) noexcept = delete;

protected:

private:
	std::shared_ptr<ULightComponent> LightComponent = nullptr;

};

