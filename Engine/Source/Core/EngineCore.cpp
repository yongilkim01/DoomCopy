#include "pch.h"
#include "EngineCore.h"
#include "WorldPartition/DebugHelpers.h"
#include "Platform/Window.h"
#include "Misc/DirectoryHelper.h"
#include "Misc/FileHelper.h"

UEngineWindow UEngineCore::MainWindow;
HMODULE UEngineCore::ContentsDLL = nullptr;
std::shared_ptr<IContentsCore> UEngineCore::Core;

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
		},
		[]()
		{
			// ������ ���ư��� �ϰ� ������
		},
		[]()
		{
			// ������ �������� �ϰ� ������.
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
