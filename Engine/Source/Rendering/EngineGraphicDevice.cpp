#include "pch.h"
#include "EngineGraphicDevice.h"

#include "Rendering/Buffer/EngineVertex.h"
#include "Rendering/Buffer/VertexBuffer.h"
#include "Rendering/Buffer/IndexBuffer.h"
#include "Rendering/EngineBlend.h"
#include "Rendering/Shader/EngineShader.h"
#include "Rendering/Buffer/DepthStencilState.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"
#include "Engine/Public/Materials/Material.h"

#include "Engine/Classes/Engine/StaticMesh.h"
#include "Engine/Classes/Engine/Mesh.h"
#include "Engine/Classes/Engine/Texture.h"

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
	DXBackBufferTexture = nullptr;
	RenderTargetView = nullptr;
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
}

void UEngineGraphicDevice::InitTexture()
{
	// Sampler �ʱ�ȭ �� ����
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

	// ���� �⺻ �ؽ��� ����
	{
		FDirectoryHelper CurDir;

		CurDir.MoveEngineShaderDirectory();

		if (false == CurDir.MoveEngineShaderDirectory())
		{
			MSGASSERT("���� ���̴� ���� ã�⿡ �����߽��ϴ�");
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
	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("SpriteMaterial");

		Mat->SetVertexShader("EngineSpriteShader.fx");
		Mat->SetPixelShader("EngineSpriteShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("CollisionDebugMaterial");
		Mat->SetVertexShader("CollisionDebugShader.fx");
		Mat->SetPixelShader("CollisionDebugShader.fx");
		// ������ ȭ�鿡 ������ ������ �̳༮�� ���� ������ ����.
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
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
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_LESS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("BaseDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0 };
		Desc.DepthEnable = true;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		// ���̰��� �� ������ �������
		Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		Desc.StencilEnable = false;
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		UDepthStencilState::Create("CollisionDebugDepth", Desc);
	}
}

void UEngineGraphicDevice::RenderStart()
{
	FVector ClearColor;
	ClearColor = FVector(0.1f, 0.1f, 0.1f, 1.0f);
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), ClearColor.Arr1D);
	DeviceContext->ClearDepthStencilView(DepthTexture->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// ����Ÿ�� ����Ÿ�� ����Ÿ��
	// RTV�� DSV�� ��ģ ������ ����Ÿ���̶�� �θ���.
	// �װ� n�� ����ϰ� �Ǹ� ��Ƽ����Ÿ���̶�� �θ���.
	// ���⿡�ٰ� �ٽ� �׷���
	ID3D11RenderTargetView* RTV = UEngineCore::GetDevice().GetRenderTargetView();
	ID3D11RenderTargetView* ArrRtv[16] = { 0 };

	ArrRtv[0] = RTV; // SV_Target0

	DeviceContext->OMSetRenderTargets(1, &ArrRtv[0], DepthTexture->GetDepthStencilView());
}

void UEngineGraphicDevice::RenderEnd()
{
	HRESULT hr = SwapChain->Present(0, 0);

	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		MSGASSERT("�ػ� �����̳� ����̽� ���� ���� ��Ÿ�� ���� ����");
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
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,	// ���� �������� Adapter�� ����̽��� ����
		nullptr,									// Ư�� �ܰ�(�����Ͷ������̼�: ������ ���������� �Ϻ�)�� �ۼ��� DLL�� ��ü
		IFlag,
		nullptr,									// DirectX Feature Level ����
		0,											// �����ϴ� Feature Level ����
		D3D11_SDK_VERSION,
		&Device,
		&ResultLevl,
		&DeviceContext);

	if (nullptr == Device)
	{
		MSGASSERT("�׷��� ����̽� ������ �����߽��ϴ�.");
		return;
	}

	if (nullptr == DeviceContext)
	{
		MSGASSERT("�׷��� ���ؽ�Ʈ ����̽� ������ �����߽��ϴ�.");
		return;
	}

	HR = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	if (HR != S_OK)
	{
		MSGASSERT("��Ƽ������ ���� ����");
		return;
	}

	InitDefaultResources();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& EngineWindow)
{
	FVector Size = EngineWindow.GetWindowSize();

	{
		D3D11_TEXTURE2D_DESC TextureDesc = { 0 };

		TextureDesc.ArraySize = 1;
		TextureDesc.Width = Size.iX();
		TextureDesc.Height = Size.iY();
		TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		TextureDesc.SampleDesc.Count = 1;
		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.MipLevels = 1;
		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

		DepthTexture = std::make_shared<UTexture>();

		DepthTexture->CreateAsset(TextureDesc);

	}

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 2;
	desc.BufferDesc.Width = Size.iX();
	desc.BufferDesc.Height = Size.iY();
	desc.OutputWindow = EngineWindow.GetWindowHandle();
	desc.Windowed = true;
	desc.BufferDesc.RefreshRate.Denominator = 1;									// �ֻ���
	desc.BufferDesc.RefreshRate.Numerator = 60;										// 1�ʿ� 60ȸ ������ �� �� ������ �ض�
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;							// ����� �� ����
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// ����ͳ� �����쿡 �ȼ��� ���ŵǴ� ������ ���� �����ɷ� ����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;	// BackBuffer�� ȭ�鿡 �׷����� �뵵�� ���̴����� ����� �� �ִ� �뵵�� ����
	desc.SampleDesc.Quality = 0;													// ���ø��� �ȼ� ���̴��� ����
	desc.SampleDesc.Count = 1;														// ���ø� �� ����
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;										// ���� ������ �ϴ� ����� ������ ������� �غ�Ǵ� ������ ����
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIFactory* PtrFactory = nullptr;

	MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&PtrFactory));

	PtrFactory->CreateSwapChain(Device.Get(), &desc, &SwapChain);
	PtrFactory->Release();
	MainAdapter->Release();

	if (nullptr == SwapChain)
	{
		MSGASSERT("����ü�� ���ۿ� �����߽��ϴ�.");
	}

	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &DXBackBufferTexture))
	{
		MSGASSERT("����� �ؽ�ó �ε� ����");
	}

	if (S_OK != Device->CreateRenderTargetView(DXBackBufferTexture.Get(), nullptr, &RenderTargetView))
	{
		MSGASSERT("�ؽ�ó �������� �Ҵ� ����");
	}
}

// ���� �����ս��� ���� �׷��� ��ġ �ϵ��� ã�� �޼ҵ�
IDXGIAdapter* UEngineGraphicDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* ResultAdapter = nullptr;

	unsigned __int64 MaxGraphicDeviceMemory = 0;

	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (nullptr == Factory)
	{
		MSGASSERT("�׷��� ī�� ���� ���丮 ���� ����");
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

		// ����� ���� ����ü
		DXGI_ADAPTER_DESC Desc;
		CurAdapter->GetDesc(&Desc);

		// TODO: VRAM ũ�⿡ ���� �� ���� ���� ��ȯ
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
		MSGASSERT("�׷���ī�尡�� ã�� ���߽��ϴ�.");
		return nullptr;
	}

	return ResultAdapter;
}
