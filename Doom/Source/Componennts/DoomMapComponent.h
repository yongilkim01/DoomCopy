#pragma once
#include <Classes/Components/PrimitiveComponent.h>

/**
 *	설명
 */
class UDoomMapComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	UDoomMapComponent();
	~UDoomMapComponent();

	/** 객체 값 복사 방지 */
	UDoomMapComponent(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent(UDoomMapComponent&& Other) noexcept = delete;
	UDoomMapComponent& operator=(const UDoomMapComponent& Other) = delete;
	UDoomMapComponent& operator=(UDoomMapComponent&& Other) noexcept = delete;

protected:


private:

};

