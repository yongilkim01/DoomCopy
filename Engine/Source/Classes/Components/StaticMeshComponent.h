#pragma once
#include "Classes/Components/PrimitiveComponent.h"
#include "Classes/Engine/StaticMesh.h"
#include "Classes/Engine/Texture.h"

/**
 *	설명
 */
class UStaticMeshComponent : public UPrimitiveComponent
{
public:
	/** 생성자, 소멸자 */
	ENGINE_API UStaticMeshComponent();
	ENGINE_API ~UStaticMeshComponent();

	/** 객체 값 복사 방지 */
	UStaticMeshComponent(const UStaticMeshComponent& Other) = delete;
	UStaticMeshComponent(UStaticMeshComponent&& Other) noexcept = delete;
	UStaticMeshComponent& operator=(const UStaticMeshComponent& Other) = delete;
	UStaticMeshComponent& operator=(UStaticMeshComponent&& Other) noexcept = delete;

	/** obj 파일 로드 메소드 */
	ENGINE_API void Init(std::string_view NewObjPath, std::string_view NewMtlPath);

protected:
	/** UObject 상속 메소드 */
	ENGINE_API virtual void BeginPlay() override;

	/** UPrimitiveComponent 상속 메소드 */
	ENGINE_API virtual void Render(UCameraComponent* CameraComponent, float DeltaTime) override;

	virtual void InitShaderResourceView() override;
	virtual void UpdateShaderResourceView() override;

	virtual void InitVertexShader() override;
	virtual void InitPixelShader() override;

private:
	std::string ObjPath = "";
	std::string MtlPath = "";
	std::string Directory = "";

	std::vector<std::shared_ptr<UStaticMesh>> StaticMeshVector;
	std::vector<std::shared_ptr<UTexture>> TextureVector;
};