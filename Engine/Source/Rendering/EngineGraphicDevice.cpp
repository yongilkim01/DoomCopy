#include "pch.h"
#include "EngineGraphicDevice.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/Buffer/VertexBuffer.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/EngineBlend.h"
#include "Rendering/Shader/EngineShader.h"
#include "Rendering/Buffer/DepthStencilState.h"
#include "Rendering/Public/RenderTarget/RenderTarget.h"

#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"

#include "Engine/Public/Materials/Material.h"
#include "Engine/Classes/Engine/StaticMesh.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/Font.h"

#include "NavigationSystem/Public/NavigationSystem.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{

	Release();
}

void UEngineGraphicDevice::Release()
{
	MainAdapter = nullptr;
	SwapChain = nullptr;
	DeviceContext = nullptr;
	Device = nullptr;
}


void UEngineGraphicDevice::InitDefaultResources()
{
	InitDepthStencil();
	InitTexture();
	InitMesh();
	InitBlend();
	InitRasterizerState();
	InitShader();
	InitMaterial();
	InitFont();
}

void UEngineGraphicDevice::InitTexture()
{
	// Sampler 초기화 및 생성
	{
		D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };

		SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		SampInfo.BorderColor[0] = 0.0f;
		SampInfo.BorderColor[1] = 0.0f;
		SampInfo.BorderColor[2] = 0.0f;
		SampInfo.BorderColor[3] = 0.0f;

		// SampInfo.MaxLOD = 0.0f;
		// SampInfo.MinLOD = 0.0f;

		UEngineSampler::Create("WRAPSampler", SampInfo);
	}

	// 엔진 기본 텍스쳐 생성
	{
		FDirectoryHelper CurDir;

		CurDir.MoveEngineShaderDirectory();

		if (false == CurDir.MoveEngineShaderDirectory())
		{
			MSGASSERT("엔진 셰이더 폴더 찾기에 실패했습니다");
			return;
		}

		std::vector<FFileHelper> ImageFiles = CurDir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });

		for (int i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UTexture::Load(FilePath);
		}
	}
}

