#pragma once
#include <GameFramework/Actor.h>

class UDoomMapComponent;
class ULineComponent;
class UWad;
/**
 *	설명 
 */
class ADrawSquareActor : public AActor
{
public:
	/** 생성자, 소멸자 */
	ADrawSquareActor();
	~ADrawSquareActor();

	/** 객체 값 복사 방지 */
	ADrawSquareActor(const ADrawSquareActor& Other) = delete;
	ADrawSquareActor(ADrawSquareActor&& Other) noexcept = delete;
	ADrawSquareActor& operator=(const ADrawSquareActor& Other) = delete;
	ADrawSquareActor& operator=(ADrawSquareActor&& Other) noexcept = delete;

protected:
	/** 액터 상속 메소드 */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::vector<std::shared_ptr<UDoomMapComponent>> SquareComponentVector;
	std::vector<std::shared_ptr<ULineComponent>> LineComponentVector;
	UWad* Wad = nullptr;

};

