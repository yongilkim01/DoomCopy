#include "pch.h"
#include "Public/Items/Weapons/Bullet.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/PrimitiveComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Engine/RenderAsset.h>

ABullet::ABullet()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;
		
	MeshComponent = CreateDefaultSubObject<UPrimitiveComponent>();
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });

	MeshComponent->CreateRenderUnit();
	URenderAsset& Unit = MeshComponent->GetRenderUnit();
	Unit.SetMesh("Sphere");
	Unit.SetMaterial("LightMaterial");

}

ABullet::~ABullet()
{
}

void ABullet::BeginPlay()
{
    AActor::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
    AActor::Tick(DeltaTime);

	if (Direction != FVector::ZERO)
	{
		AddActorLocation(Direction * DeltaTime * 10000.0f);
	}
}
