#pragma once
#include <GameFramework/Actor.h>

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

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:

};

