#include "pch.h"
#include "PaperSpriteComponent.h"

#include "Classes/Engine/PaperSprite.h"
#include "Classes/Camera/CameraComponent.h"
#include "Core/EngineCore.h"

#include "Core/Misc/DirectoryHelper.h"
#include "Classes/Engine/Texture.h"
#include "Core/Misc/FileHelper.h"

UPaperSpriteComponent::UPaperSpriteComponent()
{
}

UPaperSpriteComponent::~UPaperSpriteComponent()
{
}

void UPaperSpriteComponent::BeginPlay()
{
    UPrimitiveComponent::BeginPlay();

	SetMesh("Rect");
	SetMesh("AlphaBlend");
}

void UPaperSpriteComponent::ComponentTick(float DeltaTime)
{
    UPrimitiveComponent::ComponentTick(DeltaTime);

    if (nullptr != CurAnimation)
    {
		CurAnimation->IsEnd = false;
		std::vector<int>& Indexs = CurAnimation->FrameIndex;
		std::vector<float>& Times = CurAnimation->FrameTime;
		Sprite = CurAnimation->Sprite;
		CurAnimation->CurTime += DeltaTime * CurAnimationSpeed;
		float CurFrameTime = Times[CurAnimation->CurIndex];
		//                           0.1 0.1 0.1
		if (CurAnimation->CurTime > CurFrameTime)
		{
			CurAnimation->CurTime -= CurFrameTime;
			++CurAnimation->CurIndex;
			if (CurAnimation->Events.contains(CurIndex))
			{
				CurAnimation->Events[CurIndex]();
			}
			// 애니메이션 앤드
			if (CurAnimation->CurIndex >= Indexs.size())
			{
				CurAnimation->IsEnd = true;
			}
			else {
				CurAnimation->IsEnd = false;
			}
			if (CurAnimation->CurIndex >= Indexs.size())
			{
				if (true == CurAnimation->Loop)
				{
					CurAnimation->CurIndex = 0;
					if (CurAnimation->Events.contains(CurIndex))
					{
						CurAnimation->Events[CurIndex]();
					}
				}
				else
				{
					CurAnimation->IsEnd = true;
					--CurAnimation->CurIndex;
				}
			}
		}

		CurIndex = Indexs[CurAnimation->CurIndex];

		if (true == CurAnimation->IsAutoScale)
		{
			FVector Scale = CurAnimation->Sprite->GetSpriteScaleToReal(CurIndex);
			Scale.Z = 1.0f;
			SetRelativeScale3D(Scale * CurAnimation->AutoScaleRatio);
		}
	}
}


void UPaperSpriteComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
    UPrimitiveComponent::Render(CameraComponent, DeltaTime);

	if (nullptr != CurAnimation)
	{
		UPaperSprite* Sprite = CurAnimation->Sprite;
		//size_t CurIndex = CurAnimation->CurIndex;

		SetTexture(Sprite->GetTexture(CurIndex));
		SetSpriteData(Sprite, CurIndex);
	}

	UPrimitiveComponent::Render(CameraComponent, DeltaTime);

}

void UPaperSpriteComponent::InitShaderResourceView()
{
	UPrimitiveComponent::InitShaderResourceView();

	// 스프라이트용 상수 버퍼 생성
	{
		// 상수 버퍼 설명 구조체 초기화
		D3D11_BUFFER_DESC Desc = { 0 };
		Desc.ByteWidth = sizeof(FPaperSpriteData); // 버퍼의 크기를 FTransform 구조체 크기로 설정
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 버퍼의 바인딩 플래그를 상수 버퍼로 설정
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; // CPU가 버퍼에 쓰기 접근 가능하도록 설정
		Desc.Usage = D3D11_USAGE_DYNAMIC; // 버퍼의 사용 방식을 동적으로 설정

		// 디바이스를 사용하여 상수 버퍼 생성
		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, nullptr, &SpriteConstantBuffer))
		{
			MSGASSERT("스프라이트용 상수 버퍼에 생성 실패");
			return;
		}
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

	//SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	//SamplerDesc.MaxLOD = 0.0f;
	//SamplerDesc.MinLOD = 0.0f;


	// 디바이스를 사용하여 샘플러 상태 생성
	UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);
}

void UPaperSpriteComponent::UpdateShaderResourceView()
{
	UPrimitiveComponent::UpdateShaderResourceView();


	FTransform& RendererTransform = GetComponentTransformRef();

	D3D11_MAPPED_SUBRESOURCE SubResourceData = {};

	//렌더링 정지 후 상수 버퍼 수정
	UEngineCore::GetDevice().GetDeviceContext()->Map(SpriteConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResourceData);

	if (nullptr == SubResourceData.pData)
	{
		MSGASSERT("그래픽 카드 수정 거부");
	}

	memcpy_s(SubResourceData.pData, sizeof(FPaperSpriteData), &SpriteData, sizeof(FPaperSpriteData));
	UEngineCore::GetDevice().GetDeviceContext()->Unmap(SpriteConstantBuffer.Get(), 0);

	ID3D11Buffer* ArrPtr[16] = { SpriteConstantBuffer.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(1, 1, ArrPtr);

	ID3D11ShaderResourceView* ArrSRV[16] = { CurTexture->GetShaderResourceView() };
	UEngineCore::GetDevice().GetDeviceContext()->PSSetShaderResources(0, 1, ArrSRV);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->PSSetSamplers(0, 1, ArrSMP);
}

void UPaperSpriteComponent::InitVertexShader()
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


void UPaperSpriteComponent::InitPixelShader()
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

void UPaperSpriteComponent::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName,
									  int Start, int End, float Time /*= 0.1f*/, bool bLoop /*= true*/)
{
	int Inter = 0;
	std::vector<int> Indexs;
	std::vector<float> Times;
	if (Start < End)
	{
		Inter = (End - Start) + 1;
		for (size_t i = 0; i < Inter; i++)
		{
			Indexs.push_back(Start);
			Times.push_back(Time);
			++Start;
		}
	}
	else
	{
		Inter = (Start - End) + 1;
		for (size_t i = 0; i < Inter; i++)
		{
			Indexs.push_back(End);
			Times.push_back(Time);
			++End;
		}
	}

	CreateAnimation(AnimationName, SpriteName, Indexs, Times, bLoop);
}

