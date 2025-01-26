#pragma once
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/Engine/PaperSprite.h"
#include "Engine/Public/Global/EngineStruct.h"

#include "Core/Public/Delegates/Delegate.h"

class UCameraComponent;
class URenderAsset;

/**
 *	설명
 */
class UPaperSpriteComponent : public UPrimitiveComponent
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

public:
	/** 생성자, 소멸자 */
	ENGINE_API UPaperSpriteComponent();
	ENGINE_API ~UPaperSpriteComponent();

	/** 객체 값 복사 방지 */
	UPaperSpriteComponent(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent(UPaperSpriteComponent&& Other) noexcept = delete;
	UPaperSpriteComponent& operator=(const UPaperSpriteComponent& Other) = delete;
	UPaperSpriteComponent& operator=(UPaperSpriteComponent&& Other) noexcept = delete;

	/** 빌보드 메소드 */
	void OnBillboard() { bBillboard = true; }
	void OffBillboard() {bBillboard = false; }

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


	/** 겟, 셋 메소드 */
	//ENGINE_API void SetSprite(std::string_view SpriteName);
	ENGINE_API void SetSprite(std::string_view SpriteName, size_t Index);
	ENGINE_API void SetSprite(UPaperSprite* PaperSprite);
	ENGINE_API void SetSpriteData(UPaperSprite* PaperSprite, size_t Index);
	ENGINE_API void SetTexture(UTexture* NewTexture);
	ENGINE_API void SetTexture(std::string_view TextureName, bool bAutoScale = false, float Ratio = 1.0f);

	ENGINE_API std::string GetCurSpriteName()
	{
		return Sprite->GetName();
	}
	int GetCurIndex()
	{
		return CurIndex;
	}
	int GetFrameIndex()
	{
		return CurAnimation->CurIndex;
	}
	bool IsCurAnimationEnd()
	{
		return CurAnimation->IsEnd;
	}
	void SetAnimationSpeed(float NewSpeed)
	{
		CurAnimationSpeed = NewSpeed;
	}
	void ResetAnimationSpeed()
	{
		CurAnimationSpeed = 1.0f;
	}
	void SetAutoScale(bool NewAutoScale)
	{
		IsAutoScale = NewAutoScale;
	}
	void SetAutoScaleRatio(float NewScaleRatio)
	{
		AutoScaleRatio = NewScaleRatio;
	}

	FResultColor ColorData;
	FUVValue UVValue;
	FPaperSpriteData SpriteData;

protected:
	/** USceneComponent method */
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime) override;
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;
	ENGINE_API virtual void UpdateCameraTransform(UCameraComponent* CameraComponent) override;

private:
	URenderAsset* MainUnit;

	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;
	bool bBillboard = false;

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr;
	UPaperSprite* Sprite = nullptr;
	UTexture* CurTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> SpriteConstantBuffer = nullptr; // 스프라이트용 상수버퍼
};

