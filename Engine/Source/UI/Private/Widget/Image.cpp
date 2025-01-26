#include "Engine.h"
#include "UI/Public/Widget/Image.h"


void UImage::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName, int Start, int End, float Time, bool bLoop)
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

void UImage::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName, std::vector<int> Indexes, std::vector<float> FrameVector, bool bLoop)
{
	std::string UpperName = UEngineString::ToUpper(AnimationName);
	if (FrameVector.size() != Indexes.size())
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
	NewAnimation.FrameTime = FrameVector;
	NewAnimation.Loop = bLoop;
	NewAnimation.Reset();

	FrameAnimations.insert({ UpperName ,NewAnimation });
}

void UImage::CreateAnimation(std::string_view AnimationName, std::string_view SpriteName, std::vector<int> Indexes, float Frame, bool bLoop)
{
	std::vector<float> Times;

	for (size_t i = 0; i < Indexes.size(); i++)
	{
		Times.push_back(Frame);
	}

	CreateAnimation(AnimationName, SpriteName, Indexes, Times, bLoop);
}

void UImage::ChangeAnimation(std::string_view AnimationName, bool bForce)
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
	if (true == IsAutoScale())
	{
		FVector Scale = CurAnimation->Sprite->GetSpriteScaleToReal(CurIndex);
		Scale.Z = 1.0f;
		SetRelativeScale3D(Scale * AutoScaleRatio);
	}
}

void UImage::SetAnimationEvent(std::string_view AnimationName, int Frame, std::function<void()> EventFunction)
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

UImage::FrameAnimation* UImage::FindAnimation(std::string_view AnimationName)
{
	std::string UpperString = UEngineString::ToUpper(AnimationName);
	if (false == FrameAnimations.contains(UpperString))
	{
		return nullptr;
	}
	return &FrameAnimations[UpperString];
}
 

UImage::UImage()
{
	RenderAsset.SetTransformObject(this);
	RenderAsset.SetMesh("Rect");
	RenderAsset.SetMaterial("WidgetMaterial");

	RenderAsset.ConstantBufferLinkData("ResultColor", WidgetColor);
	RenderAsset.ConstantBufferLinkData("FSpriteData", WidgetData);
	RenderAsset.ConstantBufferLinkData("FUVValue", WidgetUV);

	WidgetUV.PlusUVValue = { 0.0f, 0.0f, 0.0f, 0.0f };

	WidgetData.CuttingLocation = { 0.0f, 0.0f };
	WidgetData.CuttingSize = { 1.0f, 1.0f };
	WidgetData.Pivot = { 0.5f, 0.5f };

	WidgetColor.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	WidgetColor.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

UImage::~UImage()
{
}

void UImage::Tick(float DeltaTime)
{
	// �ִϸ��̼� �����Ű�� �ڵ带 ComponentTick���� �Ű��. 
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
				// CurAnimation->Events[CurIndex]();
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
						// CurAnimation->Events[CurIndex]();
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

void UImage::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
	UWidget::Render(CameraComponent, DeltaTime);

	if (nullptr != CurAnimation)
	{
		Sprite = CurAnimation->Sprite;

		RenderAsset.SetTexture("ImageTexture", Sprite->GetTexture(CurIndex)->GetName());
		WidgetData = Sprite->GetSpriteData(CurIndex);
	}

	if (true == bAutoScale && nullptr != Sprite)
	{
		FVector SpriteScale = Sprite->GetSpriteScaleToReal(CurIndex);
		SpriteScale.Z = 1.0f;
		SetRelativeScale3D(SpriteScale * AutoScaleRatio);
	}

	UpdateCameraTransform(CameraComponent);

	RenderAsset.Render(CameraComponent, DeltaTime);
}

void UImage::SetSprite(std::string_view SpriteName, UINT Index)
{
	Sprite = UPaperSprite::Find<UPaperSprite>(SpriteName).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("�������� �ʴ� �ؽ�ó�� �����Ϸ��� �߽��ϴ�.");
		return;
	}

	GetRenderAsset().SetTexture("ImageTexture", Sprite->GetTexture(Index)->GetName());

	WidgetData.CuttingLocation = Sprite->GetSpriteData(Index).CuttingLocation;
	WidgetData.CuttingSize = Sprite->GetSpriteData(Index).CuttingSize;
	WidgetData.Pivot = Sprite->GetSpriteData(Index).Pivot;
}

void UImage::SetTexture(std::string_view TextureName, bool bAutoScaleValue, float bRatioValue)
{
	std::shared_ptr<UTexture> Texture = UTexture::Find<UTexture>(TextureName);

	if (nullptr == Texture)
	{
		MSGASSERT("�ε����� ���� �ؽ�ó�� ����Ϸ��� �߽��ϴ�.");
	}

	GetRenderAsset().SetTexture("ImageTexture", TextureName);

	if (true == bAutoScale)
	{
		SetRelativeScale3D(Texture->GetTextureSize() * bRatioValue);
	}
}
