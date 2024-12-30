#include "pch.h"
#include "PaperSpriteComponent.h"

#include "Classes/Engine/PaperSprite.h"
#include "Classes/Camera/CameraComponent.h"
#include "Core/EngineCore.h"

UPaperSpriteComponent::UPaperSpriteComponent()
{
}

UPaperSpriteComponent::~UPaperSpriteComponent()
{
}

void UPaperSpriteComponent::BeginPlay()
{
    UPrimitiveComponent::BeginPlay();
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
			// �ִϸ��̼� �ص�
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
    if (nullptr != CurAnimation)
    {
        SetSprite(CurAnimation->Sprite);
        SetSpriteData(CurIndex);
    }

    UPrimitiveComponent::Render(CameraComponent, DeltaTime);
}

void UPaperSpriteComponent::InitShaderResourceView()
{
	UPrimitiveComponent::InitShaderResourceView();

	// ��������Ʈ�� ��� ���� ����
	{
		// ��� ���� ���� ����ü �ʱ�ȭ
		D3D11_BUFFER_DESC Desc = { 0 };
		Desc.ByteWidth = sizeof(FPaperSpriteData); // ������ ũ�⸦ FTransform ����ü ũ��� ����
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // ������ ���ε� �÷��׸� ��� ���۷� ����
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE; // CPU�� ���ۿ� ���� ���� �����ϵ��� ����
		Desc.Usage = D3D11_USAGE_DYNAMIC; // ������ ��� ����� �������� ����

		// ����̽��� ����Ͽ� ��� ���� ����
		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&Desc, nullptr, &SpriteConstantBuffer))
		{
			MSGASSERT("��������Ʈ�� ��� ���ۿ� ���� ����");
			return;
		}
	}

	// ���÷� ���� ���� ����ü �ʱ�ȭ �� ����
	D3D11_SAMPLER_DESC SamplerDesc = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT };
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // U ��ǥ �ؽ�ó ���� ��� ����
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER; // V ��ǥ �ؽ�ó ���� ��� ����
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP; // W ��ǥ �ؽ�ó ���� ��� ����

	SamplerDesc.BorderColor[0] = 0.0f;
	SamplerDesc.BorderColor[1] = 0.0f;
	SamplerDesc.BorderColor[2] = 0.0f;
	SamplerDesc.BorderColor[3] = 0.0f;

	//SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	//SamplerDesc.MaxLOD = 0.0f;
	//SamplerDesc.MinLOD = 0.0f;


	// ����̽��� ����Ͽ� ���÷� ���� ����
	UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);
}

void UPaperSpriteComponent::UpdateShaderResourceView()
{
	UPrimitiveComponent::UpdateShaderResourceView();


	FTransform& RendererTransform = GetComponentTransformRef();

	D3D11_MAPPED_SUBRESOURCE SubResourceData = {};

	//������ ���� �� ��� ���� ����
	UEngineCore::GetDevice().GetDeviceContext()->Map(SpriteConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResourceData);

	if (nullptr == SubResourceData.pData)
	{
		MSGASSERT("�׷��� ī�� ���� �ź�");
	}

	memcpy_s(SubResourceData.pData, sizeof(FPaperSpriteData), &SpriteData, sizeof(FPaperSpriteData));
	UEngineCore::GetDevice().GetDeviceContext()->Unmap(SpriteConstantBuffer.Get(), 0);

	ID3D11Buffer* ArrPtr[16] = { SpriteConstantBuffer.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->VSSetConstantBuffers(1, 1, ArrPtr);

	ID3D11ShaderResourceView* ArrSRV[16] = { Sprite->GetShaderResourceView() };
	UEngineCore::GetDevice().GetDeviceContext()->PSSetShaderResources(0, 1, ArrSRV);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::GetDevice().GetDeviceContext()->PSSetSamplers(0, 1, ArrSMP);
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
		MSGASSERT(UpperName + "�� ����� ������ �����ϴ� �����Ӱ� Ÿ���� ī��Ʈ�� ���� �ٸ��ϴ�");
		return;
	}
	if (FrameAnimations.contains(UpperName))
	{
		return;
	}
	std::shared_ptr<UPaperSprite> FindSprite = UPaperSprite::Find<UPaperSprite>(SpriteName);
	if (nullptr == FindSprite)
	{
		MSGASSERT("�ε����� ���� ��������Ʈ�� �ִϸ��̼� ������ ����Ϸ��� �߽��ϴ�" + std::string(UpperName));
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
		MSGASSERT("�������� ���� �ִϸ��̼����� �����Ϸ��� �߽��ϴ�. = " + UpperName);
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
	Sprite = CurAnimation->Sprite;
}
void UPaperSpriteComponent::SetAnimationEvent(std::string_view AnimationName, 
											  int Frame, std::function<void()> EventFunction)
{
	std::string UpperName = UEngineString::ToUpper(AnimationName);
	if (false == FrameAnimations.contains(UpperName))
	{
		MSGASSERT("�������� ���� �ִϸ��̼����� �����Ϸ��� �߽��ϴ�. = " + UpperName);
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
		MSGASSERT("�������� �ʴ� �����ӿ� �̺�Ʈ�� �����Ϸ��� �߽��ϴ�" + std::string(AnimationName));
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

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName)
{
	std::string UpperSpriteName = UEngineString::ToUpper(SpriteName);

	Sprite = UPaperSprite::Find<UPaperSprite>(UpperSpriteName).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("��������Ʈ �ε� ����");
	}
}

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName, size_t Index)
{
    SetSprite(SpriteName);
    SetSpriteData(Index);
}

void UPaperSpriteComponent::SetSprite(UPaperSprite* PaperSprite)
{
	Sprite = PaperSprite;

	if (nullptr == Sprite)
	{
		MSGASSERT("��������Ʈ�� �������� �ʽ��ϴ�.");
	}
}

void UPaperSpriteComponent::SetSpriteData(size_t Index)
{
	SpriteData = Sprite->GetSpriteData(Index);
}
