#include "pch.h"
#include "GUIEditor.h"

#include <Core/EngineCore.h>
#include <Tools/DebugGUI/EngineGUI.h>
#include <Engine/Classes/Engine/Level.h>

UGUIEditor::UGUIEditor()
{
}

UGUIEditor::~UGUIEditor()
{
}

void UGUIEditor::OnGUI()
{
	std::map<std::string, std::shared_ptr<ULevel>> LevelMap = UEngineCore::GetAllLevelMap();

	for (std::pair<const std::string, std::shared_ptr<ULevel>>& Pair : LevelMap)
	{
		if (ImGui::Button(Pair.first.c_str()))
		{
			UEngineCore::OpenLevel(Pair.first);
		}
	}
}

