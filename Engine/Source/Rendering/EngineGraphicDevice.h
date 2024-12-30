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

	/**
	 *	Device, DeviceContext �ʱ�ȭ �޼ҵ�
	 */
	void CreateDeviceAndContext();
	/**
	 *	SwapChain�� �̿��� Backbuffer �ʱ�ȭ �޼ҵ�
	 *
	 *	@param EngineWindow - ����۸� �Ҵ��� ������ Ŭ����
	 */
	void CreateBackBuffer(const UEngineWindow& EngineWindow);
	/**
	 * �� ���� �����ս��� ���� �� �ִ� �׷��� ��ġ �ϵ��� ã�� �޼ҵ�
	 */
	IDXGIAdapter* GetHighPerformanceAdapter();
	/**
	 *	������ ���� ó���� �ִ� �޼ҵ�
	 */
	void RenderStart();
	/**
	 *	������ ���� �������� �ִ� �޼ҵ�
	 */
	void RenderEnd();

	ENGINE_API void Release();
	ENGINE_API void InitDefaultResources();
	ENGINE_API void InitMesh();
	ENGINE_API void InitBlend();


	/** ��, �� �޼ҵ� */
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