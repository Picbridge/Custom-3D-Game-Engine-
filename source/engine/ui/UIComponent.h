#pragma once

class UIComponent : public Component
{
	friend class UI;
	friend class SceneManager;
public:
	UIComponent();
	~UIComponent();

	//@brief Initialize the component
	void Init() override;

	//@brief Update the component
	virtual void Update() override;

	//@brief Shutdown the component
	void Shutdown() override;

	//@brief Returns the position of the button
	//@return glm::vec2 : the position of the button
	glm::vec2 GetPosition();

	//@brief Sets the position of the button
	//@param pos : The desired position of the button
	void SetPosition(glm::vec2 pos);

	//@brief Returns the scale of the button
	//@return glm::vec2 : the scale of the button
	glm::vec2 GetScale();

	//@brief Sets the scale of the button
	//@param scale : The desired scale of the button
	void SetScale(glm::vec2 scale);

	//@brief Returns the save path of the button
	//@return const char* : The save path of the button
	std::string GetSaveFilePath() const;

	//@brief Sets the save path of the button
	//@param saveFilePath : The desired save path of the button
	void SetSaveFilePath(const std::string& saveFilePath);

	//@brief Returns the type of the button
	//@return IMGUI_ELEMENT_TYPE : The type of the button
	int GetElementType() const;

	//@brief Sets the type of the button
	//@param type : The desired type of the button
	void SetElementType(int type);

	//@brief Creates/Overwrites a UI element based on type and if it has a save file
	//@param type : The type of the UI element
	//@param saveFilePath : The save path of the UI element
	void CreateUIElement(IMGUI_ELEMENT_TYPE type, const char* saveFilePath = "");
	virtual void CreateUIElement();
	
	//@brief Assigns the state of the UI element from JSON file if it exists
	//@param saveFilePath : The save path of the UI element
	void SetElementState(const char* saveFilePath);

	void SetColor(glm::vec3 _color);
	const glm::vec3 GetColor() const;

	void SetFont(const std::string& _fontPath);
	const char* GetFont() const { return m_fontName.c_str(); }

protected:
	std::string m_savePath = "";
	std::string m_fontName = "";
	glm::vec3 m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	std::unique_ptr<IMGUI_ELEMENT> m_pUIElement;
	IMGUI_ELEMENT_TYPE m_type = IMGUI_ELEMENT_TYPE::BUTTON_ACTION;

	//@brief If UIComponent is associated with a system setting, assigns element value to the setting
	void UpdateSettingsElement();

	virtual void defineMember() override;
};
