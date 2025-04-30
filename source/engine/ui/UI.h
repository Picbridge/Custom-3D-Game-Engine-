// Created by Cameron Allen
#pragma once

class ScriptComponent;
class CollisionComponent;
class RenderComponent;
class UIComponent;

class UI
{
	friend void DeserializeJSON::LoadEngineUI(const char*);
	friend class Engine;
private:
	std::ostringstream m_consoleBuffer;
	IMGUI_DROPDOWN_MENU m_sceneSelection = IMGUI_DROPDOWN_MENU("Select Scene");
	static std::map<std::string, ImFont*> m_fontMap;
	std::vector<std::unique_ptr<IMGUI_ELEMENT>> m_engineUIElements;
	std::vector<std::string> m_materialNames;
	std::vector<std::string> m_textureNames;
	static EventListener m_eventListener;
	GLFWwindow* m_pWindow = nullptr;
	ImGuiIO* m_pIO = nullptr;
	static FRAME_BUFFER_PROPS* m_pWindowProps;
	ComponentNames::Index m_selectedComponentIndex = ComponentNames::Index::IndexEnd;
	Scene* m_pScene;
	Node* m_pSelectedNode = nullptr;

	void (*m_pRenderFunctions[IMGUI_ELEMENT_TYPE::END])(IMGUI_ELEMENT*, glm::vec2(*)[2]) =
	{ RenderButtonAction, RenderButtonToggle, RenderSlider, RenderText, RenderToggleMenu, RenderSelectionMenu, RenderActionMenu, RenderSliderMenu };

	float m_mainMenuBarHeight = 0.0f;
	float m_leftWindowWidth = 200.0f;
	float m_rightWindowWidth = 200.0f;
	float m_bottomWindowHeight = 375.0f;
	unsigned short m_gameIndex = 0;
	unsigned short m_stringTypeIndex = 0;
	static bool m_Hovering;
	static bool m_Interacting;
	bool m_toggleDebug = false;
	bool m_isPaused = false;
	bool m_selectedNodeIsUINode = false;

	UI();
	~UI();

	//@brief Event method that creates a sphere GameObject
	void CreateSphere(Event* event);
	//@brief Event method that creates a cube GameObject
	void CreateCube(Event* event);

	//@brief Event method that saves the current scene
	void SaveScene(Event* event);
	//@brief Event method that Deletes a component from a selected node
	void DeleteComponent(Event* event);

	//@brief Swaps node with previous sibling of the same depth
	void NodeMoveBack(Event* event);
	//@brief Swaps node with next sibling of the same depth
	void NodeMoveForward(Event* event);
	//@brief Moves node to the same depth as parent node
	void NodeMoveChild(Event* event);
	//@brief Moves node to be a child of the sibling before it
	void NodeMoveSibling(Event* event);

	//@brief Duplicates a node and its components
	Node* DuplicateNode(Node* node);

	//@brief Renders a delete confirmation button and 
	//calls the lambda function with the given arguments if confirmed
	template<typename Lambda, typename... Args>
	void RenderDeleteButton(Lambda lambda, Args... args);

	//@brief Updates the ImGui keyboard inputs
	void UpdateImGuiKeyboardInputs();
	//@brief Updates the ImGui gamepad inputs
	void UpdateImGuiGamepadInputs();

	//@brief Finds components and sets their data
	void ReadAndSetComponentData(auto& getter, auto& setter);

