// Created by Cameron Allen
#pragma once

class Event;

class SystemSettings
{
private:
	std::unordered_map<std::string_view, std::pair<std::string_view, std::any>> m_settings;
	std::string m_settingsFilePath; // Resolve filepath in Init()

	// Engine settings callbacks
	void NodeTreeCallback(Event* event);
	void ConsoleCallback(Event* event);
	void NodeInspectorCallback(Event* event);

	// Debug settings callbacks ShadowMap, Normals, Wireframes, Colliders, Velocities
	void ShadowMapCallback(Event* event);
	void NormalsCallback(Event* event);
	void WireframesCallback(Event* event);
	void CollidersCallback(Event* event);
	void VelocitiesCallback(Event* event);

public:
	//@brief Initializes system settings
	void Init();

	//@brief Shuts down system settings and saves them to JSON file
	void Shutdown();

	//@brief Set a setting value
	//@param key : name of the setting
	//@param value : value to set the setting to
	void SetSetting(const std::string_view key, const std::any& value);

	//@brief Get a setting value
	//@param key : name of the setting
	//@return std::any : value of the setting
	std::any* GetSetting(const std::string_view key);

	//@brief Get a float setting value
	//@param key : name of the setting
	float GetFloatSetting(const std::string_view key);
	//@brief Set a float setting value
	//@param key : name of the setting
	//@param value : value to set the setting to
	void SetFloatSetting(const std::string_view key, const float value);

	//@brief Get a bool setting value
	//@param key : name of the setting
	bool GetBoolSetting(const std::string_view key);
	//@brief Set a bool setting value
	//@param key : name of the setting
	//@param value : value to set the setting to
	void SetBoolSetting(const std::string_view key, const bool value);

	//@brief Get a integer setting value
	//@param key : name of the setting
	int GetIntSetting(const std::string_view key);
	//@brief Set a integer setting value
	//@param key : name of the setting
	//@param value : value to set the setting to
	void SetIntSetting(const std::string_view key, const int value);

	//@brief Get the type of a setting associated with a key
	//@param key : name of the setting
	const std::string GetSettingType(const std::string_view key);

	//@brief Get all settings
	std::unordered_map<std::string_view, std::pair<std::string_view, std::any>>& GetSettings()
	{
		return m_settings;
	}

	//@brief Get instance of SystemSettings
	static SystemSettings* GetInstance()
	{
		static std::unique_ptr<SystemSettings> m_instance;
		if (!m_instance)
		{
			m_instance = std::make_unique<SystemSettings>();
		}
		return m_instance.get();
	}
};