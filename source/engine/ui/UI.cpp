// Created by Cameron Allen

#include "../pch.h"
#include "UI.h"
#include "../Input.h"
#include "RenderComponent.h"
#include "../DeserializeJSON.h"
#include "../scenemanager/SceneManager.h"

bool UI::m_Hovering = false;

void UI::Init(GLFWwindow* window)
{
	mp_window = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	mp_io = &ImGui::GetIO();
	mp_windowProps = &SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	mp_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    mp_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    mp_scene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene(); // Because UI::Init() is called in Engine::init() before initGames() starts deserializing things (including our scene), this will always be nullptr. mp_scene is also never modified anywhere else. It was previously only referenced in UI::RenderSceneGraph().
}

void UI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::UpdateImGuiKeyboardInputs()
{
    Input& input = SERVICE_LOCATOR.GetInput()->GetInstance();

    if (m_toggleDebug)
	{
		mp_io->AddKeyEvent(ImGuiKey_Tab, input.IsKeyPressed(GLFW_KEY_TAB));
		//mp_io->AddKeyEvent(ImGuiKey_Escape, input.IsKeyJustPressed(GLFW_KEY_ESCAPE));
		mp_io->AddKeyEvent(ImGuiKey_Enter, input.IsKeyPressed(GLFW_KEY_ENTER));
		mp_io->AddKeyEvent(ImGuiKey_Space, input.IsKeyPressed(GLFW_KEY_SPACE));

		mp_io->AddKeyEvent(ImGuiKey_DownArrow, input.IsKeyPressed(GLFW_KEY_DOWN));
		mp_io->AddKeyEvent(ImGuiKey_UpArrow, input.IsKeyPressed(GLFW_KEY_UP));
		mp_io->AddKeyEvent(ImGuiKey_LeftArrow, input.IsKeyPressed(GLFW_KEY_LEFT));
		mp_io->AddKeyEvent(ImGuiKey_RightArrow, input.IsKeyPressed(GLFW_KEY_RIGHT));
    }
}

void UI::UpdateImGuiGamepadInputs()
{
    Input& input = SERVICE_LOCATOR.GetInput()->GetInstance();

    // player 1 id
    unsigned int jid = 0;

    // Map gamepad buttons
    if (m_toggleDebug) 
    {
        mp_io->AddKeyEvent(ImGuiKey_GamepadStart, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_START));

        mp_io->AddKeyEvent(ImGuiKey_GamepadDpadLeft, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_LEFT));
		mp_io->AddKeyEvent(ImGuiKey_GamepadDpadRight, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT));
		mp_io->AddKeyEvent(ImGuiKey_GamepadDpadUp, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_UP));
		mp_io->AddKeyEvent(ImGuiKey_GamepadDpadDown, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_DOWN));

		mp_io->AddKeyEvent(ImGuiKey_GamepadFaceDown, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_A));
		mp_io->AddKeyEvent(ImGuiKey_GamepadFaceRight, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_B));
		mp_io->AddKeyEvent(ImGuiKey_GamepadFaceLeft, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_X));
    }
}

void UI::Update()
{
    UpdateImGuiKeyboardInputs();
	UpdateImGuiGamepadInputs();
}

