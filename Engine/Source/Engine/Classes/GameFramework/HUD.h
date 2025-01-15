#pragma once
#include "Engine/Classes/GameFramework/Actor.h"

class UWidget;
class UCameraComponent;

/**
 *	설명
 */
class AHUD : public AActor
{
	friend class ULevel;
public:
	/** 생성자, 소멸자 */
	ENGINE_API AHUD();
	ENGINE_API ~AHUD();

	/** 객체 값 복사 방지 */
	AHUD(const AHUD& Other) = delete;
	AHUD(AHUD&& Other) noexcept = delete;
	AHUD& operator=(const AHUD& Other) = delete;
	AHUD& operator=(AHUD&& Other) noexcept = delete;

	template<typename WidgetType>
	std::shared_ptr<WidgetType> CreateWidget(int ZOrder, std::string_view WidgetName = "NONE")
	{
		static_assert(std::is_base_of_v<UWidget, WidgetType>, "위젯을 상속받지 않은 클래스를 CreateWidget하려고 했습니다");

		if (false == std::is_base_of_v<UWidget, WidgetType>)
		{
			MSGASSERT("위젯을 상속받지 않은 클래스를 CreateWidget하려고 했습니다");
			return nullptr;
		}

		char* Memory = new char[sizeof(WidgetType)];

		UWidget* WidgetPtr = reinterpret_cast<WidgetType*>(Memory);

		WidgetPtr->HUD = this;

		WidgetType* NewPtr = reinterpret_cast<WidgetType*>(Memory);

		std::shared_ptr<WidgetType> NewWidgetPtr(NewPtr = new(Memory) WidgetType());

		NewWidgetPtr->SetName(WidgetName);
		NewWidgetPtr->SetOrder(ZOrder);

		WidgetMap[NewWidgetPtr->GetOrder()].push_back(NewWidgetPtr);

		return NewWidgetPtr;
	}

protected:

private:
	std::map<int, std::list<std::shared_ptr<UWidget>>> WidgetMap;

	void RenderUI(UCameraComponent* CameraComponent, float DeltaTime);
};

