#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Core/Delegates/Delegate.h"
#include "Classes/Engine/PaperSprite.h"

class UCameraComponent;

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
		float AutoScaleRatio = 1.0f;

		bool Loop = true;
		bool IsEnd = false;
		bool IsAutoScale = true;

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

	virtual void InitShaderResourceView() override;
	virtual void UpdateShaderResourceView() override;

	virtual void InitVertexShader() override;
	virtual void InitPixelShader() override;

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
	ENGINE_API void SetSprite(std::string_view SpriteName);
	ENGINE_API void SetSprite(std::string_view SpriteName, size_t Index);
	ENGINE_API void SetSprite(UPaperSprite* PaperSprite);
	ENGINE_API void SetSpriteData(UPaperSprite* PaperSprite, size_t Index);
	ENGINE_API void SetTexture(UTexture* NewTexture);

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
	

protected:
	/** USceneComponent method */
	void BeginPlay() override;
	virtual void ComponentTick(float DeltaTime) override;
	ENGINE_API void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

private:
	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr;
	UPaperSprite* Sprite = nullptr;
	UTexture* CurTexture = nullptr;
	FPaperSpriteData SpriteData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> SpriteConstantBuffer = nullptr; // 스프라이트용 상수버퍼
};