void UI::Render()
{
    m_Hovering = false;
    if (!m_toggleDebug) 
        return;

	// Debugging code
    /*static unsigned int test = 0;
    if (test <= 10000)
        std::cout << "Testing " << test++ << std::endl;*/

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
	
    if (ImGui::BeginMainMenuBar()) {
		ImVec2 mainMenuBarSize = ImGui::GetWindowSize();
		m_mainMenuBarHeight = mainMenuBarSize.y;

        // Check if the mouse is inside or focused on the menu bar
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;

        for (auto& element : m_ImGuiElements) 
            mp_renderFunctions[element->type](element.get());

        RenderSceneSelection();

        // Fixed size for buttons
        ImVec2 buttonSize = ImVec2(40, 20); // Width: 40, Height: 20 (adjust as needed)

        // Play button
        ImGui::PushID("PlayButton");
        if (ImGui::Button("Play", buttonSize)) {
			m_isPaused = false;
        }
        ImGui::PopID();

        ImGui::SameLine();

        // Pause button
        ImGui::PushID("PauseButton");
        if (ImGui::Button("Pause", buttonSize)) {
			m_isPaused = true;
        }
        ImGui::PopID();

        // Display the frame rate on the far right
        if (mp_windowProps->Width >= 790)
        {
            ImGui::SetCursorPosX(static_cast<float>(mp_windowProps->Width - 90));
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        }

        ImGui::EndMainMenuBar();
    }

    if (GetState("Windows", "Node Tree", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
        RenderNodesWindow();

    if (GetState("Windows", "Console", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
		RenderConsoleWindow();

	if (GetState("Windows", "Node Inspector", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
		RenderInspectorWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

UI& UI::GetInstance()
{
	static UI instance;
	return instance;
}

bool UI::GetState(const char* elementName, IMGUI_ELEMENT_TYPE elementType) const
{
    for (auto& element : m_ImGuiElements) 
    {
        if (strcmp(element->name.c_str(), elementName) == 0) 
        {
            if (element->type == IMGUI_ELEMENT_TYPE::BUTTON) 
            {
                return (bool)element->selected;
            }
            else
            {
                std::cerr << "<UI ERROR>\nInvalid GetState element type.\n\n";
                return false;
            }
        }
    }
    return false;
}

bool UI::GetState(const char* elementName, const char* subElementName, IMGUI_ELEMENT_TYPE elementType) const
{
    for (auto& element : m_ImGuiElements)
    {
        if (strcmp(element->name.c_str(), elementName) == 0)
        {
            if (element->type == IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE)
            {
                IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element.get());
                for (auto& item : ddMenu->items)
                {
                    if (strcmp(item.get()->name.c_str(), subElementName) == 0)
                    {
                        return item.get()->selected;
                    }
                }
            }
            else
            {
                std::cerr << "<UI ERROR>\nInvalid GetState element type.\n\n";
                return false;
            }
        }
    }
    return false;
}

float UI::GetSliderValue(const char* elementName) const
{
    for (auto& element : m_ImGuiElements)
    {
        if (strcmp(element->name.c_str(), elementName) == 0)
        {
            if (element->type == IMGUI_ELEMENT_TYPE::SLIDER)
            {
				return static_cast<IMGUI_SLIDER*>(element.get())->value;
            }
            else
            {
                std::cerr << "<UI ERROR>\nInvalid GetSliderValue element type.\n\n";
                return -1.0f;
            }
        }
    }
    std::cerr << "<UI ERROR> slider not found!\n\n";
    return -1.0f;
}

float UI::GetSliderValue(const char* elementName, const char* subElementName) const
{
    for (auto& element : m_ImGuiElements)
    {
        if (strcmp(element->name.c_str(), elementName) == 0)
        {
			IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element.get());
            for (auto& item : ddMenu->items) 
            {
				if (strcmp(item.get()->name.c_str(), subElementName) == 0)
				{
					if (item->type == IMGUI_ELEMENT_TYPE::SLIDER)
					{
						return static_cast<IMGUI_SLIDER*>(item.get())->value;
					}
					else
					{
						std::cerr << "<UI ERROR>\nInvalid GetSliderValue element type.\n\n";
						return -1.0f;
					}
				}
            }
        }
    }
    std::cerr << "<UI ERROR> slider not found!\n\n";
    return -1.0f;
}

void UI::ToggleDebug()
{
    m_toggleDebug = !m_toggleDebug;
    m_toggleDebug ? 
        glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
        glfwSetInputMode(mp_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UI::RenderButton(IMGUI_ELEMENT* element)
{
	IMGUI_BUTTON* button = static_cast<IMGUI_BUTTON*>(element);
    bool state = button->selected == 0 ? false : true;
    if (ImGui::MenuItem(button->name.c_str(), NULL, &state)) {
        if (state) {
            // Button was toggled on
            printf("%s are now ON\n", button->name.c_str());
        }
        else {
            // Button was toggled off
            printf("%s are now OFF\n", button->name.c_str());
        }
    }
    button->selected = (unsigned short)state;
}

void UI::RenderToggleMenu(IMGUI_ELEMENT* element)
{
    IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element);
    if (ImGui::BeginMenu(ddMenu->name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of toggleable buttons
        for (int i = 0; i < ddMenu->items.size(); ++i) {
			IMGUI_ELEMENT* toggleItem = ddMenu->items[i].get();
            // Display each menu item as a toggleable button
			bool state = toggleItem->selected == 0 ? false : true;
            if (ImGui::MenuItem(toggleItem->name.c_str(), NULL, &state)) {
                if (state) {
                    // Button was toggled on
                    printf("%s are now ON\n", toggleItem->name.c_str());
                }
                else {
                    // Button was toggled off
                    printf("%s are now OFF\n", toggleItem->name.c_str());
                }
            }
            toggleItem->selected = (unsigned short)state;
        }
        ImGui::EndMenu();
    }
}

void UI::RenderSelectionMenu(IMGUI_ELEMENT* element)
{
	IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element);
    if (ImGui::BeginMenu(ddMenu->name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of toggleable menu items, but only one can be selected
        for (int i = 0; i < ddMenu->items.size(); ++i) {
            bool is_selected = (ddMenu->selected == i);
            if (ImGui::MenuItem(ddMenu->items[i]->name.c_str(), NULL, &is_selected)) {
                ddMenu->selected = i;
                // Perform an action when the item is selected
                ImGui::Text("%s is now SELECTED", ddMenu->items[i]->name.c_str());
            }
        }
        ImGui::EndMenu();
    }
}

// Renders a slider element
void UI::RenderSlider(IMGUI_ELEMENT* element)
{
	IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(element);
    ImGui::SetNextItemWidth(150.0f); // Set width for the first slider
	ImGui::SliderFloat(slider->name.c_str(), &slider->value, slider->min, slider->max);
}

// Renders a slider menu
void UI::RenderSliderMenu(IMGUI_ELEMENT* element)
{
    IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element);
    if (ImGui::BeginMenu(ddMenu->name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of adjustable sliders
        for (int i = 0; i < ddMenu->items.size(); ++i)
			RenderSlider(ddMenu->items[i].get());

        ImGui::EndMenu();
    }
}

// Renders scene selection dropdown menu
void UI::RenderSceneSelection()
{
    if (ImGui::BeginMenu(m_gameSelection.name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of toggleable menu items, but only one can be selected
        for (int i = 0; i < m_gameSelection.items.size(); ++i) {
            bool is_selected = (m_gameIndex == i);
            if (ImGui::MenuItem(m_gameSelection.items[i]->name.c_str(), NULL, &is_selected)) {
                m_gameIndex = i;
                // Perform an action when the item is selected
                ImGui::Text("%s is now SELECTED", m_gameSelection.items[i]->name.c_str());
            }
            else 
				m_gameSelection.items[i]->selected = 0;
        }
        ImGui::EndMenu();
    }
}

// Renders debug console window
void UI::RenderConsoleWindow()
{
    static float consoleTextSize = 16.0f;

    // Set the position and size for the bottom window
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(mp_windowProps->Width), m_bottomWindowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, mp_windowProps->Height - m_bottomWindowHeight), ImGuiCond_Always);

    // Create a resizable window without manual repositioning
    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Check if the mouse is inside or focused on the window
    if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
        m_Hovering = true;

    ImGui::SetWindowFontScale(consoleTextSize / 16.0f);

    // Checkbox to toggle auto-scrolling
    static bool autoScroll = true;
    static bool forceAutoScroll = false; // Track when to force scrolling
    
    if (ImGui::Checkbox("Auto-Scroll", &autoScroll)) {
        // When the checkbox is toggled on, force scroll to bottom
        if (autoScroll) {
            forceAutoScroll = true;
        }
    }

    // Convert captured output to a string
    std::string output = m_consoleBuffer.str();

    // Create a scrollable region for the text output
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), true);

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        float scrollAmount = ImGui::GetIO().MouseWheel;
        consoleTextSize += scrollAmount;

        if (consoleTextSize < 8.0f)
			consoleTextSize = 8.0f;
		else if (consoleTextSize > 64.0f)
			consoleTextSize = 64.0f;
    }
    
    // Check if the mouse is inside or focused on the window
    if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
        m_Hovering = true;

    // Display the text
    ImGui::TextUnformatted(output.c_str());

    // Auto-scroll logic
    if (autoScroll && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() || forceAutoScroll)) {
        ImGui::SetScrollHereY(1.0f); // Scroll to the bottom
		forceAutoScroll = false;
    }

    ImGui::EndChild();

    // Update height based on user's interaction with the window size
    m_bottomWindowHeight = ImGui::GetWindowHeight();

    // End the ImGui window
    ImGui::End();
}

// Renders inspector window
void UI::RenderInspectorWindow()
{
    static float inspectorWindowTextSize = 16.0f;
    static float prevRightWindowWidth = m_rightWindowWidth;
    static float prevRightWindowHeight = static_cast<float>(mp_windowProps->Height); // Since height is fixed initially

    // Set the position to the left side of the window
    ImGui::SetNextWindowPos(ImVec2(mp_windowProps->Width - m_rightWindowWidth, m_mainMenuBarHeight), ImGuiCond_Always);
    // Set the size so that it spans the full height and has adjustable width
    ImGui::SetNextWindowSize(ImVec2(m_rightWindowWidth, static_cast<float>(
        GetState("Windows", "Console", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE) ?
        mp_windowProps->Height - m_bottomWindowHeight - m_mainMenuBarHeight : mp_windowProps->Height)), ImGuiCond_Always);

    // Create a resizable window without manual repositioning
    ImGui::Begin("Node Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Check if the mouse is inside or focused on the window
    if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
        m_Hovering = true;

    ImGui::SetWindowFontScale(inspectorWindowTextSize / 16.0f);

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        float scrollAmount = ImGui::GetIO().MouseWheel;
        inspectorWindowTextSize += scrollAmount;

        if (inspectorWindowTextSize < 8.0f)
            inspectorWindowTextSize = 8.0f;
        else if (inspectorWindowTextSize > 64.0f)
            inspectorWindowTextSize = 64.0f;
    }

    // Render contents here
    if (mp_selectedNode) 
    {
		RenderNodeComponents();
    }
    else 
    {
		ImGui::Text("No node selected");
    }

    // Optional: adjust width based on user's interaction with window size
    m_rightWindowWidth = ImGui::GetWindowWidth();
    float currentHeight = ImGui::GetWindowHeight();

    // Update previous dimensions for the next frame
    prevRightWindowWidth = m_rightWindowWidth;
    prevRightWindowHeight = currentHeight;

    // End the ImGui window
    ImGui::End();
}

// Renders the components of a selected node in the inspector window
void UI::RenderNodeComponents()
{
    ImGui::Text("ID: %d", mp_selectedNode->GetID());
	ImGui::Text("Name: %s", mp_selectedNode->GetName().c_str());
	ImGui::Text("\nChildren: %d", mp_selectedNode->GetChildren().size());
	ImGui::Separator();

    if (ImGui::TreeNodeEx("Transform",
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed))
    {
        if (ImGui::TreeNodeEx("Position",
            ImGuiTreeNodeFlags_OpenOnArrow)) 
        {
            glm::vec3 position = mp_selectedNode->GetTransform()->GetPosition();
			ImGui::InputFloat("X", &position.x, 1.0f, 10.0f, "%.2f");
			ImGui::InputFloat("Y", &position.y, 1.0f, 10.0f, "%.2f");
			ImGui::InputFloat("Z", &position.z, 1.0f, 10.0f, "%.2f");
			mp_selectedNode->GetTransform()->SetPosition(position);
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Rotation",
            ImGuiTreeNodeFlags_OpenOnArrow)) 
        {
			glm::vec3 rotation = mp_selectedNode->GetTransform()->GetRotation();
			ImGui::InputFloat("X", &rotation.x, 0.5f, 10.0f, "%.2f");
			ImGui::InputFloat("Y", &rotation.y, 0.5f, 10.0f, "%.2f");
			ImGui::InputFloat("Z", &rotation.z, 0.5f, 10.0f, "%.2f");
			mp_selectedNode->GetTransform()->SetRotation(rotation);
			ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Scale",
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
			glm::vec3 scale = mp_selectedNode->GetTransform()->GetScale();
			ImGui::InputFloat("X", &scale.x, 0.1f, 1.0f, "%.2f");
			ImGui::InputFloat("Y", &scale.y, 0.1f, 1.0f, "%.2f");
			ImGui::InputFloat("Z", &scale.z, 0.1f, 1.0f, "%.2f");
			mp_selectedNode->GetTransform()->SetScale(scale);
			ImGui::TreePop();
        }

        ImGui::TreePop();
    }
    

    // Display the components of the selected node
    GameObject* go = dynamic_cast<GameObject*>(mp_selectedNode);
    if (go) 
    {
		auto renderComp = go->GetComponent<RenderComponent>();
        if (renderComp) 
        {
			if (ImGui::TreeNodeEx("RenderComponent", 
                ImGuiTreeNodeFlags_OpenOnArrow |ImGuiTreeNodeFlags_Framed))
            {
                if (ImGui::TreeNodeEx("Color",
                    ImGuiTreeNodeFlags_OpenOnArrow))
                {
                    glm::vec3 color = renderComp->GetMaterial()->GetColor();
					ImGui::InputFloat("R", &color.r, 0.001f, 0.1f, "%.3f");
                    ImGui::InputFloat("G", &color.g, 0.001f, 0.1f, "%.3f");
                    ImGui::InputFloat("B", &color.b, 0.001f, 0.1f, "%.3f");
					color = glm::clamp(color, 0.0f, 1.0f);
					renderComp->GetMaterial()->SetColor(color);
					ImGui::TreePop();
                }

                float shininess = renderComp->GetMaterial()->GetShininess();
                ImGui::Text("\nShininess");
				ImGui::InputFloat("##s", &shininess, 0.001f, 0.1f, "%.3f");
				renderComp->GetMaterial()->SetShininess(shininess);

                ImGui::TreePop();
            }
        }
    }
}

// Renders node tree window
void UI::RenderNodesWindow()
{
    static float goWindowTextSize = 16.0f;
    static float prevLeftWindowWidth = m_leftWindowWidth;
    static float prevLeftWindowHeight = static_cast<float>(mp_windowProps->Height); // Since height is fixed initially

    // Set the position to the left side of the window
    ImGui::SetNextWindowPos(ImVec2(0, m_mainMenuBarHeight), ImGuiCond_Always);
    // Set the size so that it spans the full height and has adjustable width
    ImGui::SetNextWindowSize(ImVec2(m_leftWindowWidth, static_cast<float>(
        GetState("Windows", "Console", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE) ?
        mp_windowProps->Height - m_bottomWindowHeight - m_mainMenuBarHeight : mp_windowProps->Height)), ImGuiCond_Always);

    // Create a resizable window without manual repositioning
    ImGui::Begin("Node Tree", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Check if the mouse is inside or focused on the window
    if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
        m_Hovering = true;

    ImGui::SetWindowFontScale(goWindowTextSize / 16.0f);

    if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
    {
        float scrollAmount = ImGui::GetIO().MouseWheel;
        goWindowTextSize += scrollAmount;

        if (goWindowTextSize < 8.0f)
            goWindowTextSize = 8.0f;
        else if (goWindowTextSize > 64.0f)
            goWindowTextSize = 64.0f;
    }

    // Display the scene graph
    RenderSceneGraph();

    // Optional: adjust width based on user's interaction with window size
    m_leftWindowWidth = ImGui::GetWindowWidth();
    float currentHeight = ImGui::GetWindowHeight();

    // Update previous dimensions for the next frame
    prevLeftWindowWidth = m_leftWindowWidth;
    prevLeftWindowHeight = currentHeight;

    // End the ImGui window
    ImGui::End();
}

// Recursively renders the node tree
void UI::RenderNode(Node* node)
{
    if (!node)
        std::cout << "NULL!\n";
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

    if (mp_selectedNode == node)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
    if (node->GetChildren().size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    const char* test = node->GetName().c_str();

    if (ImGui::TreeNodeEx(node->GetName().c_str(), nodeFlags)) {
        if (ImGui::IsItemClicked()) 
        {
			mp_selectedNode = node;
            std::cout << mp_selectedNode->GetName().c_str() << " selected" << std::endl;
        }
		for (auto* child : node->GetChildren()) {
			RenderNode(child);
		}
        ImGui::TreePop(); // Ends the tree node
    }
}

// Renders the node tree by Rendering each node at the root level
void UI::RenderSceneGraph()
{
	for (auto& node : SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetNodes())
		RenderNode(node);
}

// Upon destruction, save the state of each element that has a save path
UI::~UI()
{
    for (auto& element : m_ImGuiElements)
    {
        if (element->savePath != "") 
        {
            if (element->type == IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE ||
                element->type == IMGUI_ELEMENT_TYPE::DROPDOWN_SLIDER)
            {
                IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element.get());
				for (auto& item : ddMenu->items)
					DeserializeJSON::SaveElementState(item.get());
            }
            else
                DeserializeJSON::SaveElementState(element.get());
        }
    }
}
