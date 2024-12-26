#pragma once
#include <GameFramework/Actor.h>

class UPaperSpriteComponent;

// Ό³Έν :
class ATitleLogo : public AActor
{
public:
	// constrcuter destructer
	ATitleLogo();
	~ATitleLogo();

	// delete Function
	ATitleLogo(const ATitleLogo& Other) = delete;
	ATitleLogo(ATitleLogo&& Other) noexcept = delete;
	ATitleLogo& operator=(const ATitleLogo& Other) = delete;
	ATitleLogo& operator=(ATitleLogo&& Other) noexcept = delete;

	std::shared_ptr<UPaperSpriteComponent> GetRenderer()
	{
		return LogoRenderer;
	}

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UPaperSpriteComponent> LogoRenderer;
	std::shared_ptr<UPaperSpriteComponent> Child;

};

