#include "pch.h"
#include "EngineGraphicDevice.h"

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

void UEngineGraphicDevice::RenderStart()
{
	FVector ClearColor;
	ClearColor = FVector(0.0f, 0.0f, 1.0f, 1.0f);
	DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), ClearColor.Arr1D);
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
	MainAdapter =  GetHighPerformanceAdapter();

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

	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(DXBackBufferTexture.GetAddressOf())))
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
