// Created by Cameron Allen

#include "pch.h"
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
		button->selected = static_cast<unsigned short>(SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting(button->name));
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
		res->min = 0.0f;
		res->max = 1.0f;
		res->value = SERVICE_LOCATOR.GetSystemSettings()->GetFloatSetting(res->name);
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
			else if (strcmp(type, "action") == 0)
			{
				res->type = IMGUI_ELEMENT_TYPE::DROPDOWN_ACTION;
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
	}
	if (mem = menu->value.FindMember(Keys::ITEM_NAMES); mem != menu->value.MemberEnd())
	{
		const auto& items = mem->value.GetArray();
		for (const auto& item : items)
		{
			switch (res->type)
			{
			case IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE:
			{
				IMGUI_BUTTON button(item.GetString());
				button.selected = static_cast<unsigned short>(SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting(button.name));
				res->items.push_back(std::make_unique<IMGUI_BUTTON>(std::move(button)));
			}
				break;	
			case IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION:
				res->items.push_back(std::make_unique<IMGUI_ELEMENT>(item.GetString()));
				break;
			case IMGUI_ELEMENT_TYPE::DROPDOWN_SLIDER:
			{
				IMGUI_SLIDER slider(item.GetString());
				slider.value = SERVICE_LOCATOR.GetSystemSettings()->GetFloatSetting(slider.name);
				res->items.push_back(std::make_unique<IMGUI_SLIDER>(std::move(slider)));
			}
				break;
			case IMGUI_ELEMENT_TYPE::DROPDOWN_ACTION:
				res->items.push_back(std::make_unique<IMGUI_ELEMENT>(item.GetString()));
				break;
			}
		}
	}
	return res;
}

// Loads ImGui Engine elements from a JSON file
void DeserializeJSON::LoadEngineUI(const char* _fileLoc)
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
			rapidjson::Value floatArray(rapidjson::kArrayType);
			floatArray.PushBack(static_cast<float>(slider->min), allocator);
			floatArray.PushBack(static_cast<float>(slider->max), allocator);
			floatArray.PushBack(static_cast<float>(slider->value), allocator);
			jsonDoc.AddMember(rapidjson::StringRef(slider->name.c_str()), floatArray, allocator);

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
			auto& value = jsonDoc[element->name.c_str()];
			if (value.IsBool())
			{
				value.SetBool(static_cast<IMGUI_BUTTON*>(element)->selected);
			}
			else if (value.IsFloat())
			{
				value.SetFloat(static_cast<IMGUI_SLIDER*>(element)->value);
			}
			else if (value.IsArray()) 
			{
				IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(element);
				value[0].SetFloat(slider->min);
				value[1].SetFloat(slider->max);
				value[2].SetFloat(slider->value);
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
				//jsonDoc.AddMember(rapidjson::StringRef(slider->name.c_str()), slider->value, allocator);
				rapidjson::Value floatArray(rapidjson::kArrayType);
				floatArray.PushBack(static_cast<float>(slider->min), allocator);
				floatArray.PushBack(static_cast<float>(slider->max), allocator);
				floatArray.PushBack(static_cast<float>(slider->value), allocator);
				jsonDoc.AddMember(rapidjson::StringRef(slider->name.c_str()), floatArray, allocator);
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
			/*else if (mem->value.IsArray()) 
			{
				IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(element);
				slider->min = mem->value[0].GetFloat();
				slider->max = mem->value[1].GetFloat();
				slider->value = mem->value[2].GetFloat();
			}*/
		}
	}
}

