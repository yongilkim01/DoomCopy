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

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

/*
	렌더링 파이프 라인

	-------------------
	|   Vertex Data	  |
	-------------------
*/

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
	VertexBuffer = nullptr;
	VSShaderCodeBlob = nullptr;
	VSErrorCodeBlob = nullptr;
}

void URenderer::BeginPlay()
{
	SetOrder(0);

	// 버텍스 버퍼 생성
	InitVertexBuffer();

	// 버텍스 셰이더 생성
	InitVertexShader();

	// 인덱스 셰이더 생성
	InitIndexBuffer();


	InitRasterizer();
	InitPixelShader();
	InitShaderResourceView();
}

void URenderer::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& RendererTransform = GetComponentTransformRef();

	RendererTransform.View = CameraTransform.View;
	RendererTransform.Projection = CameraTransform.Projection;

	RendererTransform.WVP = RendererTransform.World * RendererTransform.View * RendererTransform.Projection;

	// Rendering pipeline
	UpdateShaderResourceView();
	UpdateVertexBuffer();
	UpdateVertexShader();
	UpdateIndexBuffer();
	UpdateRasterizer();
	UpdatePixelShader();
	UpdateRenderTargetView();

	UEngineCore::Device.GetDeviceContext()->DrawIndexed(6, 0, 0);

}

void URenderer::InitVertexBuffer()
{
	// Vertex 데이터를 저장할 벡터 생성 및 크기 조절
	std::vector<EngineVertex> Vertexes;
	Vertexes.resize(4);

	// 각 Vertex의 위치, 텍스처 좌표 및 색상을 설정
	Vertexes[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
	Vertexes[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
	Vertexes[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
	Vertexes[3] = EngineVertex{ FVector(0.5f, -0.5f, -0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

	// 버텍스 버퍼 설명 구조체 초기화
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));

	Desc.ByteWidth = sizeof(EngineVertex) * Vertexes.size(); // 버퍼의 크기를 설정
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				 // 버퍼의 바인딩 플래그를 버텍스 버퍼로 설정
	Desc.CPUAccessFlags = 0;								 // CPU 접근 플래그를 설정 (기본값)
	Desc.Usage = D3D11_USAGE_DEFAULT;						 // 버퍼의 사용 방식을 설정

	// 버퍼에 초기 데이터 설정
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	// 디바이스를 사용하여 버텍스 버퍼를 생성
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, &Data, VertexBuffer.GetAddressOf()))
	{
		MSGASSERT("버텍스 버퍼 생성 실패");
		return;
	}


}

void URenderer::UpdateVertexBuffer()
{
	// 버텍스의 크기와 오프셋을 설정
	UINT VertexSize = sizeof(EngineVertex); // 버텍스 하나의 크기를 설정
	UINT Offset = 0; // 버텍스 데이터의 시작 오프셋을 설정 (0으로 설정)

	// 버텍스 버퍼 배열 생성
	ID3D11Buffer* ArrayBuffer[1];
	ArrayBuffer[0] = VertexBuffer.Get(); // 생성된 버텍스 버퍼를 배열에 저장

	// 버텍스 버퍼를 입력 어셈블러(IA)에 설정
	UEngineCore::Device.GetDeviceContext()->IASetVertexBuffers(
		0,            // 입력 슬롯의 시작 인덱스 (슬롯 0부터 시작)
		1,            // 설정할 버퍼의 개수 (여기서는 1개)
		ArrayBuffer,  // 버퍼 배열의 포인터
		&VertexSize,  // 각 버텍스의 크기를 설정
		&Offset       // 버퍼 내의 데이터 시작 오프셋을 설정
	);

	// 입력 레이아웃을 입력 어셈블러(IA)에 설정
	UEngineCore::Device.GetDeviceContext()->IASetInputLayout(InputLayout.Get());

}

void URenderer::InitVertexLayout()
{
	// Vertex 입력 레이아웃을 저장할 벡터 생성
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutData;

	// POSITION 입력 요소 설명 추가
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "POSITION"; // 의미론 이름
		Desc.InputSlot = 0; // 입력 슬롯
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 데이터 형식
		Desc.AlignedByteOffset = 0; // 데이터의 바이트 오프셋
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // 입력 슬롯 클래스
		Desc.SemanticIndex = 0; // 의미론 인덱스
		Desc.InstanceDataStepRate = 0; // 인스턴스 데이터 단계 속도
		InputLayoutData.push_back(Desc); // 입력 레이아웃 데이터에 추가
	}

	// TEXCOORD 입력 요소 설명 추가
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "TEXCOORD"; // 의미론 이름
		Desc.InputSlot = 0; // 입력 슬롯
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 데이터 형식
		Desc.AlignedByteOffset = 16; // 데이터의 바이트 오프셋
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // 입력 슬롯 클래스

		// 인스턴싱 설명
		Desc.SemanticIndex = 0; // 의미론 인덱스
		Desc.InstanceDataStepRate = 0; // 인스턴스 데이터 단계 속도
		InputLayoutData.push_back(Desc); // 입력 레이아웃 데이터에 추가
	}

	// COLOR 입력 요소 설명 추가
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR"; // 의미론 이름
		Desc.InputSlot = 0; // 입력 슬롯
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 데이터 형식
		Desc.AlignedByteOffset = 32; // 데이터의 바이트 오프셋
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // 입력 슬롯 클래스
		Desc.SemanticIndex = 0; // 의미론 인덱스
		Desc.InstanceDataStepRate = 0; // 인스턴스 데이터 단계 속도
		InputLayoutData.push_back(Desc); // 입력 레이아웃 데이터에 추가
	}

	// 입력 레이아웃 생성
	HRESULT Result = UEngineCore::Device.GetDevice()->CreateInputLayout(
		&InputLayoutData[0],                        // 입력 레이아웃 데이터 포인터
		static_cast<unsigned int>(InputLayoutData.size()), // 입력 레이아웃 요소 개수
		VSShaderCodeBlob->GetBufferPointer(),       // 쉐이더 코드 포인터
		VSShaderCodeBlob->GetBufferSize(),          // 쉐이더 코드 크기
		&InputLayout);                              // 생성된 입력 레이아웃 객체

	// 입력 레이아웃 생성 실패 시 오류 처리
	if (S_OK != Result)
	{
		MSGASSERT("인풋 레이아웃 생성 실패");
	}

}

