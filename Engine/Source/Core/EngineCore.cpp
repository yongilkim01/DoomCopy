#include "pch.h"
#include "EngineCore.h"
#include "WorldPartition/DebugHelpers.h"
#include "Platform/Window.h"
#include "Misc/DirectoryHelper.h"
#include "Misc/FileHelper.h"
#include "Classes/Engine/Level.h"
#include "Classes/Engine/RenderAsset.h"
#include "Input/EngineInput.h"
#include "Tools/DebugGUI/EngineGUI.h"

UEngineGraphicDevice UEngineCore::Device;
UEngineWindow UEngineCore::MainWindow;
HMODULE UEngineCore::ContentsDLL = nullptr;
std::shared_ptr<IContentsCore> UEngineCore::Core;
UEngineInitData UEngineCore::InitData;
UEngineTimer UEngineCore::Timer;

std::shared_ptr<ULevel> UEngineCore::NextLevel;
std::shared_ptr<ULevel> UEngineCore::CurLevel = nullptr;

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::LevelMap;

UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return MainWindow;
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

	WindowInit(Instance);

	LoadContents(DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			// ������ ������ �� �ϰ� ������
			Device.CreateDeviceAndContext();
			Core->EngineStart(InitData);

			MainWindow.SetWindowPosAndScale(InitData.WindowPosition, InitData.WindowSize);

			Device.CreateBackBuffer(MainWindow);

			UEngineGUI::Init();

		},
		[]()
		{
			// ������ ���ư� �� �ϰ� ������
			EngineFrame();
		},
		[]()
		{
			// ������ ������ �� �ϰ� ������
			EngineEnd();
		});
}

void UEngineCore::WindowInit(HINSTANCE Instance)
{
	UEngineWindow::EngineWindowInit(Instance);
	MainWindow.Open("MainWindow");
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

	ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == ContentsDLL)
	{
		MSGASSERT("������ ����� �ε��� �� �����ϴ�.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(ContentsDLL, "CreateContentsCore");


	if (nullptr == Ptr)
	{
		MSGASSERT("������ �ھ� ���ο� CreateContentsCoreDefine �޼ҵ带 �������� �ʾҽ��ϴ�.");
		return;
	}

	Ptr(Core);

	if (nullptr == Core)
	{
		MSGASSERT("������ �ھ� ������ �����߽��ϴ�.");
		return;
	}
}

void UEngineCore::EngineFrame()
{
	if (nullptr != NextLevel)
	{
		if (nullptr != CurLevel)
		{
			CurLevel->LevelChangeEnd();
		}
		CurLevel = NextLevel;

		CurLevel->LevelChangeStart();
	
		NextLevel = nullptr;
		Timer.TimeStart();
	}

	Timer.TimeCheck();

	float DeltaTime = Timer.GetDeltaTime();

	UEngineInput::KeyCheck(DeltaTime);

	CurLevel->Tick(DeltaTime);
	CurLevel->Render(DeltaTime);
}

void UEngineCore::EngineEnd()
{
	UEngineGUI::Release();

	Device.Release();

	URenderAsset::Release();

	CurLevel = nullptr;
	NextLevel = nullptr;
	LevelMap.clear();

	UEngineDebug::EndConsole();
}

FVector UEngineCore::GetSceenScale()
{
	return InitData.WindowSize;
}


// ���ο� ������ �����ϴ� �޼ҵ� ������
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view Name)
{
	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(Name);

	LevelMap.insert({ Name.data(), Ptr });

	std::cout << "New level create" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view LevelName)
{
	if (false == LevelMap.contains(LevelName.data()))
	{
		MSGASSERT("�������� �ʴ� �����Դϴ�. " + std::string(LevelName));
		return;
	}

	NextLevel = LevelMap[LevelName.data()];
}
