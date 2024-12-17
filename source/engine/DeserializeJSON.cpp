// Created by Cameron Allen

#include "pch.h"
#include "DeserializeJSON.h"
#include "ui/UI.h"
#include "rapidjson.h"
#include "AudioManager.h"

namespace Keys 
{
	// General keys
	const char* NAME = "name";
	const char* SAVE_PATH = "save_path";

	// Audio keys
	const char* VALUE = "value";
	const char* MIN = "min";
	const char* MAX = "max";
	const char* VOLUME = "volume";
	const char* MODE = "mode";

	// UI keys
	const char* BUTTON = "button";
	const char* DROPDOWN_MENU = "dropdown_menu";
	const char* SLIDER = "slider";
	const char* TYPE = "type";
	const char* ITEM_NAMES = "item_names";
}

// Extracts button information from the JSON file
static IMGUI_BUTTON* ExtractButton(rapidjson::Value::ConstMemberIterator& mButton) 
{
	rapidjson::GenericMemberIterator mem = mButton->value.FindMember(Keys::NAME);
	IMGUI_BUTTON* button = nullptr;
	if (mem != mButton->value.MemberEnd())
	{
		button = new IMGUI_BUTTON(mem->value.GetString());
	}
	else
	{
		std::cerr << "\n<DeserializeJSON ERROR>\n" <<
			"\button must have a 'name' attribute!\n\n";
		assert(0);
	}
	if (mem = mButton->value.FindMember(Keys::SAVE_PATH); mem != mButton->value.MemberEnd())
	{
		button->savePath = mem->value.GetString();
		DeserializeJSON::GetElementState(button);
	}
	return button;
}

// Extracts slider information from the JSON file
static IMGUI_SLIDER* ExtractSlider(rapidjson::Value::ConstMemberIterator& slider)
{
	IMGUI_SLIDER* res = new IMGUI_SLIDER();
	rapidjson::GenericMemberIterator mem = slider->value.FindMember(Keys::NAME);
	if (mem != slider->value.MemberEnd())
	{
		res->name = mem->value.GetString();
	}
	else
	{
		std::cerr << "\n<DeserializeJSON ERROR>\n" <<
			"\nmenu must have a 'name' attribute!\n\n";
		assert(0);
	}
	if (mem = slider->value.FindMember(Keys::SAVE_PATH); mem != slider->value.MemberEnd() &&
		mem->value.IsString())
	{
		res->savePath = mem->value.GetString();
		DeserializeJSON::GetElementState(res);
	}
	else
	{
		mem = slider->value.FindMember(Keys::VALUE);
		if (mem != slider->value.MemberEnd() && mem->value.IsFloat())
		{
			res->max = mem->value.GetFloat();
		}
		else
		{
			std::cerr << "\n<DeserializeJSON ERROR>\n" <<
				"\nSlider \"value\" member must exist with a float value!\n\n";
			assert(0);
		}
	}
	mem = slider->value.FindMember(Keys::MIN);
	if (mem != slider->value.MemberEnd() && mem->value.IsFloat())
	{
		res->min = mem->value.GetFloat();
	}
	else
	{
		std::cerr << "\n<DeserializeJSON ERROR>\n" <<
			"\nSlider \"min\" member must exist with a float value!\n\n";
		assert(0);
	}
	mem = slider->value.FindMember(Keys::MAX);
	if (mem != slider->value.MemberEnd() && mem->value.IsFloat())
	{
		res->max = mem->value.GetFloat();
	}
	else
	{
		std::cerr << "\n<DeserializeJSON ERROR>\n" <<
			"\nSlider \"max\" member must exist with a float value!\n\n";
		assert(0);
	}
	return res;
}