void DeserializeJSON::RemoveElementState(IMGUI_ELEMENT* element)
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
		if (auto mem = root.FindMember(element->name.c_str()); mem != root.MemberEnd())
		{
			root.RemoveMember(element->name.c_str());

			// **Write updated JSON back to the file**
			std::ofstream outFile(element->savePath);
			if (!outFile.is_open()) {
				return;
			}

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			outFile << buffer.GetString();
			outFile.close();
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
				if (std::strstr(res->value.GetString(), "2D"))
					mode |= FMOD_2D;
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

void DeserializeJSON::LoadSystemSettings(const char* _fileLoc)
{
	rapidjson::Document doc;
	std::ifstream file(_fileLoc);
	if (!file.is_open())
		return;

	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	if (!doc.Parse(text.c_str()).HasParseError())
	{
		SystemSettings* systemSettings = SERVICE_LOCATOR.GetSystemSettings();
		rapidjson::Value& root = doc.GetObject();
		for (auto& setting : systemSettings->GetSettings())
		{
			if (auto mem = root.FindMember(setting.first.data()); mem != root.MemberEnd())
			{
				const auto& value = mem->value;
				if (value.IsFloat())
				{
					systemSettings->SetSetting(setting.first, mem->value.GetFloat());
				}
				else if (value.IsBool())
				{
					systemSettings->SetSetting(setting.first, mem->value.GetBool());
				}
				else if (value.IsInt()) 
				{
					systemSettings->SetSetting(setting.first, mem->value.GetInt());
				}
			}
		}
	}
}

static void AddSystemSettingMember(rapidjson::Document& jsonDoc, rapidjson::Document::AllocatorType& allocator,
	const std::pair<const std::string_view, std::pair<std::string_view, std::any>>& setting) 
{
	auto& [typeName, storedValue] = setting.second;
	GameObjectTypeDictionary::TypeVariant type = GameObjectTypeDictionary::typeStore.at(typeName);
	std::visit([&](auto&& type) {
		using T = std::decay_t<decltype(type)>;
		if constexpr (std::is_same_v<T, float>)
		{
			jsonDoc.AddMember(rapidjson::StringRef(setting.first.data()), std::any_cast<float>(storedValue), allocator);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			jsonDoc.AddMember(rapidjson::StringRef(setting.first.data()), std::any_cast<bool>(storedValue), allocator);
		}
		else if constexpr (std::is_same_v<T, int>)
		{
			jsonDoc.AddMember(rapidjson::StringRef(setting.first.data()), std::any_cast<int>(storedValue), allocator);
		}
		}, type);
}

void DeserializeJSON::SaveSystemSettings(const char* _fileLoc)
{
	std::filesystem::path filePath = _fileLoc;
	filePath = filePath.parent_path();
	
	std::error_code ec;
	while (!filePath.empty() && !std::filesystem::exists(filePath)) 
	{
		if (!std::filesystem::create_directories(filePath, ec)) 
		{
			std::cerr << "<SystemSettings Error>\nCould not create file directories: " << ec.message() << std::endl;
			return;
		}
		filePath = filePath.parent_path();
	}
	std::ifstream file(_fileLoc);
	rapidjson::Document jsonDoc;
	rapidjson::StringBuffer bufferOut;
	std::stringstream buffer;
	if (!file) {
		jsonDoc.SetObject();
		rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
		for (auto& setting : SERVICE_LOCATOR.GetSystemSettings()->GetSettings()) 
		{
			AddSystemSettingMember(jsonDoc, allocator, setting);
		}
	}
	else 
	{
		buffer << file.rdbuf();
		file.close();

		if (!jsonDoc.Parse(buffer.str().c_str()).HasParseError())
		{
			rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
			for (auto& setting : SERVICE_LOCATOR.GetSystemSettings()->GetSettings())
			{
				if (jsonDoc.HasMember(setting.first.data()))
				{
					auto& [typeName, storedValue] = setting.second;
					GameObjectTypeDictionary::TypeVariant type = GameObjectTypeDictionary::typeStore.at(typeName);
					auto& value = jsonDoc[setting.first.data()];
					std::visit([&](auto&& type) {
						using T = std::decay_t<decltype(type)>;
						auto& value = jsonDoc[setting.first.data()];
						if (value.IsFloat() && std::is_same_v<T, float>)
						{
							value.SetFloat(std::any_cast<float>(storedValue));
						}
						else if (value.IsBool() && std::is_same_v<T, bool>)
						{
							value.SetBool(std::any_cast<bool>(storedValue));
						}
						else if (value.IsInt() && std::is_same_v<T, int>) 
						{
							value.SetInt(std::any_cast<int>(storedValue));
						}
					}, type);
				}
				else
				{
					AddSystemSettingMember(jsonDoc, allocator, setting);
				}
			}
		}
	}
	rapidjson::Writer<rapidjson::StringBuffer> writer(bufferOut);
	jsonDoc.Accept(writer);

	std::ofstream outFile(_fileLoc);
	if (!outFile) {
		std::cerr << "Error: Could not open the file for writing!" << std::endl;
		return;
	}

	outFile << bufferOut.GetString();
	outFile.close();

	std::cout << "JSON successfully written to " << _fileLoc << std::endl;
}