void UPaperSpriteComponent::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName, 
											std::vector<int> Indexes, float Frame, bool bLoop /*= true*/)
{
	std::vector<float> Times;
	for (size_t i = 0; i < Indexes.size(); i++)
	{
		Times.push_back(Frame);
	}
	CreateAnimation(AnimationName, SpriteName, Indexes, Times, bLoop);
}
void UPaperSpriteComponent::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName, 
											std::vector<int> Indexes, std::vector<float> Frame, bool bLoop /*= true*/)
{
	std::string UpperName = UEngineString::ToUpper(AnimationName);
	if (Frame.size() != Indexes.size())
	{
		MSGASSERT(UpperName + "을 만들다 에러가 났습니다 프레임과 타임의 카운트가 서로 다릅니다");
		return;
	}
	if (FrameAnimations.contains(UpperName))
	{
		return;
	}
	std::shared_ptr<UPaperSprite> FindSprite = UPaperSprite::Find<UPaperSprite>(SpriteName);
	if (nullptr == FindSprite)
	{
		MSGASSERT("로드하지 않은 스프라이트를 애니메이션 생서에 사용하려고 했습니다" + std::string(UpperName));
		return;
	}
	FrameAnimation NewAnimation;
	NewAnimation.Sprite = FindSprite.get();
	NewAnimation.FrameIndex = Indexes;
	NewAnimation.FrameTime = Frame;
	NewAnimation.Loop = bLoop;
	NewAnimation.Reset();
	FrameAnimations.insert({ UpperName ,NewAnimation });
}
void UPaperSpriteComponent::ChangeAnimation(std::string_view AnimationName, bool bForce /*= false*/)
{
	std::string UpperName = UEngineString::ToUpper(AnimationName);
	if (false == FrameAnimations.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 애니메이션으로 변경하려고 했습니다. = " + UpperName);
		return;
	}
	FrameAnimation* ChangeAnimation = &FrameAnimations[UpperName];
	if (CurAnimation == ChangeAnimation && false == bForce)
	{
		return;
	}

	CurAnimation = &FrameAnimations[UpperName];
	CurAnimation->Reset();
	CurIndex = CurAnimation->FrameIndex[CurAnimation->CurIndex];

	if (CurAnimation->Events.contains(CurAnimation->CurIndex))
	{
		CurAnimation->Events[CurAnimation->CurIndex]();
	}
	//Sprite = CurAnimation->Sprite;
}
void UPaperSpriteComponent::SetAnimationEvent(std::string_view AnimationName, 
											  int Frame, std::function<void()> EventFunction)
{
	std::string UpperName = UEngineString::ToUpper(AnimationName);
	if (false == FrameAnimations.contains(UpperName))
	{
		MSGASSERT("존재하지 않은 애니메이션으로 변경하려고 했습니다. = " + UpperName);
		return;
	}
	FrameAnimation* ChangeAnimation = &FrameAnimations[UpperName];
	bool Check = false;
	for (size_t i = 0; i < ChangeAnimation->FrameIndex.size(); i++)
	{
		if (Frame == ChangeAnimation->FrameIndex[i])
		{
			Check = true;
			break;
		}
	}
	if (false == Check)
	{
		MSGASSERT("존재하지 않는 프레임에 이벤트를 생성하려고 했습니다" + std::string(AnimationName));
		return;
	}
	ChangeAnimation->Events[Frame] += EventFunction;
}

UPaperSpriteComponent::FrameAnimation* UPaperSpriteComponent::FindAnimation(std::string_view AnimationName)
{
	std::string UpperString = UEngineString::ToUpper(AnimationName);
	if (false == FrameAnimations.contains(UpperString))
	{
		return nullptr;
	}
	return &FrameAnimations[UpperString];
}

//void UPaperSpriteComponent::SetSprite(std::string_view SpriteName)
//{
//	std::string UpperSpriteName = UEngineString::ToUpper(SpriteName);
//
//	Sprite = UPaperSprite::Find<UPaperSprite>(UpperSpriteName).get();
//
//	if (nullptr == Sprite)
//	{
//		MSGASSERT("스프라이트 로드 실패");
//	}
//
//	//CurTexture = Sprite->GetTexture(0);
//}
//
//void UPaperSpriteComponent::SetSprite(std::string_view SpriteName, size_t Index)
//{
//    SetSprite(SpriteName);
//    //SetSpriteData(Index);
//}

void UPaperSpriteComponent::SetSprite(UPaperSprite* PaperSprite)
{
	Sprite = PaperSprite;

	if (nullptr == Sprite)
	{
		MSGASSERT("스프라이트가 존재하지 않습니다.");
	}
}

void UPaperSpriteComponent::SetSpriteData(UPaperSprite* PaperSprite, size_t Index)
{
	SpriteData = PaperSprite->GetSpriteData(Index);
}

void UPaperSpriteComponent::SetTexture(UTexture* NewTexture)
{
	CurTexture = NewTexture;
}
