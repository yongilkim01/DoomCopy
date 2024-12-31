#pragma once
#include "EngineDefine.h"
#include "Platform/Window.h"

#include <wrl.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include "Platform/Window.h"
#include "AiMesh.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiTexture;

enum aiTextureType;


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

	bool LoadModel(std::string_view _objPath, std::string_view _mtlPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	AiMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TEXTURE> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

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

	std::string ObjPath = "";
	std::string MtlPath = "";

	std::vector<AiMesh> Meshes;
	std::vector<TEXTURE> Textures;
	std::string Directory = "";
};