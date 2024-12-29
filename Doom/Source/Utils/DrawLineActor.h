#pragma once
#include <GameFramework/Actor.h>

class ULineComponent;

/**
 *	설명
 */
class ADrawLineActor : public AActor
{
public:
	/** 생성자, 소멸자 */
	ADrawLineActor();
	~ADrawLineActor();

	/** 객체 값 복사 방지 */
	ADrawLineActor(const ADrawLineActor& Other) = delete;
	ADrawLineActor(ADrawLineActor&& Other) noexcept = delete;
	ADrawLineActor& operator=(const ADrawLineActor& Other) = delete;
	ADrawLineActor& operator=(ADrawLineActor&& Other) noexcept = delete;

protected:
	/** 액터 상속 메소드 */
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<ULineComponent> LineComponent;

};

