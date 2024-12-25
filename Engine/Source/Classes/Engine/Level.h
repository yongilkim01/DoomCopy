#pragma once

#include "Core/Object/Object.h"
#include "WorldPartition/DebugHelpers.h"
//#include "GameFramework/Actor.h"

class UPrimitiveComponent;
class AActor;
class ACameraActor;

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
	void Render(float DeltaTime);

	void ChangeRenderGroup(int CameraOrder, int PrevGroupOrder, std::shared_ptr<class UPrimitiveComponent> Renderer);
	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor()
	{
		static_assert(std::is_base_of_v<AActor, ActorType>, "���͸� ��ӹ��� ���� Ŭ����");

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

	template<typename EnumType>
	std::shared_ptr<class ACameraActor> SpawnCamera(EnumType Order)
	{
		return SpawnCamera(static_cast<int>(Order));
	}
	std::shared_ptr<class ACameraActor> SpawnCamera(int Order);

	/** ��, �� �޼ҵ� */
	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return GetCamera(0);
	}

	std::shared_ptr<ACameraActor> GetCamera(int CameraOrder)
	{
		if (false == Cameraes.contains(CameraOrder))
		{
			MSGASSERT("�������� �ʴ� ī�޶��Դϴ�.");
		}
		return Cameraes[CameraOrder];
	}


protected:

private:
	std::list<std::shared_ptr<AActor>> BeginPlayList;
	std::list<std::shared_ptr<AActor>> AllActorList;
	std::map<int, std::list<std::shared_ptr<UPrimitiveComponent>>> RendererMap;
	std::map<int, std::shared_ptr<ACameraActor>> Cameraes;

};