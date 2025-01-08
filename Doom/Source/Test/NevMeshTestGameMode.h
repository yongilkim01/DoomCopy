#pragma once
#include "GameFramework/GameMode.h"

class ACameraActor;
class ATestMap;
class ATestPlayer;

/**
 *	����
 */
class ANevMeshTestGameMode : public AGameMode
{
public:
	/** ������, �Ҹ��� */
	ANevMeshTestGameMode();
	~ANevMeshTestGameMode();

	/** ��ü �� ���� ���� */
	ANevMeshTestGameMode(const ANevMeshTestGameMode& Other) = delete;
	ANevMeshTestGameMode(ANevMeshTestGameMode&& Other) noexcept = delete;
	ANevMeshTestGameMode& operator=(const ANevMeshTestGameMode& Other) = delete;
	ANevMeshTestGameMode& operator=(ANevMeshTestGameMode&& Other) noexcept = delete;

	virtual void Tick(float DeltaTime) override;

protected:

private:
	std::shared_ptr<ACameraActor> Camera = nullptr;
	std::shared_ptr<ATestPlayer> TestPlayer = nullptr;
	std::shared_ptr<ATestMap> TestMap = nullptr;
};

