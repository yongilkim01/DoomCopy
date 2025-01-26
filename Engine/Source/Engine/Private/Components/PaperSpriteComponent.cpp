#include "Engine.h"
#include "Engine/Classes/Components/PaperSpriteComponent.h"

#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Classes/Engine/PaperSprite.h"

#include "Engine/Classes/Camera/CameraComponent.h"

#include "Engine/Classes/Engine/GameEngine.h"
#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Engine/Public/Materials/Material.h"

UPaperSpriteComponent::UPaperSpriteComponent()
{
	CreateRenderUnit();
	SetMesh("Rect");
	SetMaterial("SpriteMaterial");
	
	GetRenderUnit().ConstantBufferLinkData("ResultColor", ColorData);
	GetRenderUnit().ConstantBufferLinkData("FSpriteData", SpriteData);
	GetRenderUnit().ConstantBufferLinkData("FUVValue", UVValue);

	UVValue.PlusUVValue = { 0.0f, 0.0f, 0.0f, 0.0f };
	SpriteData.CuttingLocation = { 0.0f, 0.0f };
	SpriteData.CuttingSize = { 1.0f, 1.0f };
	SpriteData.Pivot = { 0.5f, 0.5f };

	ColorData.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

UPaperSpriteComponent::~UPaperSpriteComponent()
{
}

void UPaperSpriteComponent::BeginPlay()
{
    UPrimitiveComponent::BeginPlay();
}

void UPaperSpriteComponent::TickComponent(float DeltaTime)
{
    UPrimitiveComponent::TickComponent(DeltaTime);

    if (nullptr != CurAnimation)
    {
		FrameAnimation* EventAnimation = nullptr;
		int EventFrame = -1;

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
				EventAnimation = CurAnimation;
				EventFrame = CurIndex;
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
						EventAnimation = CurAnimation;
						EventFrame = CurIndex;
						//CurAnimation->Events[CurIndex]();
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

		if (nullptr != EventAnimation)
		{
			if (EventAnimation->Events.contains(CurIndex))
			{
				EventAnimation->Events[CurIndex]();
			}
		}

	}
}


void UPaperSpriteComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	if (nullptr != CurAnimation)
	{
		Sprite = CurAnimation->Sprite;

		GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(CurIndex)->GetName());
		SpriteData = Sprite->GetSpriteData(CurIndex);
	}

	if (true == IsAutoScale && nullptr != Sprite)
	{
		FVector Scale = Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}

	UPrimitiveComponent::Render(CameraComponent, DeltaTime);

	if (true == bBillboard)
	{
		Transform.WVP;
	}

}

void UPaperSpriteComponent::UpdateCameraTransform(UCameraComponent* CameraComponent)
{
	FTransform& CameraTransform = CameraComponent->GetComponentTransformRef();
	FTransform& ComponentTransform = GetComponentTransformRef();
	//	// �������� ���� �� ������Ʈ�� �� ���ù޾Ұ�
	// RendererTrans.View = CameraTrans.View;
	ComponentTransform.View = CameraTransform.View;
	FMatrix CurWorld = ComponentTransform.World;

	if (true == bBillboard)
	{
		FMatrix Bill = CameraTransform.View;
		// ������� �̵��κ��� �ʱ�ȭ
		Bill.ArrVector[3] = FVector(0.0f, 0.0f, 0.0f, 1.0f);
		// ����ķ� ���ص� ���������� �ִ� ȸ���Ӽ��� ������� ��ġ����̱� ������ �̷��� ó�����ָ� �ȴ�.
		Bill.Transpose();
		// ������ ����� Ʈ�������� �� ���� ���� ������ ����
		CurWorld = ComponentTransform.ScaleMat * Bill * ComponentTransform.LocationMat * ComponentTransform.RevolveMat * ComponentTransform.ParentMat;
	}

	ComponentTransform.Projection = CameraTransform.Projection;
	ComponentTransform.WVP = CurWorld * ComponentTransform.View * ComponentTransform.Projection;
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

	if (true == IsAutoScale)
	{
		FVector Scale = CurAnimation->Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}
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

//void UPaperSpriteComponent::SetSprite(std::string_view SpriteName)
//{
//	std::string UpperSpriteName = UEngineString::ToUpper(SpriteName);
//
//	Sprite = UPaperSprite::Find<UPaperSprite>(UpperSpriteName).get();
//
//	if (nullptr == Sprite)
//	{
//		MSGASSERT("��������Ʈ �ε� ����");
//	}
//
//	//CurTexture = Sprite->GetTexture(0);
//}

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName, size_t Index)
{
	Sprite = UPaperSprite::Find<UPaperSprite>(SpriteName).get();

	GetRenderUnit().SetTexture("ImageTexture", Sprite->GetTexture(Index)->GetName());
	SpriteData = Sprite->GetSpriteData(Index);

	CurIndex = Index;
}

void UPaperSpriteComponent::SetSprite(UPaperSprite* PaperSprite)
{
	Sprite = PaperSprite;

	if (nullptr == Sprite)
	{
		MSGASSERT("��������Ʈ�� �������� �ʽ��ϴ�.");
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

void UPaperSpriteComponent::SetTexture(std::string_view TextureName, bool bAutoScale, float Ratio)
{
	std::shared_ptr<UTexture> Texture = UTexture::Find<UTexture>(TextureName);
	if (nullptr == Texture)
	{
		MSGASSERT("�ε����� ���� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
	}

	GetRenderUnit().SetTexture("ImageTexture", TextureName);

	if (true == bAutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * Ratio);
	}
}
