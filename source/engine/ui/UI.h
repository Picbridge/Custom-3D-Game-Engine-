// Created by Cameron Allen
#pragma once

class UI
{
private:
	std::vector<std::unique_ptr<IMGUI_ELEMENT>> m_ImGuiElements;
	std::ostringstream m_consoleBuffer;
	IMGUI_DROPDOWN_MENU m_gameSelection = IMGUI_DROPDOWN_MENU("Select Scene");
	GLFWwindow* mp_window = nullptr;
	ImGuiIO* mp_io = nullptr;
	FRAME_BUFFER_PROPS* mp_windowProps = nullptr;
	Scene* mp_scene;
	Node* mp_selectedNode = nullptr;
	float m_mainMenuBarHeight = 0.0f;
	float m_leftWindowWidth = 200.0f;
	float m_rightWindowWidth = 200.0f;
	float m_bottomWindowHeight = 375.0f;
	unsigned short m_gameIndex = 0;
	static bool m_Hovering;
	bool m_toggleDebug = false;
	bool m_isPaused = false;
	void (*mp_renderFunctions[IMGUI_ELEMENT_TYPE::END])(IMGUI_ELEMENT*) = 
	{ RenderButton, RenderToggleMenu, RenderSelectionMenu, RenderSlider, RenderSliderMenu };
	UI() 
	{
		m_gameSelection.type = IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION;
	}
	~UI();
public:
	void Init(GLFWwindow* window);
	void Shutdown();
	void UpdateImGuiKeyboardInputs();
	void UpdateImGuiGamepadInputs();
	void Update();
	void Render();
	static UI& GetInstance();
	const bool& Hovering() const { return m_Hovering; }
	unsigned short GetGameIndex() const { return m_gameIndex; }
	bool GetIsPaused() const { return m_isPaused; }
	bool GetState(const char* elementName, IMGUI_ELEMENT_TYPE elementType) const;
	bool GetState(const char* elementName, const char* subElementName, IMGUI_ELEMENT_TYPE elementType) const;
	float GetSliderValue(const char* elementName) const;
	float GetSliderValue(const char* elementName, const char* subElementName) const;
	void PushGame(const char* name) 
	{ 
		m_gameSelection.items.push_back(std::unique_ptr<IMGUI_ELEMENT>(new IMGUI_ELEMENT(name)));
	}
	void SetDebug(bool state) { m_toggleDebug = state; }
	void ToggleDebug();
	void PushElement(std::unique_ptr<IMGUI_ELEMENT> element)
	{
		m_ImGuiElements.push_back(std::move(element));
	}
	std::ostringstream& GetConsoleBuffer() { return m_consoleBuffer; }

	static void RenderButton(IMGUI_ELEMENT* element);
	static void RenderToggleMenu(IMGUI_ELEMENT* element);
	static void RenderSelectionMenu(IMGUI_ELEMENT* element);
	static void RenderSlider(IMGUI_ELEMENT* element);
	static void RenderSliderMenu(IMGUI_ELEMENT* element);

	void RenderSceneSelection();
	void RenderConsoleWindow();
	void RenderInspectorWindow();
	void RenderNodeComponents();
	void RenderNodesWindow();
	void RenderNode(Node* node);
	void RenderSceneGraph();
};