void UEngineGraphicDevice::InitMesh()
{
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(4);
		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		FVertexBuffer::Create("Rect", Vertexs);

		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		Indexs.push_back(1);
		Indexs.push_back(3);
		Indexs.push_back(2);

		FIndexBuffer::Create("Rect", Indexs);

		UMesh::Create("Rect");
	}
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(3);

		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(0.0f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };


		FVertexBuffer::Create("Triangle", Vertexs);

		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		FIndexBuffer::Create("Triangle", Indexs);

		UMesh::Create("Triangle");
	}
	{
		std::vector<EngineVertex> Vertexs;

		Vertexs.resize(4);

		Vertexs[0] = EngineVertex{ FVector(-1.0f, 1.0f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(1.0f, 1.0f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-1.0f, -1.0f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(1.0f, -1.0f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };
		
		FVertexBuffer::Create("FullRect", Vertexs);

		UMesh::Create("FullRect", "FullRect", "Rect");
	}

	{
		std::vector<EngineVertex> Vertex;
		Vertex.resize(24);
		// 앞면
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		// 뒷면
		Vertex[4] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(180) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[5] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(180)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[6] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(180) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[7] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(180), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		// 왼쪽
		Vertex[8] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[9] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[10] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[11] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		// 오른쪽
		Vertex[12] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[13] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[14] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[15] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		// 위
		Vertex[16] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[17] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[18] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[19] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		// 아래
		Vertex[20] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[21] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[22] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[23] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		
		FVertexBuffer::Create("Box", Vertex);

		std::vector<int> Index;
		// Index.resize(36);
		for (int i = 0; i < 6; i++)
		{
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 1);
			Index.push_back(i * 4 + 0);
			Index.push_back(i * 4 + 3);
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 0);
		}

		FIndexBuffer::Create("Box", Index);
		UMesh::Create("Box", "Box", "Box");
	}

	// Sphere
	{
		EngineVertex VertexData; // 정점을 정의하기 위한 구조체
		std::vector<EngineVertex> VBVector; // 정점 데이터를 저장하는 벡터
		std::vector<UINT> IBVector; // 인덱스 데이터를 저장하는 벡터
		float Radius = 0.5f; // 구의 반지름

		// 북극점 (구의 가장 윗부분)의 정점 데이터를 생성합니다.
		VertexData.POSITION = float4(0.0f, Radius, 0.0f, 1.0f); // 위치 설정 (y축으로 반지름만큼 떨어짐)
		VertexData.TEXCOORD = float4(0.5f, 0.0f); // 텍스처 좌표 설정 (중앙 위쪽)

		// 법선 벡터를 설정 (면에 수직으로 빛 반사를 계산하기 위함)
		VertexData.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		VertexData.NORMAL.Normalize(); // 벡터를 정규화 (길이를 1로 만듦)
		VertexData.NORMAL.W = 0.0f; // W 성분은 0으로 설정 (위치와 구분)

		// 접선 벡터와 비노말 벡터를 설정 (법선과 수직인 벡터들로 표면 디테일을 표현하기 위함)
		VertexData.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f); // 접선 벡터
		VertexData.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f); // 비노말 벡터

		VBVector.push_back(VertexData); // 정점 데이터를 벡터에 추가

		// 스택과 슬라이스의 개수를 설정 (구를 분할하는 세로 및 가로 개수)
		UINT iStackCount = 16; // 수직 분할 개수
		UINT iSliceCount = 16; // 수평 분할 개수

		// 각 스택과 슬라이스 사이의 회전 각도를 계산
		float yRotAngle = FMath::PI / (float)iStackCount; // 세로 방향 회전 간격
		float zRotAngle = FMath::PI2 / (float)iSliceCount; // 가로 방향 회전 간격

		// UV 텍스처 좌표의 간격을 계산
		float yUvRatio = 1.0f / (float)iStackCount;
		float zUvRatio = 1.0f / (float)iSliceCount;

		// 북극에서 남극까지 정점 데이터를 생성
		for (UINT y = 1; y < iStackCount; ++y) // 북극과 남극 제외
		{
			float phi = y * yRotAngle; // 현재 스택의 세로 방향 각도
			for (UINT z = 0; z < iSliceCount + 1; ++z) // 슬라이스 개수만큼
			{
				float theta = z * zRotAngle; // 현재 슬라이스의 가로 방향 각도

				// 현재 위치를 계산 (구면 좌표)
				VertexData.POSITION = float4{
					Radius * sinf(phi) * cosf(theta),
					Radius * cosf(phi),
					Radius * sinf(phi) * sinf(theta),
					1.0f
				};

				// 텍스처 좌표를 설정
				VertexData.TEXCOORD = float4(yUvRatio * z, zUvRatio * y);

				// 법선 벡터를 정규화
				VertexData.NORMAL = VertexData.POSITION.NormalizeReturn();
				VertexData.NORMAL.W = 0.0f;

				// 접선 벡터 설정
				VertexData.TANGENT.X = -Radius * sinf(phi) * sinf(theta);
				VertexData.TANGENT.Y = 0.0f;
				VertexData.TANGENT.Z = Radius * sinf(phi) * cosf(theta);
				VertexData.TANGENT = VertexData.TANGENT.NormalizeReturn();
				VertexData.TANGENT.W = 0.0f;

				// 비노말 벡터 설정
				VertexData.BINORMAL = float4::Cross(VertexData.TANGENT, VertexData.NORMAL);
				VertexData.BINORMAL = VertexData.BINORMAL.NormalizeReturn();
				VertexData.BINORMAL.W = 0.0f;

				// 생성된 정점을 벡터에 추가
				VBVector.push_back(VertexData);
			}
		}

		// 남극점 (구의 가장 아래 부분)의 정점 데이터를 생성
		VertexData.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f); // 위치 설정 (y축으로 반지름만큼 아래)
		VertexData.TEXCOORD = float4(0.5f, 1.0f); // 텍스처 좌표 설정 (중앙 아래쪽)
		VertexData.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		VertexData.NORMAL.Normalize();
		VertexData.NORMAL.W = 0.0f;
		VertexData.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		VertexData.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);
		VBVector.push_back(VertexData);

		// 인덱스 데이터를 생성하여 삼각형 면을 정의
		IBVector.clear();

		// 북극점을 기준으로 삼각형 정의
		for (UINT i = 0; i < iSliceCount; i++)
		{
			IBVector.push_back(0); // 북극점
			IBVector.push_back(i + 2); // 현재 슬라이스의 점
			IBVector.push_back(i + 1); // 이전 슬라이스의 점
		}

		// 스택 사이의 사각형 면을 두 개의 삼각형으로 정의
		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			}
		}

		// 남극점을 기준으로 삼각형 정의
		UINT iBotIndex = (UINT)VBVector.size() - 1; // 남극점의 인덱스
		for (UINT i = 0; i < iSliceCount; i++)
		{
			IBVector.push_back(iBotIndex); // 남극점
			IBVector.push_back(iBotIndex - (i + 2)); // 현재 슬라이스의 점
			IBVector.push_back(iBotIndex - (i + 1)); // 이전 슬라이스의 점
		}

		// 생성된 정점 버퍼와 인덱스 버퍼를 활용해 구를 생성
		FVertexBuffer::Create("Sphere", VBVector);
		FIndexBuffer::Create("Sphere", IBVector);

		UMesh::Create("Sphere");
	}


}

