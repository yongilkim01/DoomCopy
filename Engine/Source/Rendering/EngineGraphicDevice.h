#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"

#include <wrl.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "Platform/Window.h"

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
	 *
	 *	@param EngineWindow - 백버퍼를 할당할 윈도우 클래스
	 */
	void CreateBackBuffer(const UEngineWindow& EngineWindow);
	/**
	 * 더 좋은 퍼포먼스를 보일 수 있는 그래픽 장치 하드웨어를 찾는 메소드
	 */
	IDXGIAdapter* GetHighPerformanceAdapter();
	/**
	 *	렌더러 과정 처음에 넣는 메소드
	 */
	void RenderStart();
	/**
	 *	렌더러 과정 마지막에 넣는 메소드
	 */
	void RenderEnd();

	ENGINE_API void Release();
	ENGINE_API void InitTexture();
	ENGINE_API void InitDefaultResources();
	ENGINE_API void InitMesh();
	ENGINE_API void InitBlend();
	ENGINE_API void	InitShader();
	ENGINE_API void InitMaterial();
	ENGINE_API void InitRasterizerState();



	/** 겟, 셋 메소드 */
	ENGINE_API ID3D11Device* GetDevice()
	{
		return Device.Get();
	}
	ENGINE_API ID3D11DeviceContext* GetDeviceContext()
	{
		return DeviceContext.Get();
	}
	ENGINE_API ID3D11RenderTargetView* GetRenderTargetView()
	{
		return RenderTargetView.Get();
	}

protected:

private:
	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> MainAdapter = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DXBackBufferTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr;
};