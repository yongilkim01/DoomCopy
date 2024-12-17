#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"

#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 

/**
 *	����
 */
class UEngineGraphicDevice
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API UEngineGraphicDevice();
	ENGINE_API ~UEngineGraphicDevice();

	/** ��ü �� ���� ���� */
	UEngineGraphicDevice(const UEngineGraphicDevice& Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& Other) noexcept = delete;

	void CreateDeviceAndContext();
	void CreateBackBuffer(const UEngineWindow& EngineWindow);
	void GetHighPerformanceAdapter();

protected:

private:
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* Context = nullptr;

};
