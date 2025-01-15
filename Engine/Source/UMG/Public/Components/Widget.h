#pragma once
#include "Engine/Classes/Engine/TransformObject.h"
#include "Engine/Classes/Engine/RenderAsset.h"

class AHUD;
class UCameraComponent;
class ULevel;

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

	ENGINE_API void Render(UCameraComponent* CameraComponent, float DeltaTime);

	ULevel* GetWorld();

	/** 겟, 셋 메소드 */
	URenderAsset& GetRenderUnit()
	{
		return RenderAsset;
	}

protected:

private:
	AHUD* HUD = nullptr;

	UWidget* ParentWidget = nullptr;
	std::list<std::shared_ptr<UWidget>> ChildWidgetList;

	URenderAsset RenderAsset;

	FWidgetColor WidgetColor;
	FWidgetUV WidgetUV;
	FWidgetData WidgetData;
};