void UEngineGraphicDevice::InitBlend()
{
	D3D11_BLEND_DESC BlendDesc = { 0 };

	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = true;
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	UEngineBlend::Create("AlphaBlend", BlendDesc);
}

void UEngineGraphicDevice::InitShader()
{
	FDirectoryHelper CurDir;
	CurDir.MoveEngineShaderDirectory();

	std::vector<FFileHelper> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}
}

void UEngineGraphicDevice::InitMaterial()
{
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");

		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetBlend("AlphaBlend");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("WidgetMaterial");
		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
		Mat->SetDepthStencilState("UIDepth");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("CollisionDebugShader.fx");
		Mat->SetPixelShader("CollisionDebugShader.fx");
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("RenderTargetMaterial");
		Mat->SetVertexShader("RenderTargetMergeShader.fx");
		Mat->SetPixelShader("RenderTargetMergeShader.fx");
		Mat->SetDepthStencilState("RenderTargetMergeDepth");
	}
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("LightMaterial");
		Mat->SetVertexShader("MeshLightShader.fx");
		Mat->SetPixelShader("MeshLightShader.fx");
	}
}

void UEngineGraphicDevice::InitRasterizerState()
{
	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		UEngineRasterizerState::Create("EngineBase", Desc);
	}

	{
		D3D11_RASTERIZER_DESC Desc = {};
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

		UEngineRasterizerState::Create("CollisionDebugRas", Desc);
	}
}

