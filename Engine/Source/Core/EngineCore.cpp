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
std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::LevelMap;

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
			// �����Ҷ� �ϰ� ������
			// 1. ������â ũ�� �ٲٰ� �ʹ�.
			UEngineInitData Data;
			Core->EngineStart(Data);

			MainWindow.SetWindowPosAndScale(Data.WindowPosition, Data.WindowSize);
		},
		[]()
		{
			// ������ ���ư��� �ϰ� ������
		},
		[]()
		{
			// ������ �������� �ϰ� ������.
			
			EngineEnd();
		});
}

void UEngineCore::OpenLevel(std::string_view LevelName)
{
	if (false == LevelMap.contains(LevelName.data()))
	{
		MSGASSERT("������ ���� ������ �����Ϸ��� �߽��ϴ�." + std::string(LevelName));
		return;
	}

	NextLevel = LevelMap[LevelName.data()];
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
	}

	CurLevel->Tick(0.0f);
}

void UEngineCore::EngineEnd()
{
	LevelMap.clear();
	UEngineDebug::EndConsole();
}

// ���ο� ������ �����ϴ� �޼ҵ� ������
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view Name)
{
	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	
	Ptr->SetName(Name);

	Levels.insert({ Name.data(), Ptr });

	return Ptr;
}
