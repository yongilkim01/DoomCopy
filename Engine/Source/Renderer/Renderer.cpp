#include "pch.h"
#include "Renderer.h"

#include "Classes/Engine/Level.h"
#include "Core/Misc/DirectoryHelper.h"
#include "Core/Misc/FileHelper.h"
#include "Core/Containers/EngineString.h"
#include "Core/EngineCore.h"
#include "Classes/Camera/CameraComponent.h"
#include "Core/Containers/EngineString.h"

#include "ThirdParty/DirectxTex/Include/DirectXTex.h"

#pragma comment(lib, "DirectXTex.lib")

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::BeginPlay()
{
	SetOrder(0);

	InputAssembler1Init();
	VertexShaderInit();
	InputAssembler2Init();
	RasterizerInit();
	PixelShaderInit();
	ShaderResInit();
}

void URenderer::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& RendererTransform = GetComponentTransformRef();

	RendererTransform.View = CameraTransform.View;
	RendererTransform.Projection = CameraTransform.Projection;

	RendererTransform.WVP = RendererTransform.World * RendererTransform.View * RendererTransform.Projection;

	// Rendering pipeline
	ShaderResSetting();
	InputAssembler1Setting();
	VertexShaderSetting();
	InputAssembler2Setting();
	RasterizerSetting();
	PixelShaderSetting();
	OutPutMergeSetting();

	UEngineCore::Device.GetDeviceContext()->DrawIndexed(6, 0, 0);

}

void URenderer::InputAssembler1Init()
{
	std::vector<EngineVertex> Vertexes;
	Vertexes.resize(4);

	Vertexes[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.0f), {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} };
	Vertexes[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.0f), {1.0f, 0.0f} , {0.0f, 1.0f, 0.0f, 1.0f} };
	Vertexes[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.0f), {0.0f, 1.0f} , {0.0f, 0.0f, 1.0f, 1.0f} };
	Vertexes[3] = EngineVertex{ FVector(0.5f, -0.5f, -0.0f), {1.0f, 1.0f} , {1.0f, 1.0f, 1.0f, 1.0f} };

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = sizeof(EngineVertex) * Vertexes.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&desc, &Data, &VertexBuffer))
	{
		MSGASSERT("버텍스 버퍼 생성 실패");
		return;
	}

}

void URenderer::InputAssembler1Setting()
{
	UINT VertexSize = sizeof(EngineVertex);
	UINT Offset = 0;

	ID3D11Buffer* ArrayBuffer[1];
	ArrayBuffer[0] = VertexBuffer.Get();

	UEngineCore::Device.GetDeviceContext()->IASetVertexBuffers(0, 1, ArrayBuffer, &VertexSize, &Offset);
	UEngineCore::Device.GetDeviceContext()->IASetInputLayout(InputLayout.Get());
}

void URenderer::InputAssembler1Layout()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;

	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "POSITION";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 0;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayoutData.push_back(Desc);
	}

	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "TEXCOORD";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 16;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

		// 인스턴싱을 설명할때 이야기 하겠습니다.
		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayoutData.push_back(Desc);
	}

	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 32;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayoutData.push_back(Desc);
	}

	HRESULT Result = UEngineCore::Device.GetDevice()->CreateInputLayout(
		&InputLayoutData[0],
		static_cast<unsigned int>(InputLayoutData.size()),
		VSShaderCodeBlob->GetBufferPointer(),
		VSShaderCodeBlob->GetBufferSize(),
		&InputLayout);

	if (S_OK != Result)
	{
		MSGASSERT("인풋 레이아웃 생성 실패");
	}
}

void URenderer::VertexShaderInit()
{
	//std::string Path = "D:/Projects/DoomCopy/Engine/Shaders/SpriteShader.fx";
	std::string Path = "C:/Users/yongi/OneDrive/projects/DoomCopy/Engine/Shaders/SpriteShader.fx";
	//std::string Path = "../../../../../Shaders/SpriteShader.fx";
	//std::string Path = "D:\Projects\DoomCopy\Engine\Source\Renderer\Renderer.cpp";

	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	std::string VSVersion = "vs_5_0";

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif
	
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr,
		nullptr,
		"VertexToWorld",
		VSVersion.c_str(),
		Flag0,
		Flag1,
		&VSShaderCodeBlob,
		&VSErrorCodeBlob);

	if (nullptr == VSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(VSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("버텍스 쉐이더 생성 실패 Error code : " + ErrString);
		return;
	}

	// 버텍스 쉐이더 생성
	HRESULT Result = UEngineCore::Device.GetDevice()->CreateVertexShader(
		VSShaderCodeBlob->GetBufferPointer(),
		VSShaderCodeBlob->GetBufferSize(),
		nullptr,
		&VertexShader);

	if (S_OK != Result)
	{
		MSGASSERT("버텍스 쉐이더 생성 실패");
		return;
	}
	
	InputAssembler1Layout();
}

