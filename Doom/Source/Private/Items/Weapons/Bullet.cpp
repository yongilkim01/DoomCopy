#include "Doom.h"
#include "Public/Items/Weapons/Bullet.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

#include <Engine/Classes/Engine/RenderAsset.h>

#include "Public/Characters/Enemy/EnemyCharacter.h"

ABullet::ABullet()
{
	MeshComponent = CreateDefaultSubObject<UPrimitiveComponent>();
	RootComponent = MeshComponent;
	MeshComponent->SetRelativeScale3D({ 0.0f, 0.0f, 0.0f });
	MeshComponent->SetSimulatePhysics(false);

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
			AEnemyCharacter* EnemyCharacter = dynamic_cast<AEnemyCharacter*>(Other->GetOwner());
			
			if (nullptr != EnemyCharacter)
			{
				EnemyCharacter->TakeDamage(30);
			}
			
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

	CurLifeTime += DeltaTime;

	if (CurLifeTime > LifeTime)
	{
		Destroy();
	}

	if (Direction != FVector::ZERO)
	{
		MeshComponent->AddForce(Direction * 10000.0f);
	}
}
