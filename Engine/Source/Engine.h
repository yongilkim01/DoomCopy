#pragma once

// os
#include <Windows.h>

// std
#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <memory>

#include <wrl.h>
#include <d3d11_4.h> // directx 11 ����4�� ���
#include <d3dcompiler.h> // ���̴� �����Ϸ��� �������̽� ���̴��� ���� ����

// ���̺귯����
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 

#include "Core/Public/Math/EngineMath.h"
#include "Core/Public/Misc/DirectoryHelper.h"
#include "Core/Public/Misc/FileHelper.h"
#include "Core/Public/Misc/Paths.h"
#include "Core/Public/Serialization/Archive.h"
#include "Core/Public/Debugging/DebugMacros.h"

#include "Platform/Public/Window/Window.h"
#include "Platform/Public/Input/EngineInput.h"

#include "Engine/Public/Global/EngineDefine.h"
#include "Engine/Public/Global/EngineEnums.h"
#include "Engine/Public/Global/EngineStruct.h"

#include "Engine/Classes/Engine/GameEngine.h"
#include "Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Classes/GameFramework/GameMode.h"