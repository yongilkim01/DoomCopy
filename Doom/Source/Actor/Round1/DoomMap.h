#pragma once
#include <GameFramework/Actor.h>

class UDoomMapComponent;
class ULineComponent;
class UWad;
// Ό³Έν :
class ADoomMap : public AActor
{
public:
	// constrcuter destructer
	ADoomMap();
	~ADoomMap();

	// delete Function
	ADoomMap(const ADoomMap& Other) = delete;
	ADoomMap(ADoomMap&& Other) noexcept = delete;
	ADoomMap& operator=(const ADoomMap& Other) = delete;
	ADoomMap& operator=(ADoomMap&& Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	std::shared_ptr<UDoomMapComponent> DoomMapComponent;
	std::shared_ptr<ULineComponent> LineComponent;

};

