#include "pch.h"
#include "PaperSpriteComponent.h"

#include "Classes/Engine/PaperSprite.h"
#include "Classes/Camera/CameraComponent.h"

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
	}
}


void UPaperSpriteComponent::Render(UCameraComponent* CameraComponent, float DeltaTime)
{
    if (nullptr != CurAnimation)
    {
        UPrimitiveComponent::SetSprite(CurAnimation->Sprite);
        UPrimitiveComponent::SetSpriteData(CurAnimation->CurIndex);
    }

    UPrimitiveComponent::Render(CameraComponent, DeltaTime);
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
	Sprite = CurAnimation->Sprite;
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

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName)
{
    //SetTexture(SpriteName);
}

void UPaperSpriteComponent::SetSprite(std::string_view SpriteName, size_t Index)
{
    UPrimitiveComponent::SetSprite(SpriteName);
    SetSpriteData(Index);
}

FVector UPaperSpriteComponent::SetSpriteScale(float Ratio, int CurIndex)
{
	if (nullptr == Sprite)
	{
		MSGASSERT("스프라이트를 세팅하지 않고 스프라이트 크기로 랜더러 크기를 조정할수 없습니다.");
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	return { 0.0f, 0.0f, 0.0f };
}