	//@brief Renders a toggle button
	//@param element : The toggle button to render
	//@param gameUIScale : The scale of the button if it is a Game UI element
	static void RenderButtonToggle(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a action button
	//@param element : The action button to render
	//@param gameUIScale : The scale of the button if it is a Game UI element
	static void RenderButtonAction(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a toggle button dropwdown menu
	static void RenderToggleMenu(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a selection dropdown menu
	static void RenderSelectionMenu(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a action button dropdown menu
	static void RenderActionMenu(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a slider
	static void RenderSlider(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders a slider dropdown menu
	static void RenderSliderMenu(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);
	//@brief Renders Text
	static void RenderText(IMGUI_ELEMENT* element, glm::vec2 (*gameUIPosScale)[2] = nullptr);

	//@brief Renders a node's transform component
	void RenderTransformComponent();
	//@brief Renders a node's render component
	void RenderRenderComponent(RenderComponent* renderComp);
	//@brief Renders a node's collision component
	void RenderCollisionComponent(CollisionComponent* colComp);
	//@brief Renders a node's script component
	void RenderScriptComponent(ScriptComponent* scriptComp);

	//@brief Pushes an element to the engine UI
	void PushElement(std::unique_ptr<IMGUI_ELEMENT> element)
	{ m_engineUIElements.push_back(std::move(element)); }

	//@brief Renders scene selection dropdown menu
	void RenderSceneSelection();
	//@brief Renders the console window
	void RenderConsoleWindow();
	//@brief Renders the inspector window
	void RenderInspectorWindow();
	//@brief Renders the scene graph nodes
	void RenderSceneGraph();

	//@Renders Node component data within the inspector window
	void RenderNodeComponents();
	//@brief Renders UINode component data within the inspector window
	void RenderUINodeComponent();

	//@brief Renders scene graph window
	void RenderNodesWindow();
	//@brief Renders leaf node within the scene graph
	void RenderNode(Node* node, int index, bool isUINode = false);

	//@brief Renders the game UI
	void RenderGameUI();
	void RenderGameUIElement(Node* node, glm::mat4 transform = glm::mat4(1.0f));

	//@brief Renders the engine UI
	void RenderEngineUI();

	//@brief Initializes UI
	void Init(GLFWwindow* window);
	//@brief Shuts down UI
	void Shutdown();

	//@brief Gets the UI console buffer
	std::ostringstream& GetConsoleBuffer() { return m_consoleBuffer; }

	//@brief Populates scene vector with scene names from SceneManager (should only be called once)
	void SetScenes();

	//@brief Updates UI
	void Update();

	//@brief Renders UI
	void Render();

public:
	//@brief retrieves UI instance
	static UI& GetInstance();

	//@brief Returns if the mouse is hovering over an engine UI element
	const bool& Hovering() const { return m_Hovering; }

	//@brief Returns true if interacting (like typing in text box) with the UI
	const bool& Interacting() const { return m_Interacting; }

	//@brief DEPRECATED: returns the game index 
	unsigned short GetGameIndex() const { return m_gameIndex; }

	//@brief returns if the game is paused
	bool GetIsPaused() const { return m_isPaused; }
	//@brief returns if debug mode is enabled
	bool GetDebugState() const { return m_toggleDebug; }

	//@brief gets the state of a UI toggle button
	//@param buttonName : Name of the button
	//@param isEngineUI : If the button is part of the engine UI
	//@param parent : The parent node whose children are being searched
	int GetToggleState(const char* buttonName, bool isEngineUI = false, Node* parent = nullptr) const;
	//@brief gets the state of a UI toggle button within a dropdown menu (Engine UI only)
	//@param elementName : Name of the dropdown element
	//@param subElementName : Name of the sub element (toggle button)
	int GetToggleState(const char* elementName, const char* subElementName) const;

	//@brief gets the state of a UI toggle button
	//@param sliderName : Name of the slider
	//@param isEngineUI : If the button is part of the engine UI
	//@param parent : The parent node whose children are being searched
	float GetSliderValue(const char* sliderName, bool isEngineUI = false, Node* parent = nullptr) const;
	//@brief gets the state of a UI slider within a dropdown menu (Engine UI only)
	//@param elementName : Name of the dropdown element
	//@param subElementName : Name of the sub element (slider)
	float GetSliderValue(const char* elementName, const char* subElementName) const;
	
	//@brief Sets debug mode state (shows engine UI if true)
	void SetDebug(bool state) { m_toggleDebug = state; }
	//@brief Toggles debug mode state (shows engine UI if true)
	void ToggleDebug();

	//@brief Gets the name of the currently selected scene
	const std::string& GetSelectedSceneName() const;
};

template<typename Lambda, typename... Args>
void UI::RenderDeleteButton(Lambda lambda, Args... args)
{
	if (ImGui::Button("Delete"))
	{
		ImVec2 button_pos = ImGui::GetItemRectMin(); // Get button's top-left position
		button_pos.y += ImGui::GetItemRectSize().y;  // Move below the button

		ImGui::SetNextWindowPos(button_pos);
		ImGui::OpenPopup("Confirm Delete");
	}
	if (ImGui::BeginPopup("Confirm Delete", ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Are you sure?");
		if (ImGui::Button("Yes"))
		{
			lambda(args...);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}