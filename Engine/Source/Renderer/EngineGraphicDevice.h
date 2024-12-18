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
 *	설명
 */
class UEngineGraphicDevice
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UEngineGraphicDevice();
	ENGINE_API ~UEngineGraphicDevice();

	/** 객체 값 복사 방지 */
	UEngineGraphicDevice(const UEngineGraphicDevice& Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& Other) noexcept = delete;

	/** 
	 *	Device, DeviceContext 초기화 메소드
	 */
	void CreateDeviceAndContext();
	/**
	 *	SwapChain을 이용한 Backbuffer 초기화 메소드
	 */
	void CreateBackBuffer(const UEngineWindow& EngineWindow);

	void Release();

	/**
	 * 더 좋은 퍼포먼스를 보일 수 있는 그래픽 장치 하드웨어를 찾는 메소드
	 */
	IDXGIAdapter* GetHighPerformanceAdapter();

protected:

private:
	ID3D11Device* Device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	IDXGISwapChain* BackBuffer = nullptr;

};