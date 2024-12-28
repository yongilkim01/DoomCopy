#include "pch.h"
#include "LineComponent.h"

#include <Classes/Camera/CameraComponent.h>
#include <Core/Misc/DirectoryHelper.h>
#include <Core/Misc/FileHelper.h>

ULineComponent::ULineComponent()
{
}

ULineComponent::~ULineComponent()
{
	VertexBuffer = nullptr;
	VSShaderCodeBlob = nullptr;
	VSErrorCodeBlob = nullptr;
}

void ULineComponent::BeginPlay()
{
	USceneComponent::BeginPlay();

	SetOrder(0);

	InitVertexBuffer();
	InitVertexShader();
	InitIndexBuffer();
	InitRasterizer();
	InitPixelShader();
	InitShaderResourceView();
}

void ULineComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
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

	UEngineCore::GetDevice().GetDeviceContext()->DrawIndexed(2, 0, 0);
}

void ULineComponent::InitVertexBuffer()
{
	// Vertex 데이터를 저장할 벡터 생성 및 크기 조절
	std::vector<LineVertex> Vertexes;
	Vertexes.resize(4);

	// 각 Vertex의 위치, 텍스처 좌표 및 색상을 설정
	Vertexes[0] = LineVertex{ FVector(-0.5f, 0.0f, -0.0f), {1.0f, 0.0f, 0.0f, 1.0f} };
	Vertexes[1] = LineVertex{ FVector(0.5f, 0.0f, -0.0f), {0.0f, 1.0f, 0.0f, 1.0f} };

	// 버텍스 버퍼 설명 구조체 초기화
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = static_cast <UINT>(sizeof(LineVertex) * Vertexes.size()); // 버퍼의 크기를 설정
	Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				 // 버퍼의 바인딩 플래그를 버텍스 버퍼로 설정
	Desc.CPUAccessFlags = 0;								 // CPU 접근 플래그를 설정 (기본값)
	Desc.Usage = D3D11_USAGE_DEFAULT;						 // 버퍼의 사용 방식을 설정

	// 버퍼에 초기 데이터 설정
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexes[0];

	// 디바이스를 사용하여 버텍스 버퍼를 생성
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, &Data, VertexBuffer.GetAddressOf()))
	{
		MSGASSERT("버텍스 버퍼 생성 실패");
		return;
	}


}

void ULineComponent::UpdateVertexBuffer()
{
	// 버텍스의 크기와 오프셋을 설정
	UINT VertexSize = sizeof(LineVertex); // 버텍스 하나의 크기를 설정
	UINT Offset = 0; // 버텍스 데이터의 시작 오프셋을 설정 (0으로 설정)

	// 버텍스 버퍼 배열 생성
	ID3D11Buffer* ArrayBuffer[1];
	ArrayBuffer[0] = VertexBuffer.Get(); // 생성된 버텍스 버퍼를 배열에 저장

	// 버텍스 버퍼를 입력 어셈블러(IA)에 설정
	UEngineCore::GetDevice().GetDeviceContext()->IASetVertexBuffers(
		0,            // 입력 슬롯의 시작 인덱스 (슬롯 0부터 시작)
		1,            // 설정할 버퍼의 개수 (여기서는 1개)
		ArrayBuffer,  // 버퍼 배열의 포인터
		&VertexSize,  // 각 버텍스의 크기를 설정
		&Offset       // 버퍼 내의 데이터 시작 오프셋을 설정
	);

	// 입력 레이아웃을 입력 어셈블러(IA)에 설정
	UEngineCore::GetDevice().GetDeviceContext()->IASetInputLayout(InputLayout.Get());

}

void ULineComponent::InitVertexLayout()
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

	// COLOR 입력 요소 설명 추가
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR"; // 의미론 이름
		Desc.InputSlot = 0; // 입력 슬롯
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 데이터 형식
		Desc.AlignedByteOffset = 16; // 데이터의 바이트 오프셋
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA; // 입력 슬롯 클래스
		Desc.SemanticIndex = 0; // 의미론 인덱스
		Desc.InstanceDataStepRate = 0; // 인스턴스 데이터 단계 속도
		InputLayoutData.push_back(Desc); // 입력 레이아웃 데이터에 추가
	}

	// 입력 레이아웃 생성
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
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

void ULineComponent::InitVertexShader()
{
	// 현재 디렉토리 헬퍼 객체 생성
	FDirectoryHelper CurDir;
	// 엔진 쉐이더 디렉토리로 이동
	CurDir.MoveSelectShaderDirectory("Doom");
	// 쉐이더 파일을 가져옴
	FFileHelper VSFile = CurDir.GetFile("DoomMapShader.fx");
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
		"VS",       // 엔트리 포인트 함수 이름
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
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateVertexShader(
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

void ULineComponent::UpdateVertexShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void ULineComponent::InitIndexBuffer()
{
	// 인덱스 데이터를 저장할 벡터 생성
	std::vector<unsigned int> Indexes;

	// 인덱스 데이터 추가
	Indexes.push_back(0);
	Indexes.push_back(1);

	// 인덱스 버퍼 설명 구조체 초기화
	D3D11_BUFFER_DESC Desc = { 0 };

	Desc.ByteWidth = static_cast <UINT>(sizeof(unsigned int) * static_cast<int>(Indexes.size())); // 버퍼의 크기를 설정
	Desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 버퍼의 바인딩 플래그를 설정 (인덱스 버퍼로 사용)
	Desc.CPUAccessFlags = 0; // CPU 접근 플래그를 설정 (기본값)
	Desc.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 사용 방식을 설정

	// 버퍼에 초기 데이터 설정
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Indexes[0]; // 초기 데이터 설정

	// 디바이스를 사용하여 인덱스 버퍼를 생성
	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, &Data, &IndexBuffer))
	{
		MSGASSERT("인덱스 버퍼 생성 실패");
		return;
	}

}

