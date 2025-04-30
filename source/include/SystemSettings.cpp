#include "../engine/pch.h"
#include "../engine/events/BoolEvent.h"

static EventListener* eventListener = new EventListener();

void SystemSettings::NodeTreeCallback(Event* event)
{
	SetSetting("Node Tree", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::ConsoleCallback(Event* event)
{
	SetSetting("Console", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::NodeInspectorCallback(Event* event)
{
	SetSetting("Node Inspector", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::ShadowMapCallback(Event* event)
{
	SetSetting("ShadowMap", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::NormalsCallback(Event* event)
{
	SetSetting("Normals", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::WireframesCallback(Event* event)
{
	SetSetting("Wireframes", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::CollidersCallback(Event* event)
{
	SetSetting("Colliders", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::VelocitiesCallback(Event* event)
{
	SetSetting("Velocities", static_cast<BoolEvent*>(event)->value);
}

void SystemSettings::Init()
{
	m_settings.reserve(25);
	m_settings["Master Volume"] = { "float", 0.5f };
	m_settings["Music Volume"] = { "float", 0.5f };
	m_settings["SFX Volume"] = { "float", 0.5f };
	m_settings["Mute"] = { "bool", false };

	m_settings["Node Tree"] = { "bool", false };
	m_settings["Console"] = { "bool", false };
	m_settings["Node Inspector"] = { "bool", false };
	m_settings["Invert Y-Axis"] = { "bool", false };

	m_settings["ShadowMap"] = { "bool", false };
	m_settings["Normals"] = { "bool", false };
	m_settings["Wireframes"] = { "bool", false };
	m_settings["Colliders"] = { "bool", false };
	m_settings["Velocities"] = { "bool", false };

	m_settings["Pause"] = { "bool", false };
#ifdef _DEBUG
	m_settingsFilePath = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "code" / "json_files").string();
	DeserializeJSON::LoadSystemSettings(std::string(m_settingsFilePath + "\\EngineSettings.json").c_str());
	DeserializeJSON::LoadEngineUI(std::string(m_settingsFilePath + "\\EngineUI.json").c_str());
	m_settingsFilePath += "\\EngineSettings.json";
#else
	m_settings["L0_Time"] = { "float", -1.0f };
	m_settings["L1_Time"] = { "float", -1.0f };
	m_settings["L2_Time"] = { "float", -1.0f };
	m_settings["L3_Time"] = { "float", -1.0f };

	m_settings["L0_Points"] = { "int", 0 };
	m_settings["L1_Points"] = { "int", 0 };
	m_settings["L2_Points"] = { "int", 0 };
	m_settings["L3_Points"] = { "int", 0 };

	m_settingsFilePath = (Utils::GetProfileDirectory() / "Documents" / "DigiPen" / "GrapplingWithIdeas" / "GameSettings.json").string();
	DeserializeJSON::LoadSystemSettings(m_settingsFilePath.data());
#endif

	eventListener->SubscribeToEvent("Node Tree", std::bind(&SystemSettings::NodeTreeCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Console", std::bind(&SystemSettings::ConsoleCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Node Inspector", std::bind(&SystemSettings::NodeInspectorCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("ShadowMap", std::bind(&SystemSettings::ShadowMapCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Normals", std::bind(&SystemSettings::NormalsCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Wireframes", std::bind(&SystemSettings::WireframesCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Colliders", std::bind(&SystemSettings::CollidersCallback, this, std::placeholders::_1));
	eventListener->SubscribeToEvent("Velocities", std::bind(&SystemSettings::VelocitiesCallback, this, std::placeholders::_1));
}

void SystemSettings::Shutdown()
{
	DeserializeJSON::SaveSystemSettings(m_settingsFilePath.data());
	delete(eventListener);
}

void SystemSettings::SetSetting(const std::string_view key, const std::any& value)
{
	std::unordered_map<std::string_view, std::pair<std::string_view, std::any>>::iterator itr = m_settings.find(key);
	if (itr != m_settings.end())
	{
		auto& [typeName, storedValue] = itr->second;
		auto typeItr = GameObjectTypeDictionary::typeStore.find(typeName);
		if (typeItr != GameObjectTypeDictionary::typeStore.end())
		{
			storedValue = value;
		}
		else
		{
			//throw std::runtime_error("Type mismatch");
		}
	}
}

std::any* SystemSettings::GetSetting(const std::string_view key)
{
	auto itr = m_settings.find(key);
	if (itr != m_settings.end())
	{
		return &itr->second.second;
	}
	else
	{
		return nullptr;
	}
}

float SystemSettings::GetFloatSetting(const std::string_view key)
{
	std::any* setting = GetSetting(key);
	if (setting)
	{
		try
		{
			return std::any_cast<float>(*setting);
		}
		catch (const std::bad_any_cast& e)
		{
			std::cout << e.what() << '\n';
			return -1.0f;
		}
	}
	std::cerr << "Setting not found: " << key << std::endl;
	return -1.0f;
}

void SystemSettings::SetFloatSetting(const std::string_view key, const float value)
{
	SetSetting(key, value);
}

bool SystemSettings::GetBoolSetting(const std::string_view key)
{
	std::any* setting = GetSetting(key);
	if (setting)
	{
		try
		{
			return std::any_cast<bool>(*setting);
		}
		catch (const std::bad_any_cast& e)
		{
			std::cout << e.what() << '\n';
			return false;
		}
	}
	std::cerr << "Setting not found: " << key << std::endl;
	return false;
}

void SystemSettings::SetBoolSetting(const std::string_view key, bool value)
{
	SetSetting(key, value);
}

int SystemSettings::GetIntSetting(const std::string_view key)
{
	std::any* setting = GetSetting(key);
	if (setting)
	{
		try
		{
			return std::any_cast<int>(*setting);
		}
		catch (const std::bad_any_cast& e)
		{
			std::cout << e.what() << '\n';
			return 0;
		}
	}
	std::cerr << "Setting not found: " << key << std::endl;
	return 0;
}

void SystemSettings::SetIntSetting(const std::string_view key, const int value)
{
	SetSetting(key, value);
}

const std::string SystemSettings::GetSettingType(const std::string_view key)
{
	auto itr = m_settings.find(key);
	if (itr != m_settings.end())
	{
		return itr->second.first.data();
	}
	else
	{
		std::cerr << "Setting not found: " << key << std::endl;
		return "";
	}
}
