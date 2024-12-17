// Created by Cameron Allen
#pragma once

namespace DeserializeJSON 
{
	void LoadImGui(const char* _fileLoc);
	void SaveElementState(IMGUI_ELEMENT* element);
	void GetElementState(IMGUI_ELEMENT* element);

	void LoadAudio(const char* _fileLoc);
}