void URenderer::VertexShaderSetting()
{
	UEngineCore::Device.GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void URenderer::InputAssembler2Init()
{
	std::vector<unsigned int> Indexes;

	Indexes.push_back(0);
	Indexes.push_back(1);
	Indexes.push_back(2);
		 
	Indexes.push_back(1);
	Indexes.push_back(3);
	Indexes.push_back(2);

	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = sizeof(unsigned int) * static_cast<int>(Indexes.size());
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	Desc.CPUAccessFlags = 0;
	Desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Indexes[0];
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, &Data, &IndexBuffer))
	{
		MSGASSERT("인덱스 버퍼 생성 실패");
		return;
	}
}

void URenderer::InputAssembler2Setting()
{
	int Offset = 0;

	UEngineCore::Device.GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, Offset);

	UEngineCore::Device.GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void URenderer::RasterizerInit()
{
	D3D11_RASTERIZER_DESC Desc = {};

	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	UEngineCore::Device.GetDevice()->CreateRasterizerState(&Desc, RasterizerState.GetAddressOf());

	ViewPortInfo.Height = 720.0f;
	ViewPortInfo.Width = 1280.0f;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;
}

void URenderer::RasterizerSetting()
{
	UEngineCore::Device.GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::Device.GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void URenderer::PixelShaderInit()
{
	//std::string Path = "D:/Projects/DoomCopy/Engine/Shaders/SpriteShader.fx";
	std::string Path = "C:/Users/yongi/OneDrive/projects/DoomCopy/Engine/Shaders/SpriteShader.fx";

	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	// 버전을 만든다.
	std::string PSVersion = "ps_5_0";

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr, // Define TEST 등으로 전처리기를 넣을수.
		nullptr,
		"PixelToWorld",
		PSVersion.c_str(),
		Flag0,
		Flag1,
		&PSShaderCodeBlob,
		&PSErrorCodeBlob
	);

	if (nullptr == PSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(PSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("쉐이더 코드 중간빌드에서 실패했습니다\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::Device.GetDevice()->CreatePixelShader(
		PSShaderCodeBlob->GetBufferPointer(),
		PSShaderCodeBlob->GetBufferSize(),
		nullptr,
		&PixelShader
	);

	if (S_OK != Result)
	{
		MSGASSERT("픽셀 쉐이더 생성에 실패");
	}
}

void URenderer::PixelShaderSetting()
{
	UEngineCore::Device.GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

void URenderer::OutPutMergeSetting()
{

	ID3D11RenderTargetView* RTV = UEngineCore::Device.GetRenderTargetView();

	ID3D11RenderTargetView* ArrRtv[16] = { 0 };
	ArrRtv[0] = RTV; // SV_Target0

	UEngineCore::Device.GetDeviceContext()->OMSetRenderTargets(1, &ArrRtv[0], nullptr);
}

void URenderer::ShaderResInit()
{
	D3D11_BUFFER_DESC Desc = { 0 };
	Desc.ByteWidth = sizeof(FTransform);
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	Desc.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, nullptr, &TransformConstBuffer))
	{
		MSGASSERT("상수 버퍼에 생성 실패");
		return;
	}

	FDirectoryHelper CurDirectory;
	CurDirectory.MoveParentToDirectory("Resources");
	FFileHelper FileHelper = CurDirectory.GetFile("Player.png");

	std::string Str = FileHelper.GetPathToString();
	std::string Ext = FileHelper.GetExtension();

	std::wstring WLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());

	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage ImageData;

	if (UpperExt == ".DDS")
	{
		if (S_OK != DirectX::LoadFromDDSFile(WLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS 파일 로드에 실패했습니다.");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(WLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA 파일 로드에 실패했습니다.");
			return;
		}
	}
	else
	{
		if (S_OK != DirectX::LoadFromWICFile(WLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + "파일 로드에 실패했습니다.");
			return;
		}
	}

	// 
	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::Device.GetDevice(),
		ImageData.GetImages(),
		ImageData.GetImageCount(),
		ImageData.GetMetadata(),
		&ShaderResourceView
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패했습니다..");
		return;
	}

	D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };

	SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	UEngineCore::Device.GetDevice()->CreateSamplerState(&SampInfo, &SamplerState);
}

void URenderer::ShaderResSetting()
{
	FTransform& RendererTransform = GetComponentTransformRef();

	D3D11_MAPPED_SUBRESOURCE MappedSubResource = {};

	UEngineCore::Device.GetDeviceContext()->Map(TransformConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	if (nullptr == MappedSubResource.pData)
	{
		MSGASSERT("그래픽 카드 수정 권한 거부");
	}

	memcpy_s(MappedSubResource.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));

	UEngineCore::Device.GetDeviceContext()->Unmap(TransformConstBuffer.Get(), 0);

	ID3D11Buffer* ArrPtr[16] = { TransformConstBuffer.Get() };
	UEngineCore::Device.GetDeviceContext()->VSSetConstantBuffers(0, 1, ArrPtr);


	ID3D11ShaderResourceView* ArrSRV[16] = { ShaderResourceView.Get() };
	UEngineCore::Device.GetDeviceContext()->PSSetShaderResources(0, 1, ArrSRV);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::Device.GetDeviceContext()->PSSetSamplers(0, 1, ArrSMP);
}

void URenderer::SetOrder(int NewOrder)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(NewOrder);
	ULevel* Level = GetOwner()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}