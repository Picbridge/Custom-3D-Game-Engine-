// Created by Cameron Allen
#pragma once

namespace DeserializeJSON 
{
	void LoadEngineUI(const char* _fileLoc);
	void SaveElementState(IMGUI_ELEMENT* element);
	void GetElementState(IMGUI_ELEMENT* element);
	void RemoveElementState(IMGUI_ELEMENT* element);
	void LoadAudio(const char* _fileLoc);
	void LoadSystemSettings(const char* _fileLoc);
	void SaveSystemSettings(const char* _fileLoc);
}