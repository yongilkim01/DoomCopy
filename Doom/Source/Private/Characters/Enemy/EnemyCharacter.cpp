#include "Doom.h"
#include "Public/Characters/Enemy/EnemyCharacter.h"

#include <Engine/Classes/Components/SceneComponent.h>
#include <Engine/Classes/Components/PaperSpriteComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>

AEnemyCharacter::AEnemyCharacter()
{
	std::shared_ptr<USceneComponent> SceneComponent = CreateDefaultSubObject<USceneComponent>();
	RootComponent = SceneComponent;
}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::BeginPlay()
{
	AActor::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