void URenderer::InitVertexShader()
{
	// 현재 디렉토리 헬퍼 객체 생성
	FDirectoryHelper CurDir;
	// 엔진 쉐이더 디렉토리로 이동
	CurDir.MoveEngineShaderDirectory();
	// 쉐이더 파일을 가져옴
	FFileHelper VSFile = CurDir.GetFile("SpriteShader.fx");
	// 파일 경로를 문자열로 변환
	std::string Path = VSFile.GetPathToString();
	// 문자열 경로를 와이드 문자열로 변환
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);
	// 버텍스 쉐이더 버전 설정
	std::string VSVersion = "vs_5_0";

	// 플래그 변수 초기화
	int Flag0 = 0;
	int Flag1 = 0;

	// 디버그 모드에서 디버그 플래그 설정
#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// 행렬을 행 우선 방식으로 패킹하는 플래그 설정
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// 파일에서 쉐이더를 컴파일
	D3DCompileFromFile(
		WPath.c_str(),         // 쉐이더 파일 경로
		nullptr,               // 매크로 정의 (없음)
		nullptr,               // 포함 파일 핸들러 (없음)
		"VertexToWorld",       // 엔트리 포인트 함수 이름
		VSVersion.c_str(),     // 타겟 쉐이더 모델
		Flag0,                 // 컴파일 플래그 1
		Flag1,                 // 컴파일 플래그 2
		&VSShaderCodeBlob,     // 컴파일된 쉐이더 코드 블롭
		&VSErrorCodeBlob);     // 오류 코드 블롭

	// 컴파일된 쉐이더 코드 블롭이 없는 경우 오류 처리
	if (nullptr == VSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(VSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("버텍스 쉐이더 생성 실패 Error code : " + ErrString);
		return;
	}

	// 버텍스 쉐이더 생성
	HRESULT Result = UEngineCore::Device.GetDevice()->CreateVertexShader(
		VSShaderCodeBlob->GetBufferPointer(), // 쉐이더 코드 포인터
		VSShaderCodeBlob->GetBufferSize(),    // 쉐이더 코드 크기
		nullptr,                              // 클래스 링크 (없음)
		&VertexShader);                       // 생성된 쉐이더 객체

	// 쉐이더 생성 실패 시 오류 처리
	if (S_OK != Result)
	{
		MSGASSERT("버텍스 쉐이더 생성 실패");
		return;
	}

	// 입력 어셈블러 레이아웃 설정
	InitVertexLayout();

}