void UEngineGraphicDevice::InitDepthStencil()
{
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("BaseDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = false;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("UIDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// 깊이값이 더 작으면 통과시켜
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("CollisionDebugDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		UDepthStencilState::Create("RenderTargetMergeDepth", Desc);
	}
}

void UEngineGraphicDevice::InitFont()
{
	UFont::Load("궁서", "궁서");
}

void UEngineGraphicDevice::RenderStart()
{
	BackBufferRenderTarget->Clear();
	BackBufferRenderTarget->Setting();
}

void UEngineGraphicDevice::RenderEnd()
{
	HRESULT hr = SwapChain->Present(0, 0);

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		MSGASSERT("해상도 변경이나 디바이스 관련 설정 런타임 도중 수정");
		return;
	}
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
	MainAdapter = GetHighPerformanceAdapter();

	int IFlag = 0;

#ifdef _DEBUG
	IFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL ResultLevl;

	HRESULT HR = D3D11CreateDevice(
		MainAdapter.Get(),
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,	// 내가 전달해준 Adapter로 디바이스를 생성
		nullptr,									// 특정 단계(레스터라이제이션: 렌더링 파이프라인 일부)를 작성한 DLL로 대체
		IFlag,
		nullptr,									// DirectX Feature Level 지정
		0,											// 지원하는 Feature Level 개수
		D3D11_SDK_VERSION,
		&Device,
		&ResultLevl,
		&DeviceContext);

	if (nullptr == Device)
	{
		MSGASSERT("그래픽 디바이스 생성에 실패했습니다.");
		return;
	}

	if (nullptr == DeviceContext)
	{
		MSGASSERT("그래픽 콘텍스트 디바이스 생성에 실패했습니다.");
		return;
	}

	HR = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (HR != S_OK)
	{
		MSGASSERT("멀티스레드 지정 실패");
		return;
	}

	InitDefaultResources();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& EngineWindow)
{
	FVector Size = EngineWindow.GetWindowSize();

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 2;
	desc.BufferDesc.Width = Size.iX(); 
	desc.BufferDesc.Height = Size.iY();
	desc.OutputWindow = EngineWindow.GetWindowHandle();
	desc.Windowed = true;
	desc.BufferDesc.RefreshRate.Denominator = 1;									// 주사율
	desc.BufferDesc.RefreshRate.Numerator = 60;										// 1초에 60회 갱신을 할 수 있으면 해라
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// 백버퍼 색 범위
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// 모니터나 윈도우에 픽셀이 갱신되는 순서를 가장 빠른걸로 지정
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;	// BackBuffer를 화면에 그려지는 용도와 쉐이더에서 사용할 수 있는 용도로 지정
	desc.SampleDesc.Quality = 0;													// 샘플링은 픽셀 쉐이더와 관련
	desc.SampleDesc.Count = 1;														// 샘플링 점 개수
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;										// 버퍼 스왑을 하는 방법을 순번에 상관없이 준비되는 순으로 지정
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIFactory* PtrFactory = nullptr;

	MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&PtrFactory));

	PtrFactory->CreateSwapChain(Device.Get(), &desc, &SwapChain);
	PtrFactory->Release();
	MainAdapter->Release();

	if (nullptr == SwapChain)
	{
		MSGASSERT("스왑체인 제작에 실패했습니다.");
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBufferTexture = nullptr;
	
	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBufferTexture))
	{
		MSGASSERT("백버퍼 텍스처 로드 실패");
	}
	
	BackBufferRenderTarget = std::make_shared<URenderTarget>();
	BackBufferRenderTarget->CreateTarget(BackBufferTexture);
	BackBufferRenderTarget->CreateDepthStencil();

}

// 가장 퍼포먼스가 좋은 그래픽 장치 하드웨어를 찾는 메소드
IDXGIAdapter* UEngineGraphicDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* ResultAdapter = nullptr;

	unsigned __int64 MaxGraphicDeviceMemory = 0;

	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (nullptr == Factory)
	{
		MSGASSERT("그래픽 카드 조사 팩토리 생성 실패");
		return nullptr;
	}

	for (int Index = 0; ; ++Index)
	{
		IDXGIAdapter* CurAdapter = nullptr;
		Factory->EnumAdapters(Index, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		// 어댑터 정보 구조체
		DXGI_ADAPTER_DESC Desc;
		CurAdapter->GetDesc(&Desc);

		// TODO: VRAM 크기에 따라서 더 높은 쪽을 반환
		if (MaxGraphicDeviceMemory <= Desc.DedicatedVideoMemory)
		{
			MaxGraphicDeviceMemory = Desc.DedicatedVideoMemory;

			if (nullptr != ResultAdapter)
			{
				ResultAdapter->Release();
			}

			ResultAdapter = CurAdapter;
			continue;
		}

		CurAdapter->Release();
	}

	if (nullptr != Factory)
	{
		Factory->Release();
	}

	if (nullptr == ResultAdapter)
	{
		MSGASSERT("그래픽카드가를 찾지 못했습니다.");
		return nullptr;
	}

	return ResultAdapter;
}