// Extracts dropdown menu information from the JSON file
static IMGUI_DROPDOWN_MENU* ExtractDropdownMenu(rapidjson::Value::ConstMemberIterator& menu)
{
	std::string savePath = "";
	IMGUI_DROPDOWN_MENU* res = new IMGUI_DROPDOWN_MENU();
	rapidjson::GenericMemberIterator mem = menu->value.FindMember(Keys::NAME);
	if (mem != menu->value.MemberEnd()) 
	{
		res->name = mem->value.GetString();
	}
	else 
	{
		std::cerr << "\n<DeserializeJSON ERROR>\n" <<
			"\nmenu must have a 'name' attribute!\n\n";
		assert(0);
	}
	if (mem = menu->value.FindMember(Keys::TYPE); mem != menu->value.MemberEnd()) 
	{
		if (mem->value.IsInt()) 
		{
			res->type = static_cast<IMGUI_ELEMENT_TYPE>(mem->value.GetInt());
		}
		else 
		{
			const char* type = mem->value.GetString();
			if (strcmp(type, "toggle") == 0) 
			{
				res->type = IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE;
			}
			else if (strcmp(type, "selection") == 0)
			{
				res->type = IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION;
			}
			else if (strcmp(type, "slider") == 0) 
			{
				res->type = IMGUI_ELEMENT_TYPE::DROPDOWN_SLIDER;
			}
		}
	}
	else
	{
		std::cerr << "\n<DeserializeJSON ERROR>\nmember name: " <<
			"\nmenu must have a 'type' attribute!\n\n";
		assert(0);
	}
	if (mem = menu->value.FindMember(Keys::SAVE_PATH); mem != menu->value.MemberEnd() &&
		mem->value.IsString())
	{
		res->savePath = mem->value.GetString();
		if (res->type == IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION &&
			res->savePath != "")
			DeserializeJSON::GetElementState(res);
	}
	if (mem = menu->value.FindMember(Keys::ITEM_NAMES); mem != menu->value.MemberEnd()) 
	{
		const auto& items = mem->value.GetArray();
		for (const auto& item : items) 
		{
			switch(res->type)
			{
			case IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE:
				{
					IMGUI_BUTTON button(item.GetString());
					if (res->savePath != "")
					{
						button.savePath = res->savePath;
						DeserializeJSON::GetElementState(&button);
					}
					res->items.push_back(std::make_unique<IMGUI_BUTTON>(std::move(button)));
				}
				break;
			case IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION:
				res->items.push_back(std::make_unique<IMGUI_ELEMENT>(item.GetString()));
				break;
			case IMGUI_ELEMENT_TYPE::DROPDOWN_SLIDER:
				{
					IMGUI_SLIDER slider(item.GetString());
					if (res->savePath != "")
					{
						slider.savePath = res->savePath;
						DeserializeJSON::GetElementState(&slider);
					}
					res->items.push_back(std::make_unique<IMGUI_SLIDER>(std::move(slider)));
				}
				break;
			}
		}
	}
	return res;
}

// Loads ImGui elements from the JSON file
void DeserializeJSON::LoadImGui(const char* _fileLoc)
{
	UI& ui = UI::GetInstance();
	rapidjson::Document doc;
	std::ifstream file(_fileLoc);
	
	if (!file.is_open()) 
	{
		std::cerr << "Could not open the file: " << _fileLoc << std::endl;
		assert(1);
	}

	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	if (!doc.Parse(text.c_str()).HasParseError()) 
	{
		rapidjson::Value& root = doc.GetObject();
		for (rapidjson::Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); itr++) 
		{
			if (itr->value.GetType() != rapidjson::kObjectType) {
				std::cerr << "\n<DeserializeJSON ERROR>\nmember name: " << itr->name.GetString() <<
					"\nmember of root object is not an object\n\n";
				assert(0);
			}
			else 
			{
				if (std::strcmp(itr->name.GetString(), Keys::BUTTON) == 0) 
				{
					ui.PushElement(std::unique_ptr<IMGUI_BUTTON>(std::move(ExtractButton(itr))));
				}
				else if (std::strcmp(itr->name.GetString(), Keys::DROPDOWN_MENU) == 0) 
				{
					ui.PushElement(std::unique_ptr<IMGUI_DROPDOWN_MENU>(std::move(ExtractDropdownMenu(itr))));
				}
				else if (std::strcmp(itr->name.GetString(), Keys::SLIDER) == 0) 
				{
					ui.PushElement(std::unique_ptr<IMGUI_SLIDER>(std::move(ExtractSlider(itr))));
				}
			}
		}
	}
}

