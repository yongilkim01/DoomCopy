#include "pch.h"
#include "Engine/Classes/Engine/TransformObject.h"

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}

void UTransformObject::TransformUpdate()
{
	ParentMatrixCheck();

	Transform.TransformUpdate(bAbsolute);

	for (UTransformObject* Child : Childs)
	{
		Child->TransformUpdate();
	}

	bAbsolute = false;
}

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> ParentComponent)
{
	SetupAttachment(ParentComponent.get());
}

void UTransformObject::SetupAttachment(UTransformObject* ParentComponent)
{
	Parent = ParentComponent;
	Parent->Childs.push_back(this);

	TransformUpdate();
}

void UTransformObject::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}
