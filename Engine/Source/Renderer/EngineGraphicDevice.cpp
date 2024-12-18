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
	if (nullptr != DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext = nullptr;
	}

	if (nullptr != Device)
	{
		Device->Release();
		Device = nullptr;
	}
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
	IDXGIAdapter* Adapter =  GetHighPerformanceAdapter();

	int IFlag = 0;

#ifdef _DEBUG
	IFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL ResultLevl;

	HRESULT HR = D3D11CreateDevice(
		Adapter,
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
	}

	Adapter->Release();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& EngineWindow)
{
	int a = 0;
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