// Saves the state of an ImGui element to a JSON file
void DeserializeJSON::SaveElementState(IMGUI_ELEMENT* element)
{
	std::ifstream file(element->savePath.c_str());
	if (!file) {
		rapidjson::Document jsonDoc;
		jsonDoc.SetObject();

		rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
		if (IMGUI_BUTTON* button = dynamic_cast<IMGUI_BUTTON*>(element); button) 
		{
			jsonDoc.AddMember(rapidjson::StringRef(button->name.c_str()), static_cast<bool>(button->selected), allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			jsonDoc.Accept(writer);

			std::ofstream outFile(button->savePath.c_str());
			if (!outFile) {
				std::cerr << "Error: Could not open the file for writing!" << std::endl;
				return;
			}

			outFile << buffer.GetString();
			outFile.close();
		}
		else if (IMGUI_SLIDER* slider = dynamic_cast<IMGUI_SLIDER*>(element); slider) 
		{
			jsonDoc.AddMember(rapidjson::StringRef(button->name.c_str()), static_cast<float>(slider->value), allocator);

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			jsonDoc.Accept(writer);

			std::ofstream outFile(slider->savePath.c_str());
			if (!outFile) {
				std::cerr << "Error: Could not open the file for writing!" << std::endl;
				return;
			}

			outFile << buffer.GetString();
			outFile.close();
		}

		std::cout << "JSON successfully written to output.json" << std::endl;
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	rapidjson::Document jsonDoc;
	if (!jsonDoc.Parse(buffer.str().c_str()).HasParseError()) 
	{
		if (jsonDoc.HasMember(element->name.c_str())) 
		{
			if (jsonDoc[element->name.c_str()].IsBool())
			{
				jsonDoc[element->name.c_str()].SetBool(static_cast<IMGUI_BUTTON*>(element)->selected);
			}
			else if (jsonDoc[element->name.c_str()].IsFloat())
			{
				jsonDoc[element->name.c_str()].SetFloat(static_cast<IMGUI_SLIDER*>(element)->value);
			}
			else
			{
				std::cerr << "Error: Key \"" << element->name.c_str() << "\" not found or not correct type!" << std::endl;
				return;
			}
		}
		else 
		{
			rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
			if (IMGUI_BUTTON* button = dynamic_cast<IMGUI_BUTTON*>(element); button) 
			{
				jsonDoc.AddMember(rapidjson::StringRef(button->name.c_str()), static_cast<bool>(button->selected), allocator);
			}
			else if (IMGUI_SLIDER* slider = dynamic_cast<IMGUI_SLIDER*>(element); slider)
			{
				jsonDoc.AddMember(rapidjson::StringRef(slider->name.c_str()), slider->value, allocator);
			}
			else if (IMGUI_DROPDOWN_MENU* menu = dynamic_cast<IMGUI_DROPDOWN_MENU*>(element); menu)
			{
				jsonDoc.AddMember(rapidjson::StringRef(menu->name.c_str()), menu->selected, allocator);
			}
		}

		rapidjson::StringBuffer bufferOut;
		rapidjson::Writer<rapidjson::StringBuffer> writer(bufferOut);
		jsonDoc.Accept(writer);

		std::ofstream outFile(element->savePath.c_str());
		if (!outFile) {
			std::cerr << "Error: Could not open the file for writing!" << std::endl;
			return;
		}

		outFile << bufferOut.GetString();
		outFile.close();

		std::cout << "JSON successfully modified and written to output.json" << std::endl;
	}
}

// Retrieves the state of an ImGui element from a JSON file
void DeserializeJSON::GetElementState(IMGUI_ELEMENT* element)
{
	rapidjson::Document doc;
	std::ifstream file(element->savePath.c_str());
	if (!file.is_open())
		return;

	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	if (!doc.Parse(text.c_str()).HasParseError()) 
	{
		rapidjson::Value& root = doc.GetObject();
		rapidjson::GenericMemberIterator itr = root.MemberBegin();
		if (auto mem = root.FindMember(element->name.c_str()); mem != root.MemberEnd())
		{
			if (mem->value.IsBool()) 
			{
				IMGUI_BUTTON* button = static_cast<IMGUI_BUTTON*>(element);
				button->selected = mem->value.GetBool();
			}
			else if (mem->value.IsFloat()) 
			{
				IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(element);
				slider->value = mem->value.GetFloat();
			}
			else if (mem->value.IsUint()) 
			{
				IMGUI_DROPDOWN_MENU* menu = static_cast<IMGUI_DROPDOWN_MENU*>(element);
				menu->selected = mem->value.GetUint();
			}
		}
	}
}

// Loads audio from a JSON file
void DeserializeJSON::LoadAudio(const char* _fileLoc)
{
	AudioManager* audioManager = SERVICE_LOCATOR.GetAudioManager();

	rapidjson::Document doc;
	std::ifstream file(_fileLoc);

	if (!file.is_open())
	{
		std::cerr << "Could not open the file: " << _fileLoc << std::endl;
		assert(1);
	}

	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	if (!doc.Parse(text.c_str()).HasParseError())
	{
		rapidjson::Value& root = doc.GetObject();
		for (rapidjson::Value::ConstMemberIterator itr = root.MemberBegin();
			itr != root.MemberEnd(); itr++)
		{
			if (itr->value.GetType() != rapidjson::kObjectType) {
				std::cerr << "\n<DeserializeJSON ERROR>\nmember name: " << itr->name.GetString() <<
					"\nmember of root object is not an object\n\n";
				assert(0);
			}

			std::string path = itr->name.GetString();
			FMOD_MODE mode = FMOD_DEFAULT;
			float volume = 1.0f;
			
			auto res = itr->value.FindMember("mode");
			if (res != itr->value.MemberEnd() &&
				res->value.IsString()) 
			{
				if (std::strstr(res->value.GetString(), "loop"))
					mode |= FMOD_LOOP_NORMAL;
				if (std::strstr(res->value.GetString(), "3D"))
					mode |= FMOD_3D;
			}

			res = itr->value.FindMember("volume");
			if (res != itr->value.MemberEnd() &&
				res->value.IsFloat()) 
			{
				volume = res->value.GetFloat();
			}
			audioManager->CreateSound(path, mode, volume);
		}
	}
	else 
	{
		std::cerr << "Parse error: " << doc.GetParseError()
			<< " at offset " << doc.GetErrorOffset() << std::endl;
	}
}
