#include "Doom.h"
#include "Public/Core/DoomCore.h"

#include <Core/Public/Math/EngineMath.h>
#include <Core/Public/Misc/DirectoryHelper.h>
#include <Core/Public/Misc/FileHelper.h>

#include <Engine/Public/Materials/Material.h>
#include <Engine/Classes/Engine/StaticMesh.h>
#include <Engine/Classes/Engine/GameEngine.h>
#include <Engine/Classes/Engine/Level.h>
#include <Engine/Classes/Engine/Texture.h>
#include <Engine/Classes/Engine/PaperSprite.h>
#include <Engine/Classes/GameFramework/Actor.h>
#include <Engine/Classes/GameFramework/HUD.h>

#include <Editor/Public/EngineGUI.h>
#include <Editor/Public/EngineGUIWindow.h>

#include <Rendering/Public/Shader/EngineShader.h>
#include <NavigationSystem/Public/NavigationSystem.h>

#include "Public/GameModes//TitleGameMode.h"
#include "Public/GameModes//E1M1GameMode.h"
#include "Public/GameModes/NavMeshGameMode.h"

#include "Public/Characters/DoomGuy.h"
#include "Public/Editor/GUIEditor.h"
#include "Public/NavigationSystem/NavMeshResource.h"
#include "Public/Global/GameInstance/DoomGameInstance.h"
#include "Public/UI/HUD/TitleHUD.h"
#include "Public/UI/HUD/E1M1HUD.h"


CreateContentsCoreDefine(UDoomCore);

UDoomCore::UDoomCore()
{

}

UDoomCore::~UDoomCore()
{
}

void UDoomCore::EngineStart(UEngineInitData& Data)
{
	GEngine->CreateGameInstance<UDoomGameInstance>();

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

		UPaperSprite::CreateSpriteToMeta("Player.png", ".sdata");
		UPaperSprite::CreateSpriteToMeta("DoomGuyHUD.png", ".sdata");

		UPaperSprite::CreateSpriteToCount("Shotgun.png", 8, 1);
		UPaperSprite::CreateSpriteToCount("Pistol.png", 5, 1);
		UPaperSprite::CreateSpriteToCount("Potion.png", 4, 1);
		UPaperSprite::CreateSpriteToCount("NormalArmor.png", 2, 1);
		UPaperSprite::CreateSpriteToCount("Weapons.png", 7, 1);
		UPaperSprite::CreateSpriteToCount("DrumContainer.png", 7, 1);
		UPaperSprite::CreateSpriteToCount("DoomImpFire.png", 5, 1);

		UPaperSprite::CreateSpriteToCount("DoomZombie.png", 6, 12);
		UPaperSprite::CreateSpriteToCount("DoomZombieSgt.png", 6, 12);
		UPaperSprite::CreateSpriteToCount("DoomImp.png", 7, 11);

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
}

void UDoomCore::EngineLevelStart()
{
	//UGameEngine::CreateLevel<ATitleGameMode, AActor, AHUD>("TitleLevel");
	UGameEngine::CreateLevel<AE1M1GameMode, ADoomGuyCharacter, AE1M1HUD>("E1M1Level");
	//UGameEngine::CreateLevel<ANavMeshGameMode, APawn, ATitleHUD>("NavMeshLevel");
	UGameEngine::OpenLevel("E1M1Level");

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
