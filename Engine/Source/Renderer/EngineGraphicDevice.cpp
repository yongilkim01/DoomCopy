#include "pch.h"
#include "EngineGraphicDevice.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
	GetHighPerformanceAdapter();

#ifdef _DEBUG
#endif

}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& EngineWindow)
{
	int a = 0;
}

void UEngineGraphicDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;

	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

	if (nullptr == Factory)
	{
		MSGASSERT("�׷��� ī�� ���� ���丮 ���� ����");
		return;
	}

	for (int Index = 0; ; ++Index)
	{
		IDXGIAdapter* CurAdapter = nullptr;

		Factory->EnumAdapters(Index, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		Adapter = CurAdapter;
	}

	if (nullptr != Factory)
	{
		Factory->Release();
	}

	if (nullptr == Adapter)
	{
		MSGASSERT("�׷���ī�尡 �޷����� ���� ��ǻ��");
		return;
	}

	Adapter->Release();
}
