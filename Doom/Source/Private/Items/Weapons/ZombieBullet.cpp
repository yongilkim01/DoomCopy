#include "Doom.h"
#include "Public/Items/Weapons/ZombieBullet.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Engine/Classes/Engine/RenderAsset.h>

AZombieBullet::AZombieBullet()
{
	MeshComponent = CreateDefaultSubObject<UPrimitiveComponent>();
	RootComponent = MeshComponent;
	MeshComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });
	MeshComponent->SetSimulatePhysics(false);

	MeshComponent->CreateRenderUnit();
	URenderAsset& Unit = MeshComponent->GetRenderUnit();
	Unit.SetMesh("Sphere");
	Unit.SetMaterial("LightMaterial");

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetCollisionProfileName("EnemyAttack");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);
	ShapeComponent->SetRelativeScale3D({ 3.0f, 3.0f, 3.0f });

	ShapeComponent->SetCollisionEnter([](UShapeComponent* This, UShapeComponent* Other)
		{
			//This->GetOwner()->Destroy();
			//Other->GetOwner()->Destroy();
			UEngineDebug::OutPutString("Player Hit!!");

		});

}

AZombieBullet::~AZombieBullet()
{
}

void AZombieBullet::BeginPlay()
{
	AActor::BeginPlay();
}

void AZombieBullet::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	CurLifeTime += DeltaTime;

	if (CurLifeTime > LifeTime)
	{
		Destroy();
	}

	if (Direction != FVector::ZERO)
	{
		MeshComponent->AddForce(Direction * 10.0f);
	}
}
