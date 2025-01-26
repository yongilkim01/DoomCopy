#pragma once
#include "UI/Public/Widget/Widget.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Core/Public/Delegates/Delegate.h"
#include "EngineStruct.h"

/**
 *	설명
 */
class UImage : public UWidget
{
public:
	class FrameAnimation
	{
	public:
		UPaperSprite* Sprite = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, UDelegate> Events;
		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;
		bool IsEnd = false;
		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
			IsEnd = false;
		}
	};
	
	/** Animation 메소드 */
	ENGINE_API void CreateAnimation(std::string_view AnimationName, std::string_view SpriteName,
		int Start, int End, float Time = 0.1f, bool Loop = true);
	ENGINE_API void CreateAnimation(std::string_view AnimationName, std::string_view SpriteName,
		std::vector<int> Indexes, std::vector<float> Frames, bool Loop = true);
	ENGINE_API void CreateAnimation(std::string_view AnimationName, std::string_view SpriteName,
		std::vector<int> Indexes, float Frame, bool Loop = true);

	ENGINE_API void ChangeAnimation(std::string_view AnimationName, bool Force = false);
	ENGINE_API void SetAnimationEvent(std::string_view AnimationName, int Frame,
		std::function<void()> EventFunction);

	ENGINE_API FrameAnimation* FindAnimation(std::string_view AnimationName);

private:
	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;
	FrameAnimation* CurAnimation = nullptr;
	std::map<std::string, FrameAnimation> FrameAnimations;

public:
	/** 생성자, 소멸자 */
	ENGINE_API UImage();
	ENGINE_API ~UImage();

	/** 객체 값 복사 방지 */
	UImage(const UImage& Other) = delete;
	UImage(UImage&& Other) noexcept = delete;
	UImage& operator=(const UImage& Other) = delete;
	UImage& operator=(UImage&& Other) noexcept = delete;

	virtual void Tick(float DeltaTime) override;
	virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

	ENGINE_API void SetSprite(std::string_view SpriteName, UINT Index = 0);
	ENGINE_API void SetTexture(std::string_view TextureName, bool bAutoScaleValue = false, float bRatioValue = 1.0f);

	/** 겟, 셋 메소드 */
	URenderAsset& GetRenderAsset()
	{
		return RenderAsset;
	}
	bool IsAutoScale()
	{
		return bAutoScale;
	}
	void SetAutoScale(bool Value)
	{
		bAutoScale = Value;
	}
	float GetAutoScaleRatio()
	{
		return AutoScaleRatio;
	}
	void SetAutoScaleRatio(float Value)
	{
		AutoScaleRatio = Value;
	}

protected:

private:
	URenderAsset RenderAsset;

	bool bAutoScale = false;
	float AutoScaleRatio = 1.0f;
	UPaperSprite* Sprite = nullptr;

	FResultColor WidgetColor;
	FUVValue WidgetUV;
	FPaperSpriteData WidgetData;
};

