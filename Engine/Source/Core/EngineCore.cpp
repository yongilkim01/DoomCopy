#include "pch.h"
#include "EngineCore.h"
#include "WorldPartition/DebugHelpers.h"
#include "Platform/Window.h"
#include "Misc/DirectoryHelper.h"
#include "Misc/FileHelper.h"
#include "Classes/Level.h"

UEngineWindow UEngineCore::MainWindow;
HMODULE UEngineCore::ContentsDLL = nullptr;
std::shared_ptr<IContentsCore> UEngineCore::Core;
std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::Levels;

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
			// 시작할때 하고 싶은것
			// 1. 윈도우창 크기 바꾸고 싶다.
			UEngineInitData Data;
			Core->EngineStart(Data);

			MainWindow.SetWindowPosAndScale(Data.WindowPosition, Data.WindowSize);
		},
		[]()
		{
			// 엔진이 돌아갈때 하고 싶은것
		},
		[]()
		{
			// 엔진이 끝났을때 하고 싶은것.
			
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
		MSGASSERT("컨텐츠 기능을 로드할 수 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(ContentsDLL, "CreateContentsCore");


	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine 메소드를 정의하지 않았습니다.");
		return;
	}

	Ptr(Core);

	if (nullptr == Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::EngineEnd()
{
	Levels.clear();
}

// 새로운 레벨을 생성하는 메소드 구현부
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view Name)
{
	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	
	Ptr->SetName(Name);

	Levels.insert({ Name.data(), Ptr });

	return Ptr;
}
