#include "Doom.h"
#include "Public/Maps/Door.h"

#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/ShapeComponent.h>
#include <Core/Public/Misc/DirectoryHelper.h>

ADoor::ADoor()
{
	std::shared_ptr<USceneComponent> Default = CreateDefaultSubObject<USceneComponent>();
	RootComponent = Default;

	DoorMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
	DoorMeshComponent->SetupAttachment(RootComponent);

	ShapeComponent = CreateDefaultSubObject<UShapeComponent>();
	ShapeComponent->SetupAttachment(RootComponent);
	ShapeComponent->SetRelativeScale3D({ 200.0f, 200.0f, 200.0f });
	ShapeComponent->SetCollisionProfileName("DoorInteraction");
	ShapeComponent->SetCollisionType(ECollisionType::Sphere);

	ShapeComponent->SetCollisionEnter([this](UShapeComponent* This, UShapeComponent* Other)
		{
			ChangeState(EDoorState::OPEN);
		});
	//ShapeComponent->SetCollisionEnd([this](UShapeComponent* This, UShapeComponent* Other)
	//	{
	//		ChangeState(EDoorState::CLOSE);
	//	});

}

ADoor::~ADoor()
{
}

void ADoor::BeginPlay()
{
	AActor::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	//std::string DebugMsg = "Door State : ";

	//switch (CurDoorState)
	//{
	//case EDoorState::STAY:
	//	DebugMsg += "Stay";
	//	break;
	//case EDoorState::OPEN:
	//	DebugMsg += "Open";
	//	break;
	//case EDoorState::CLOSE:
	//	DebugMsg += "Close";
	//	break;
	//default:
	//	break;
	//}

	//UEngineDebug::OutPutString(DebugMsg);

	switch (CurDoorState)
	{
	case EDoorState::STAY:
		Stay(DeltaTime);
		break;
	case EDoorState::OPEN:
		Open(DeltaTime);
		break;
	case EDoorState::CLOSE:
		Close(DeltaTime);
		break;
	default:
		break;
	}
}

void ADoor::ChangeState(EDoorState State)
{
	CurDoorState = State;

	switch (CurDoorState)
	{
	case EDoorState::STAY:
		EntryStay();
		break;
	case EDoorState::OPEN:
		EntryOpen();
		break;
	case EDoorState::CLOSE:
		EntryClose();
		break;
	default:
		break;
	}
}

void ADoor::EntryStay()
{
}

void ADoor::EntryOpen()
{
	PrevDoorLocation = DoorMeshComponent->GetWorldLocation();
}

void ADoor::EntryClose()
{
}

void ADoor::Stay(float DeltaTime)
{

}

void ADoor::Open(float DeltaTime)
{
	if (DoorMeshComponent->GetWorldLocation().Y > PrevDoorLocation.Y + 70.0f)
	{
		StayCheckTime += DeltaTime;

		if (StayCheckTime > StayTimeLimit)
		{
			StayCheckTime = 0.0f;
			ChangeState(EDoorState::CLOSE);
		}
	}
	else
	{
		DoorMeshComponent->AddWorldLocation(FVector{ 0.0f, DoorMoveSpeed * DeltaTime, 0.0f });

	}
}

void ADoor::Close(float DeltaTime)
{
	DoorMeshComponent->AddWorldLocation(FVector{ 0.0f, -DoorMoveSpeed * DeltaTime, 0.0f });

	if (DoorMeshComponent->GetWorldLocation().Y < PrevDoorLocation.Y)
	{
		DoorMeshComponent->SetWorldLocation(FVector{ DoorMeshComponent->GetWorldLocation().X, PrevDoorLocation.Y , DoorMeshComponent->GetWorldLocation().Z });
		ChangeState(EDoorState::STAY);
	}
}

// 15, 32, 34
void ADoor::SetModel(std::string_view ModelName, int ModelNumber)
{
	FDirectoryHelper DirectoryHelper;

	if (false == DirectoryHelper.MoveParentToDirectory("Resources", "Doom"))
	{
		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
		return;
	}

	DirectoryHelper.Append("Models\\E1M1");

	std::string Path = DirectoryHelper.GetPathToString();

	{
		DoorMeshComponent = CreateDefaultSubObject<UStaticMeshComponent>();
		DoorMeshComponent->SetupAttachment(RootComponent);

		std::vector<int> DoorModelNumber;
		DoorModelNumber.reserve(1);
		DoorModelNumber.push_back(ModelNumber);

		DoorMeshComponent->SetModel(ModelName, DoorModelNumber);
	}
}

void ADoor::SetShapeComponentLocation(FVector Location)
{
	ShapeComponent->SetWorldLocation(Location);
}