void URenderer::UpdateVertexShader()
{
	UEngineCore::Device.GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void URenderer::InitIndexBuffer()
{
	// 인덱스 데이터를 저장할 벡터 생성
	std::vector<unsigned int> Indexes;

	// 인덱스 데이터 추가
	Indexes.push_back(0);
	Indexes.push_back(1);
	Indexes.push_back(2);

	Indexes.push_back(1);
	Indexes.push_back(3);
	Indexes.push_back(2);

	// 인덱스 버퍼 설명 구조체 초기화
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = sizeof(unsigned int) * static_cast<int>(Indexes.size()); // 버퍼의 크기를 설정
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 버퍼의 바인딩 플래그를 설정 (인덱스 버퍼로 사용)
	Desc.CPUAccessFlags = 0; // CPU 접근 플래그를 설정 (기본값)
	Desc.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 사용 방식을 설정

	// 버퍼에 초기 데이터 설정
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Indexes[0]; // 초기 데이터 설정

	// 디바이스를 사용하여 인덱스 버퍼를 생성
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, &Data, &IndexBuffer))
	{
		MSGASSERT("인덱스 버퍼 생성 실패");
		return;
	}

}

void URenderer::UpdateIndexBuffer()
{
	int Offset = 0;

	UEngineCore::Device.GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, Offset);

	UEngineCore::Device.GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void URenderer::InitRasterizer()
{
	// 래스터라이저 상태 설명 구조체 초기화
	D3D11_RASTERIZER_DESC Desc = {};

	// 컬링 모드를 백 페이스(뒤쪽 면을 제거)로 설정
	// 폴리곤의 뒷면을 렌더링하지 않도록 하여 성능을 향상
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	// 폴리곤 채우기 모드를 솔리드로 설정
	// 폴리곤의 내부를 채우는 기본 렌더링 모드
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	// 디바이스를 사용하여 래스터라이저 상태를 생성
	// GPU가 폴리곤을 렌더링하는 방식을 정의
	UEngineCore::Device.GetDevice()->CreateRasterizerState(&Desc, RasterizerState.GetAddressOf());

	// 뷰포트 정보 설정
	// 뷰포트는 렌더링된 이미지가 화면에 그려질 영역을 정의합니다.
	ViewPortInfo.Width = 1280.0f; // 뷰포트의 너비를 1280으로 설정
	ViewPortInfo.Height = 720.0f; // 뷰포트의 높이를 720으로 설정
	ViewPortInfo.TopLeftX = 0.0f; // 뷰포트의 시작 X 좌표를 0으로 설정 (화면의 왼쪽 가장자리)
	ViewPortInfo.TopLeftY = 0.0f; // 뷰포트의 시작 Y 좌표를 0으로 설정 (화면의 상단 가장자리)
	ViewPortInfo.MinDepth = 0.0f; // 뷰포트의 최소 깊이를 0.0으로 설정 (가장 가까운 깊이)
	ViewPortInfo.MaxDepth = 1.0f; // 뷰포트의 최대 깊이를 1.0으로 설정 (가장 먼 깊이)


}

