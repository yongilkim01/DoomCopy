#include "Doom.h"
#include "Public/Items/Weapons/Bullet.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Engine/Classes/Engine/RenderAsset.h>

ABullet::ABullet()
{
	MeshComponent = CreateDefaultSubObject<UPrimitiveComponent>();
	RootComponent = MeshComponent;
	MeshComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });

	MeshComponent->CreateRenderUnit();
	URenderAsset& Unit = MeshComponent->GetRenderUnit();
	Unit.SetMesh("Sphere");
	Unit.SetMaterial("LightMaterial");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("PlayerAttack");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
	ShapeComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });

	ShapeComponent->SetCollisionEnter([](UShapeComponent* This, UShapeComponent* Other)
		{
			This->GetOwner()->Destroy();
			Other->GetOwner()->Destroy();
			UEngineDebug::OutPutString("Enter");
			
		});

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
		MeshComponent->AddForce(Direction * 1000.0f);
	}
}
