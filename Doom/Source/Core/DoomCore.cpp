#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"
#include "GameMode/E1M1GameMode.h"

#include "Utils/NavMesh/GameMode/NavMeshGameMode.h"
#include "Utils/GUI/GUIEditor.h"
#include "Utils/NavMesh/Actor/NavMeshResource.h"
#include "UI/Public/TitleHUD.h"

#include <Core/Public/Math/EngineMath.h>
#include <Engine/Classes/Engine/GameEngine.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Misc/FileHelper.h>
#include <Engine/Classes/Engine/Level.h>
#include <Engine/Classes/Engine/Texture.h>
#include <Engine/Classes/Engine/PaperSprite.h>
#include <Engine/Classes/GameFramework/Actor.h>
#include <Engine/Classes/GameFramework/HUD.h>
#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <Rendering/Shader/EngineShader.h>
#include <Engine/Public/Materials/Material.h>
#include <Engine/Classes/Engine/StaticMesh.h>
#include <NavigationSystem/Public/NavigationSystem.h>

#include <stdio.h>


CreateContentsCoreDefine(UDoomCore);

UDoomCore::UDoomCore()
{

}

UDoomCore::~UDoomCore()
{
}

void UDoomCore::EngineStart(UEngineInitData& Data)
{
	InitWindowSize(Data);
	InitContentsInfo(Data);
}

void UDoomCore::EngineAssetLoad()
{
	{
		FDirectoryHelper DirectoryHelper;
		if (false == DirectoryHelper.MoveParentToDirectory("Resources", "Doom"))
		{
			MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
			return;
		}

		DirectoryHelper.Append("Images");

		std::vector<FFileHelper> ImageFiles = DirectoryHelper.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

		for (int i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UTexture::Load(FilePath);
		}
	}
	{
		FDirectoryHelper CurDir;
		CurDir.MoveSelectShaderDirectory("Doom");

		std::vector<FFileHelper> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

		for (size_t i = 0; i < ShaderFiles.size(); i++)
		{
			UEngineShader::ReflectionCompile(ShaderFiles[i]);
		}
	}
	{
		UStaticMesh::Create("E1M1", "doom_E1M1");
	}
	{
		std::vector<int> GroundModelNumber;
		GroundModelNumber.reserve(20);

		GroundModelNumber.push_back(35);
		GroundModelNumber.push_back(36);
		GroundModelNumber.push_back(37);
		GroundModelNumber.push_back(41);
		GroundModelNumber.push_back(45);
		GroundModelNumber.push_back(51);
		GroundModelNumber.push_back(52);

		UNavigationSystem::GetInstance().CreateNaviData("E1M1", "doom_E1M1", GroundModelNumber);
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("NavMeshTestMaterial");
		Mat->SetVertexShader("NavMeshTestShader.fx");
		Mat->SetPixelShader("NavMeshTestShader.fx");
	}
	{
		UNavMeshResource::Create("NavMapResource");
	}

	UPaperSprite::CreateSpriteToMeta("Player.png", ".sdata");

	{
		FDirectoryHelper Dir;
		if (false == Dir.MoveParentToDirectory("Resources", "Doom"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Images/Tevi");

		UPaperSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
}

void UDoomCore::EngineLevelStart()
{
	//UGameEngine::CreateLevel<ATitleGameMode, AActor, AHUD>("TitleLevel");
	UGameEngine::CreateLevel<AE1M1GameMode, APawn, AHUD>("E1M1Level");
	UGameEngine::CreateLevel<ANavMeshGameMode, APawn, ATitleHUD>("NavMeshLevel");
	UGameEngine::OpenLevel("NavMeshLevel");

	UEngineGUI::OffAllWindow();

	UEngineGUI::CreateGUIWindow<UGUIEditor>("GUIEditor");
	std::shared_ptr<UGUIEditor> Window = UEngineGUI::FindGUIWindow<UGUIEditor>("GUIEditor");
	Window->SetActive(true);
}

void UDoomCore::EngineTick(float DeltaTime)
{
}

void UDoomCore::EngineEnd()
{
}

void UDoomCore::InitContentsRenderingResource()
{

}

void UDoomCore::InitWindowSize(UEngineInitData& Data)
{
	float width = GetSystemMetrics(SM_CXSCREEN) - WindowWidth;
	float height = GetSystemMetrics(SM_CYSCREEN) - WindowHeight;

	Data.WindowPosition = { width / 2.0f , height / 2.0f };

	Data.WindowSize = { WindowWidth, WindowHeight };
}

void UDoomCore::InitContentsInfo(UEngineInitData& Data)
{
	Data.ContentsProjectName = ContentsName;
}
