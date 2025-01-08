#pragma once
#include <GameFramework/Actor.h>

class UTestComponent;
class UPaperSpriteComponent;

/**
 *	설명
 */
class ATestMap : public AActor
{
public:
	/** 생성자, 소멸자 */
	ATestMap();
	~ATestMap();

	/** 객체 값 복사 방지 */
	ATestMap(const ATestMap& Other) = delete;
	ATestMap(ATestMap&& Other) noexcept = delete;
	ATestMap& operator=(const ATestMap& Other) = delete;
	ATestMap& operator=(ATestMap&& Other) noexcept = delete;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UPaperSpriteComponent> SpriteComponent = nullptr;
};

