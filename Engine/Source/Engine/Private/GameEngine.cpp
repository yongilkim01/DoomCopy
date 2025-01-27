#include "Engine.h"
#include "Engine/Classes/Engine/GameEngine.h"

#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Debugging/DebugMacros.h"

#include "Engine/Classes/Engine/Level.h"
#include "Engine/Classes/Engine/EngineResource.h"
#include "Engine/Classes/Engine/GameInstance.h"

#include "Rendering/Public/Buffer/ConstantBuffer.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Platform/Public/Window/Window.h"
#include "Platform/Public/Input/EngineInput.h"

#include "Editor/Public/EngineGUI.h"

#include "Physics/Public/PhysicsCore.h"


UGameEngine* GEngine = nullptr;

UGameInstance* UGameEngine::GetGameInstance()
{
	return GEngine->GameInstance.get();
}

ID3D11Device* UGameEngine::GetDevice()
{
	return GEngine->Device.GetDevice();
}

ID3D11DeviceContext* UGameEngine::GetDeviceContext()
{
	return GEngine->Device.GetDeviceContext();
}

void UGameEngine::StartRender()
{
	GEngine->Device.RenderStart();
}

void UGameEngine::EndRender()
{
	GEngine->Device.RenderEnd();
}

std::shared_ptr<URenderTarget> UGameEngine::GetBackBufferRenderTarget()
{
	return GEngine->Device.GetBackBufferRenderTarget();
}

UEngineWindow& UGameEngine::GetMainWindow()
{
	return GEngine->MainWindow;
}

UGameEngine::UGameEngine()
{
}

UGameEngine::~UGameEngine()
{
}

void UGameEngine::SetGameInstance(std::shared_ptr<UGameInstance> Instance)
{
	GEngine->GameInstance = Instance;
}


void UGameEngine::EngineStart(HINSTANCE Instance, std::string_view DllName)
{
    UEngineDebug::LeakCheck();

	UGameEngine EngineCore;

	GEngine = &EngineCore;
	
	GEngine->PhysicsSubSystem = std::make_shared<UPhysicsCore>();
 
	WindowInit(Instance);

	LoadContents(DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			// ������ ������ �� �ϰ� ������
			GEngine->Device.CreateDeviceAndContext();

			GEngine->ContentsCore->EngineStart(GEngine->InitData);
			GEngine->SetContentsProjectName(GEngine->InitData.ContentsProjectName);
			GEngine->ContentsCore->EngineAssetLoad();
			GEngine->ContentsCore->EngineLevelStart();

			GEngine->MainWindow.SetWindowPosAndScale(GEngine->InitData.WindowPosition, GEngine->InitData.WindowSize);


			GEngine->Device.CreateBackBuffer(GEngine->MainWindow);

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

void UGameEngine::WindowInit(HINSTANCE Instance)
{
	UEngineWindow::EngineWindowInit(Instance);
	GEngine->MainWindow.Open("MainWindow");
}

void UGameEngine::LoadContents(std::string_view DllName)
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
		MSGASSERT("������ ����� �ε��� �� �����ϴ�.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");


	if (nullptr == Ptr)
	{
		MSGASSERT("������ �ھ� ���ο� CreateContentsCoreDefine �޼ҵ带 �������� �ʾҽ��ϴ�.");
		return;
	}

	Ptr(GEngine->ContentsCore);

	if (nullptr == GEngine->ContentsCore)
	{
		MSGASSERT("������ �ھ� ������ �����߽��ϴ�.");
		return;
	}
}

void UGameEngine::EngineFrame()
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
	//GEngine->PhysicsSubSystem->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);
	GEngine->CurLevel->Collision(DeltaTime);
	GEngine->CurLevel->Release(DeltaTime);
}

void UGameEngine::EngineEnd()
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

FVector UGameEngine::GetSceenScale()
{
	return GEngine->InitData.WindowSize;
}


// ���ο� ������ �����ϴ� �޼ҵ� ������
std::shared_ptr<ULevel> UGameEngine::NewLevelCreate(std::string_view Name)
{
	if (true == GEngine->LevelMap.contains(Name.data()))
	{
		MSGASSERT("������ �̸��� ������ �����մϴ� " + std::string(Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(Name);

	GEngine->LevelMap.insert({ Name.data(), Ptr });

	std::cout << "New level create" << std::endl;

	return Ptr;
}

void UGameEngine::OpenLevel(std::string_view LevelName)
{
	std::string UpperLevelName = UEngineString::ToUpper(LevelName);

	if (false == GEngine->LevelMap.contains(UpperLevelName))
	{
		MSGASSERT("�������� �ʴ� �����Դϴ�. " + std::string(LevelName));
		return;
	}

	GEngine->NextLevel = GEngine->LevelMap[UpperLevelName];
}

std::map<std::string, std::shared_ptr<ULevel>> UGameEngine::GetAllLevelMap()
{
	return GEngine->LevelMap;
}
