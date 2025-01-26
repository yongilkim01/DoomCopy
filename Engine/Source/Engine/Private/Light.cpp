#include "Engine.h"
#include "Engine/Classes/Engine/Light.h"

#include "Engine/Classes/Components/LightComponent.h"

ALight::ALight()
{
	LightComponent = CreateDefaultSubObject<ULightComponent>();
	RootComponent = LightComponent;
}

ALight::~ALight()
{
}