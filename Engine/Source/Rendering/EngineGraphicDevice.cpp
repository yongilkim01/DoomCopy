#include "pch.h"
#include "EngineGraphicDevice.h"

#include "Rendering/EngineVertex.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/EngineBlend.h"
#include "Rendering/Shader/EngineShader.h"
#include "Rendering/EngineMaterial.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"

#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Mesh.h"
#include "Classes/Engine/Texture.h"



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
	DXBackBufferTexture = nullptr;
	RenderTargetView = nullptr;
	SwapChain = nullptr;
	DeviceContext = nullptr;
	Device = nullptr;
}


void UEngineGraphicDevice::InitDefaultResources()
{
	InitTexture();
	InitMesh();
	InitBlend();
	InitRasterizerState();
	InitShader();
	InitMaterial();
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
	//{
	//	FDirectoryHelper DirectoryHelper;
	//	if (false == DirectoryHelper.MoveParentToDirectory("Resources"))
	//	{
	//		MSGASSERT("리소스 폴더를 찾기에 실패했습니다");
	//		return;
	//	}

	//	DirectoryHelper.Append("Models\\E1M1");

	//	std::string Path = DirectoryHelper.GetPathToString();

	//	LoadModel(Path + "\\doom_E1M1.obj", Path + "\\doom_E1M1.mtl");

	//	int MeshVectorSize = Meshes.size();

	//	for (int i = 0; i < Meshes.size(); i++)
	//	{
	//		std::string MeshName = "Map" + std::to_string(i + 1);

	//		std::vector<EngineVertex> Vertexs;

	//		for (int j = 0; j < Meshes[i].vertices_.size(); j++)
	//		{
	//			EngineVertex Vertex = EngineVertex{ FVector(
	//				Meshes[i].vertices_[j].X,
	//				Meshes[i].vertices_[j].Y,
	//				Meshes[i].vertices_[j].Z),
	//				{0.0f , 0.0f},
	//				{1.0f, 1.0f, 1.0f, 1.0f}};

	//			Vertexs.push_back(Vertex);
	//		}

	//		FVertexBuffer::Create(MeshName, Vertexs);


	//		std::vector<unsigned int> Indexs;

	//		for (int j = 0; j < Meshes[i].indices_.size(); j++)
	//		{
	//			Indexs.push_back(Meshes[i].indices_[j]);
	//		}

	//		FIndexBuffer::Create(MeshName, Indexs);

	//		UStaticMesh::Create(MeshName);

	//	}

	//	int TextureVectorSize = Textures.size();
	//}


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
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
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
	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");

	Mat->SetVertexShader("EngineSpriteShader.fx");
	Mat->SetPixelShader("EngineSpriteShader.fx");
}

void UEngineGraphicDevice::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC Desc = {};

	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	UEngineRasterizerState::Create("EngineBase", Desc);
}

void UEngineGraphicDevice::RenderStart()
{
	FVector ClearColor;
	ClearColor = FVector(0.1f, 0.1f, 0.1f, 1.0f);
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), ClearColor.Arr1D);
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

	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &DXBackBufferTexture))
	{
		MSGASSERT("백버퍼 텍스처 로드 실패");
	}

	if (S_OK != Device->CreateRenderTargetView(DXBackBufferTexture.Get(), nullptr, &RenderTargetView))
	{
		MSGASSERT("텍스처 수정권한 할당 실패");
	}
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
