#include "pch.h"
#include "EngineCore.h"
#include "WorldPartition/DebugHelpers.h"
#include "Platform/Window.h"
#include "Misc/DirectoryHelper.h"
#include "Misc/FileHelper.h"
#include "Engine/Classes/Engine/Level.h"
#include "Engine/Classes/Engine/EngineResource.h"
#include "Input/EngineInput.h"
#include "Tools/DebugGUI/EngineGUI.h"
#include "Rendering/Buffer/EngineConstantBuffer.h"

UEngineCore* GEngine = nullptr;

UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return GEngine->Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return GEngine->MainWindow;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}


void UEngineCore::EngineStart(HINSTANCE Instance, std::string_view DllName)
{
    UEngineDebug::LeakCheck();

	UEngineCore EngineCore;

	GEngine = &EngineCore;

	WindowInit(Instance);

	LoadContents(DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			// 엔진이 시작할 때 하고 싶은것
			GEngine->Device.CreateDeviceAndContext();
			GEngine->Core->EngineStart(GEngine->InitData);

			GEngine->MainWindow.SetWindowPosAndScale(GEngine->InitData.WindowPosition, GEngine->InitData.WindowSize);

			GEngine->Device.CreateBackBuffer(GEngine->MainWindow);

			UEngineGUI::Init();

		},
		[]()
		{
			// 엔진이 돌아갈 때 하고 싶은것
			EngineFrame();
		},
		[]()
		{
			// 엔진이 끝났을 때 하고 싶은것
			EngineEnd();
		});
}

void UEngineCore::WindowInit(HINSTANCE Instance)
{
	UEngineWindow::EngineWindowInit(Instance);
	GEngine->MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContents(std::string_view DllName)
{
	FDirectoryHelper DirectoryHelper;

	DirectoryHelper.MoveParentToDirectory("Build");
	DirectoryHelper.Move("Bin/x64");

#ifdef _DEBUG
	DirectoryHelper.Move("Debug");
#else
	DirectoryHelper.Move("Release");
#endif

	FFileHelper FileHelper = DirectoryHelper.GetFile(DllName);

	std::string FullPath = FileHelper.GetPathToString();

	GEngine->ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == GEngine->ContentsDLL)
	{
		MSGASSERT("컨텐츠 기능을 로드할 수 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");


	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine 메소드를 정의하지 않았습니다.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::EngineFrame()
{
	if (nullptr != GEngine->NextLevel)
	{
		if (nullptr != GEngine->CurLevel)
		{
			GEngine->CurLevel->LevelChangeEnd();
		}
		GEngine->CurLevel = GEngine->NextLevel;

		GEngine->CurLevel->LevelChangeStart();
	
		GEngine->NextLevel = nullptr;
		GEngine->Timer.TimeStart();
	}

	GEngine->Timer.TimeCheck();

	float DeltaTime = GEngine->Timer.GetDeltaTime();

	if (true == GEngine->MainWindow.IsFocus())
	{
		UEngineInput::KeyCheck(DeltaTime);
	}
	else {
		UEngineInput::KeyReset();
	}

	GEngine->CurLevel->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);
	GEngine->CurLevel->Collision(DeltaTime);
	GEngine->CurLevel->Release(DeltaTime);
}

void UEngineCore::EngineEnd()
{
	UEngineGUI::Release();

	GEngine->Device.Release();

	UResource::Release();
	UEngineConstantBuffer::Release();

	GEngine->CurLevel = nullptr;
	GEngine->NextLevel = nullptr;
	GEngine->LevelMap.clear();

	UEngineDebug::EndConsole();
}

FVector UEngineCore::GetSceenScale()
{
	return GEngine->InitData.WindowSize;
}


// 새로운 레벨을 생성하는 메소드 구현부
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view Name)
{
	if (true == GEngine->LevelMap.contains(Name.data()))
	{
		MSGASSERT("동일한 이름의 레벨이 존재합니다 " + std::string(Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(Name);

	GEngine->LevelMap.insert({ Name.data(), Ptr });

	std::cout << "New level create" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view LevelName)
{
	std::string UpperLevelName = UEngineString::ToUpper(LevelName);

	if (false == GEngine->LevelMap.contains(UpperLevelName))
	{
		MSGASSERT("존재하지 않는 레벨입니다. " + std::string(LevelName));
		return;
	}

	GEngine->NextLevel = GEngine->LevelMap[UpperLevelName];
}

std::map<std::string, std::shared_ptr<ULevel>> UEngineCore::GetAllLevelMap()
{
	return GEngine->LevelMap;
}
