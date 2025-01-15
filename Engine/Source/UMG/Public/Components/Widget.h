#pragma once
#include "Engine/Classes/Engine/TransformObject.h"
#include "Engine/Classes/Engine/RenderAsset.h"

class AHUD;
class UCameraComponent;
class ULevel;
class UPaperSprite;

struct FWidgetUV
{
public:
	float4 PlusUVValue;
};

struct FWidgetColor
{
public:
	float4 PlusColor;
	float4 MulColor;
};

struct FWidgetData
{
	float4 CuttingLocation = { 0.0f, 0.0f };
	float4 CuttingSize = { 1.0f, 1.0f };
	float4 Pivot = { 0.5f, 0.5f };
};

/**
 *	설명
 */
class UWidget : public UObject, public UTransformObject
{
	friend AHUD;
public:
	/** 생성자, 소멸자 */
	ENGINE_API UWidget();
	ENGINE_API ~UWidget();

	/** 객체 값 복사 방지 */
	UWidget(const UWidget& Other) = delete;
	UWidget(UWidget&& Other) noexcept = delete;
	UWidget& operator=(const UWidget& Other) = delete;
	UWidget& operator=(UWidget&& Other) noexcept = delete;

	void Tick(float DeltaTime);
	void Render(UCameraComponent* CameraComponent, float DeltaTime);

	ENGINE_API void SetSprite(std::string_view SpriteName, UINT Index = 0);
	ENGINE_API void SetTexture(std::string_view TextureName, bool bAutoScaleValue = false, float bRatioValue = 1.0f);

	ULevel* GetWorld();

	/** 겟, 셋 메소드 */
	URenderAsset& GetRenderUnit()
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
	void SetHoverEventFunction(std::function<void()> Function)
	{
		HoverEventFunction = Function;
	}
	void SetMouseClickEventFunction(std::function<void()> Function)
	{
		MouseClickEventFunction = Function;
	}
	void SetMouseUpEventFunction(std::function<void()> Function)
	{
		MouseUpEventFunction = Function;
	}
protected:
	FWidgetColor WidgetColor;
	FWidgetUV WidgetUV;
	FWidgetData WidgetData;

private:
	AHUD* HUD = nullptr;

	UWidget* ParentWidget = nullptr;
	std::list<std::shared_ptr<UWidget>> ChildWidgetList;

	URenderAsset RenderAsset;

	int CurIndex = 0;
	bool bAutoScale = false;
	float AutoScaleRatio = 1.0f;
	UPaperSprite* Sprite = nullptr;

	std::function<void()> HoverEventFunction;
	std::function<void()> MouseClickEventFunction;
	std::function<void()> MouseUpEventFunction;
};

