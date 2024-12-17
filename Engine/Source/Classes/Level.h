#pragma once

#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
#include "EngineDefine.h"
#include "GameFramework/Actor.h"

/**
 *	���� ���� Ŭ����
 */
class ULevel : public UObject
{
public:
	/** ������, �Ҹ��� */
	ENGINE_API ULevel();
	ENGINE_API ~ULevel();

	/** ��ü �� ���� ���� */
	ULevel(const ULevel& Other) = delete;
	ULevel(ULevel&& Other) noexcept = delete;
	ULevel& operator=(const ULevel& Other) = delete;
	ULevel& operator=(ULevel&& Other) noexcept = delete;

	void LevelChangeStart();
	void LevelChangeEnd();

	void Tick(float DeltaTime);

	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor()
	{
		// ���ø����� ���޹��� ���� Ÿ���� ���͸� ��ӹ��� �ʾ��� ��� üũ
		static_aseert(std::is_base_of_v<AActor, ActorType>, "���͸� ��ӹ��� ���� Ŭ����");
		
		if (false == std::is_base_of_v<AActor, ActorType>)
		{
			MSGASSERT("���͸� ��ӹ��� ���� Ŭ������ SpawnActor �õ�");
			return nullptr;
		}

		char* ActorMemory = new char[sizeof(ActorType)];

		AActor* PtrActor = reinterpret_cast<ActorType*>(ActorMemory);
		PtrActor->World = this;

		ActorType* NewPtr = reinterpret_cast<ActorType*>(ActorMemory);
		std::shared_ptr<ActorType> NewActor(NewPtr = new(ActorMemory) ActorType());

		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

protected:

private:
	std::list<std::shared_ptr<AActor>> BeginPlayList;
	std::list<std::shared_ptr<AActor>> AllActorList;
};