void URenderer::UpdateRasterizer()
{
	UEngineCore::Device.GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::Device.GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void URenderer::InitPixelShader()
{
	// 현재 디렉토리 헬퍼 객체 생성
	FDirectoryHelper CurDir;
	// 엔진 쉐이더 디렉토리로 이동
	CurDir.MoveEngineShaderDirectory();
	// 쉐이더 파일을 가져옴
	FFileHelper VSFile = CurDir.GetFile("SpriteShader.fx");
	// 파일 경로를 문자열로 변환
	std::string Path = VSFile.GetPathToString();

	// 문자열 경로를 와이드 문자열로 변환
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	// 쉐이더 버전을 정의 (픽셀 쉐이더 버전 5.0)
	std::string PSVersion = "ps_5_0";

	// 컴파일 플래그 초기화
	int Flag0 = 0;
	int Flag1 = 0;

	// 디버그 모드에서 디버그 플래그 설정
#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// 행렬을 행 우선 방식으로 패킹하는 플래그 설정
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// 파일에서 쉐이더를 컴파일
	D3DCompileFromFile(
		WPath.c_str(),         // 쉐이더 파일 경로
		nullptr,               // 매크로 정의 (없음)
		nullptr,               // 포함 파일 핸들러 (없음)
		"PixelToWorld",        // 엔트리 포인트 함수 이름
		PSVersion.c_str(),     // 타겟 쉐이더 모델
		Flag0,                 // 컴파일 플래그 1
		Flag1,                 // 컴파일 플래그 2
		&PSShaderCodeBlob,     // 컴파일된 쉐이더 코드 블롭
		&PSErrorCodeBlob);     // 오류 코드 블롭

	// 컴파일된 쉐이더 코드 블롭이 없는 경우 오류 처리
	if (nullptr == PSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(PSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("쉐이더 코드 중간빌드에서 실패했습니다\n" + ErrString);
		return;
	}

	// 픽셀 쉐이더 생성
	HRESULT Result = UEngineCore::Device.GetDevice()->CreatePixelShader(
		PSShaderCodeBlob->GetBufferPointer(), // 쉐이더 코드 포인터
		PSShaderCodeBlob->GetBufferSize(),    // 쉐이더 코드 크기
		nullptr,                              // 클래스 링크 (없음)
		&PixelShader);                        // 생성된 픽셀 쉐이더 객체

	// 쉐이더 생성 실패 시 오류 처리
	if (S_OK != Result)
	{
		MSGASSERT("픽셀 쉐이더 생성에 실패");
	}

}

void URenderer::UpdatePixelShader()
{
	UEngineCore::Device.GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

// 그리기 위한 백지 준비 과정
void URenderer::UpdateRenderTargetView()
{
	// 렌더 타겟 뷰 포인터를 가져옴
	ID3D11RenderTargetView* RenderTargetView = UEngineCore::Device.GetRenderTargetView();

	// 렌더 타겟 뷰 포인터 배열을 초기화
	ID3D11RenderTargetView* ArrRtv[16] = { 0 }; // 최대 16개의 렌더 타겟 뷰를 가질 수 있음
	ArrRtv[0] = RenderTargetView; // 첫 번째 렌더 타겟 뷰를 설정 (여기서는 SV_Target0)

	// 출력 머지(OM) 스테이지에서 렌더 타겟을 설정
	UEngineCore::Device.GetDeviceContext()->OMSetRenderTargets(
		1,            // 렌더 타겟 뷰의 수 (여기서는 1개)
		&ArrRtv[0],   // 렌더 타겟 뷰 배열의 포인터
		nullptr       // 깊이-스텐실 뷰 (여기서는 사용 안 함)
	);

}

void URenderer::InitShaderResourceView()
{
	// 상수 버퍼 생성
	{
		// 상수 버퍼 설명 구조체 초기화
		D3D11_BUFFER_DESC Desc = { 0 };
		Desc.ByteWidth = sizeof(FTransform); // 버퍼의 크기를 FTransform 구조체 크기로 설정
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 버퍼의 바인딩 플래그를 상수 버퍼로 설정
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; // CPU가 버퍼에 쓰기 접근 가능하도록 설정
		Desc.Usage = D3D11_USAGE_DYNAMIC; // 버퍼의 사용 방식을 동적으로 설정

		// 디바이스를 사용하여 상수 버퍼 생성
		if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&Desc, nullptr, &ConstantBuffer))
		{
			MSGASSERT("상수 버퍼에 생성 실패");
			return;
		}
	}

	// 현재 디렉토리 헬퍼 객체 생성
	FDirectoryHelper CurDirectory;
	// 부모 디렉토리로 이동하여 "Resources" 디렉토리 설정
	CurDirectory.MoveParentToDirectory("Resources");
	CurDirectory.MoveParentToDirectory("TitleLevel");
	// "Player.png" 파일을 가져옴
	FFileHelper FileHelper = CurDirectory.GetFile("TitleLogo.png");

	// 파일 경로를 문자열로 변환
	std::string Str = FileHelper.GetPathToString();
	// 파일 확장자를 가져옴
	std::string Ext = FileHelper.GetExtension();

	// 문자열 경로를 와이드 문자열로 변환
	std::wstring WLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	// 확장자를 대문자로 변환
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());

	DirectX::TexMetadata Metadata; // 텍스처 메타데이터 객체 생성
	DirectX::ScratchImage ImageData; // 텍스처 이미지 데이터를 저장할 객체 생성

	// 파일 확장자에 따라 적절한 함수로 파일 로드
	if (UpperExt == ".DDS")
	{
		// DDS 파일 로드
		if (S_OK != DirectX::LoadFromDDSFile(WLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS 파일 로드에 실패했습니다.");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		// TGA 파일 로드
		if (S_OK != DirectX::LoadFromTGAFile(WLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA 파일 로드에 실패했습니다.");
			return;
		}
	}
	else
	{
		// 그 외의 파일 형식(WIC 사용)
		if (S_OK != DirectX::LoadFromWICFile(WLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + "파일 로드에 실패했습니다.");
			return;
		}
	}

	// 쉐이더 리소스 뷰 생성
	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::Device.GetDevice(),	// 디바이스 객체
		ImageData.GetImages(),				// 이미지 데이터 배열
		ImageData.GetImageCount(),			// 이미지 개수
		ImageData.GetMetadata(),			// 이미지 메타데이터
		ShaderResourceView.GetAddressOf()   // 생성된 쉐이더 리소스 뷰
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패했습니다.");
		return;
	}

	// 샘플러 상태 설명 구조체 초기화 및 설정
	D3D11_SAMPLER_DESC SamplerDesc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // U 좌표 텍스처 래핑 모드 설정
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // V 좌표 텍스처 래핑 모드 설정
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // W 좌표 텍스처 래핑 모드 설정

	SamplerDesc.BorderColor[0] = 0.0f;
	SamplerDesc.BorderColor[1] = 0.0f;
	SamplerDesc.BorderColor[2] = 0.0f;
	SamplerDesc.BorderColor[3] = 0.0f;

	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	//SamplerDesc.MaxLOD = 0.0f;
	//SamplerDesc.MinLOD = 0.0f;


	// 디바이스를 사용하여 샘플러 상태 생성
	UEngineCore::Device.GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);

}

void URenderer::UpdateShaderResourceView()
{
	FTransform& RendererTransform = GetComponentTransformRef();

	D3D11_MAPPED_SUBRESOURCE MappedSubResource = {};

	UEngineCore::Device.GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	if (nullptr == MappedSubResource.pData)
	{
		MSGASSERT("그래픽 카드 수정 권한 거부");
	}

	memcpy_s(MappedSubResource.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));

	UEngineCore::Device.GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);

	ID3D11Buffer* ArrPtr[16] = { ConstantBuffer.Get() };
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