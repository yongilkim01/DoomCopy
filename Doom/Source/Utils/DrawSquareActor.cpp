#include "pch.h"
#include "DrawSquareActor.h"

#include <iostream>
#include <algorithm>
using namespace std;

#include <Core/Math/EngineMath.h>
#include <Classes/Components/PaperSpriteComponent.h>
#include <Classes/Components/SceneComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Input/EngineInput.h>
#include <Classes/Components/PrimitiveComponent.h>

#include "Componennts/DoomMapComponent.h"
#include "Componennts/LineComponent.h"
#include "Utils/Wad.h"
#include "Utils/WadMap.h"
#include "Utils/DoomMesh.h"

ADrawSquareActor::ADrawSquareActor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	std::shared_ptr<UDoomMesh> DoomMesh = std::make_shared<UDoomMesh>();

	Wad = new UWad();

	FVector OutMin = { 20.0f, 20.0f };
	FVector OutMax = { 1280 - 20.0f, 720 - 20.0f };
	std::vector<FVector> RemappedVertexVector;

	for (int i = 0; i < Wad->GetMap()->GetVertexCount(); i++)
	{
		float RVMINX = min(Wad->GetMap()->GetVertexByIndex(i).X, Wad->GetMap()->GetMaxVector().X);
		float RVMAXX = max(Wad->GetMap()->GetMinVector().X, RVMINX);

		float RVMINY = min(Wad->GetMap()->GetVertexByIndex(i).Y, Wad->GetMap()->GetMaxVector().Y);
		float RVMAXY = max(Wad->GetMap()->GetMinVector().Y, RVMINY);
		FVector RemappedVertex = {
			-640.0f + (RVMAXX - Wad->GetMap()->GetMinVector().X) * (OutMax.X - OutMin.X) / (Wad->GetMap()->GetMaxVector().X - Wad->GetMap()->GetMinVector().X) + OutMin.X,
			-315.0f + (RVMAXY - Wad->GetMap()->GetMinVector().Y) * (OutMax.Y - OutMin.Y) / (Wad->GetMap()->GetMaxVector().Y - Wad->GetMap()->GetMinVector().Y) - OutMin.Y,
			1.0f,
			1.0f
		};

		RemappedVertexVector.push_back(RemappedVertex);
	}

	//for (int i = 0; i < Wad->GetMap()->GetWadLineCount(); i++)
	//{
	//	//UWadLine* WadLine = Wad->GetMap()->GetWadLineByIndex(i);
	//	//std::shared_ptr<ULineComponent> LineComponent = CreateDefaultSubObject<ULineComponent>();
	//	//LineComponent->SetupAttachment(RootComponent);
	//	//LineComponent->SetRelativeScale3D({ 10.0f, 1.0f, 1.0f });

	//	//LineComponentVector.push_back(LineComponent);
	//}

	for (int i = 0; i < RemappedVertexVector.size(); i++)
	{
		std::shared_ptr<UPrimitiveComponent> SquareComponent = CreateDefaultSubObject<UPrimitiveComponent>();
		SquareComponent->SetupAttachment(RootComponent);
		//SquareComponent->SetWorldLocation({ 0, 0 });
		SquareComponent->SetWorldLocation(RemappedVertexVector[i]);
		SquareComponent->SetRelativeScale3D({ FVector{10.0f, 10.0f} });
	}
}

ADrawSquareActor::~ADrawSquareActor()
{
	delete Wad;
	Wad = nullptr;
}

void ADrawSquareActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADrawSquareActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//AddActorRotation(FVector{ 0.0f, 0.0f, 360 * DeltaTime });
}