void ULineComponent::UpdateIndexBuffer()
{
	int Offset = 0;

	UEngineCore::GetDevice().GetDeviceContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, Offset);

	UEngineCore::GetDevice().GetDeviceContext()->IASetPrimitiveTopology(Topology);
}

void ULineComponent::InitRasterizer()
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
	UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&Desc, RasterizerState.GetAddressOf());

	// 뷰포트 정보 설정
	// 뷰포트는 렌더링된 이미지가 화면에 그려질 영역을 정의합니다.
	ViewPortInfo.Width = 1280.0f; // 뷰포트의 너비를 1280으로 설정
	ViewPortInfo.Height = 720.0f; // 뷰포트의 높이를 720으로 설정
	ViewPortInfo.TopLeftX = 0.0f; // 뷰포트의 시작 X 좌표를 0으로 설정 (화면의 왼쪽 가장자리)
	ViewPortInfo.TopLeftY = 0.0f; // 뷰포트의 시작 Y 좌표를 0으로 설정 (화면의 상단 가장자리)
	ViewPortInfo.MinDepth = 0.0f; // 뷰포트의 최소 깊이를 0.0으로 설정 (가장 가까운 깊이)
	ViewPortInfo.MaxDepth = 1.0f; // 뷰포트의 최대 깊이를 1.0으로 설정 (가장 먼 깊이)


}

void ULineComponent::UpdateRasterizer()
{
	UEngineCore::GetDevice().GetDeviceContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::GetDevice().GetDeviceContext()->RSSetState(RasterizerState.Get());
}

void ULineComponent::InitPixelShader()
{
	// 현재 디렉토리 헬퍼 객체 생성
	FDirectoryHelper CurDir;
	// 엔진 쉐이더 디렉토리로 이동
	CurDir.MoveSelectShaderDirectory("Doom");
	// 쉐이더 파일을 가져옴
	FFileHelper VSFile = CurDir.GetFile("DoomMapShader.fx");
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
		"PS",        // 엔트리 포인트 함수 이름
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
	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreatePixelShader(
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

void ULineComponent::UpdatePixelShader()
{
	UEngineCore::GetDevice().GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);

}

// 그리기 위한 백지 준비 과정
void ULineComponent::UpdateRenderTargetView()
{
	// 렌더 타겟 뷰 포인터를 가져옴
	ID3D11RenderTargetView* RenderTargetView = UEngineCore::GetDevice().GetRenderTargetView();

	// 렌더 타겟 뷰 포인터 배열을 초기화
	ID3D11RenderTargetView* ArrRtv[16] = { 0 }; // 최대 16개의 렌더 타겟 뷰를 가질 수 있음
	ArrRtv[0] = RenderTargetView; // 첫 번째 렌더 타겟 뷰를 설정 (여기서는 SV_Target0)

	// 출력 머지(OM) 스테이지에서 렌더 타겟을 설정
	UEngineCore::GetDevice().GetDeviceContext()->OMSetRenderTargets(
		1,            // 렌더 타겟 뷰의 수 (여기서는 1개)
		&ArrRtv[0],   // 렌더 타겟 뷰 배열의 포인터
		nullptr       // 깊이-스텐실 뷰 (여기서는 사용 안 함)
	);

}

void ULineComponent::InitShaderResourceView()
{
	D3D11_BUFFER_DESC BufferInfo = { 0 };
	BufferInfo.ByteWidth = sizeof(FTransform);
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, ConstantBuffer.GetAddressOf()))
	{
		MSGASSERT("상수버퍼 생성에 실패했습니다..");
		return;
	}
}

void ULineComponent::UpdateShaderResourceView()
{
	{
		FTransform& RendererTransform = GetComponentTransformRef();

		D3D11_MAPPED_SUBRESOURCE SubResourceData = {};

		//렌더링 정지 후 상수 버퍼 수정
		UEngineCore::GetDevice().GetDeviceContext()->Map(ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResourceData);

		if (nullptr == SubResourceData.pData)
		{
			MSGASSERT("그래픽 카드 수정 거부");
		}

		memcpy_s(SubResourceData.pData, sizeof(FTransform), &RendererTransform, sizeof(FTransform));
		UEngineCore::GetDevice().GetDeviceContext()->Unmap(ConstantBuffer.Get(), 0);

		ID3D11Buffer* ArrPtr[16] = { ConstantBuffer.Get() };
		UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(0, 1, ArrPtr);
	}
}