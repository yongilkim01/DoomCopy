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
		MSGASSERT("해상도 변경이나 디바이스 관련 설정 런타임 도중 수정");
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

	if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(DXBackBufferTexture.GetAddressOf())))
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
