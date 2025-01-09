#include "pch.h"
#include "DoomCore.h"

#include "GameMode/TitleGameMode.h"
#include "GameMode/E1M1GameMode.h"

#include "Utils/NavMesh/GameMode/NavMeshGameMode.h"
#include "Utils/GUI/GUIEditor.h"
#include "Utils/NavMesh/Actor/NavMeshResource.h"

#include <Core/Math/EngineMath.h>
#include <Core/EngineCore.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>
#include <Classes/Engine/Level.h>
#include <Classes/Engine/Texture.h>
#include <Classes/Engine/PaperSprite.h>
#include <GameFramework/Actor.h>
#include <Tools/DebugGUI/EngineGUI.h>
#include <Tools/DebugGUI/EngineGUIWindow.h>
#include <Rendering/Shader/EngineShader.h>
#include <Core/Materials/Material.h>

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

	{
		FDirectoryHelper DirectoryHelper;
		if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
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

	InitContentsRenderingResource();

	UPaperSprite::CreateSpriteToMeta("Player.png", ".sdata");

	//{
	//	FDirectoryHelper DirectoryHelper;
	//	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	//	{
	//		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
	//		return;
	//	}

	//	DirectoryHelper.Append("Images/Textures");

	//	UPaperSprite::CreateSpriteToFolder(DirectoryHelper.GetPathToString());
	//}

	{
		FDirectoryHelper Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Images/Tevi");

		UPaperSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}

	//UEngineCore::CreateLevel<ATitleGameMode, AActor>("TitleLevel");
	UEngineCore::CreateLevel<AE1M1GameMode, APawn>("E1M1Level");
	UEngineCore::CreateLevel<ANavMeshGameMode, APawn>("NavMeshLevel");
	UEngineCore::OpenLevel("NavMeshLevel");

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
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("NavMeshTestMaterial");
		Mat->SetVertexShader("NavMeshTestShader.fx");
		Mat->SetPixelShader("NavMeshTestShader.fx");
	}
	{
		UNavMeshResource::Create("NavMapResource");
	}

	//{
	//	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyCollisionDebugMaterial");
	//	Mat->SetVertexShader("EngineDebugCollisionShader.fx");
	//	Mat->SetPixelShader("EngineDebugCollisionShader.fx");
	//	// 언제나 화면에 나오는 누구도 이녀석의 앞을 가릴수 없어.
	//	Mat->SetDepthStencilState("CollisionDebugDepth");
	//	Mat->SetRasterizerState("CollisionDebugRas");
	//}
}

void UDoomCore::InitWindowSize(UEngineInitData& Data)
{
	float width = GetSystemMetrics(SM_CXSCREEN) - WindowWidth;
	float height = GetSystemMetrics(SM_CYSCREEN) - WindowHeight;

	Data.WindowPosition = { width / 2.0f , height / 2.0f };

	Data.WindowSize = { WindowWidth, WindowHeight };
}