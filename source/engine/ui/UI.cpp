// Created by Cameron Allen

#include "../pch.h"
#include "UI.h"
#include "../Input.h"
#include "../DeserializeJSON.h"
#include "../scenemanager/SceneManager.h"
#include "../objectmanager/GameObjectManager.h"
#include "../resourcemanager/ResourceManager.h"
#include "../ComponentIncludes.h"

FRAME_BUFFER_PROPS* UI::m_pWindowProps = nullptr;
bool UI::m_Hovering = false;
bool UI::m_Interacting = false;
EventListener UI::m_eventListener;
std::string matName(25, '\0');
std::map<std::string, ImFont*> UI::m_fontMap;

constexpr std::string_view colShapes[] =
{
    CollisionShapeConstants::SPHERE,
    CollisionShapeConstants::CUBOID
};

const std::pair<std::string_view, IMGUI_ELEMENT_TYPE> GameUITypes[4]
{
    { "Button Action", IMGUI_ELEMENT_TYPE::BUTTON_ACTION },
    { "Button Toggle", IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE },
    { "Slider", IMGUI_ELEMENT_TYPE::SLIDER },
    { "Text", IMGUI_ELEMENT_TYPE::TEXT }
};

enum StringTypes
{
    Default,
    FilePath,
    End
};

std::string_view stringTypes[2] =
{
    "Default",
    "File Path"
};

const std::string defaultDirectory = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content").string();
const std::string fontDirectory = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "art" / "fonts").string();
const std::string scriptDirectory = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "code" / "scripts").string();
const std::string jsonGameUISaveFile = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "code" / "json_files" / "GameUIStates.json").string();

void UI::Init(GLFWwindow* window)
{
    m_pWindow = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_pIO = &ImGui::GetIO();
    m_pWindowProps = &SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    m_pScene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene(); // Because UI::Init() is called in Engine::init() before initGames() starts deserializing things (including our scene), this will always be nullptr. mp_scene is also never modified anywhere else. It was previously only referenced in UI::RenderSceneGraph().

    m_eventListener.SubscribeToEvent("New Sphere", std::bind(&UI::CreateSphere, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("New Cube", std::bind(&UI::CreateCube, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("Save Scene", std::bind(&UI::SaveScene, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("Delete Component", std::bind(&UI::DeleteComponent, this, std::placeholders::_1));

    m_eventListener.SubscribeToEvent("Node Move: Forward", std::bind(&UI::NodeMoveForward, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("Node Move: Backward", std::bind(&UI::NodeMoveBack, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("Node Move: Child", std::bind(&UI::NodeMoveChild, this, std::placeholders::_1));
    m_eventListener.SubscribeToEvent("Node Move: Sibling", std::bind(&UI::NodeMoveSibling, this, std::placeholders::_1));

    auto& materials = SERVICE_LOCATOR.GetResourceManager()->GetMaterials();
    for (auto& mat : materials)
    {
        m_materialNames.push_back(mat.first);
    }
    for (auto& tex : SERVICE_LOCATOR.GetResourceManager()->GetTextures())
    {
        m_textureNames.push_back(tex.first);
    }


    m_pIO->Fonts->Clear();
    m_pIO->Fonts->AddFontDefault();

    for (const auto& entry : std::filesystem::directory_iterator(fontDirectory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ttf")
        {
            std::string fontName = entry.path().filename().string();
            ImFont* font = m_pIO->Fonts->AddFontFromFileTTF(entry.path().string().c_str(), 39.0f);
            m_fontMap[fontName] = font;
        }
    }

#ifdef _DEBUG
    m_isPaused = true;
#endif
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
        m_pIO->AddKeyEvent(ImGuiKey_Tab, input.IsKeyPressed(GLFW_KEY_TAB));
        //mp_io->AddKeyEvent(ImGuiKey_Escape, input.IsKeyJustPressed(GLFW_KEY_ESCAPE));
        m_pIO->AddKeyEvent(ImGuiKey_Enter, input.IsKeyPressed(GLFW_KEY_ENTER));
        m_pIO->AddKeyEvent(ImGuiKey_Space, input.IsKeyPressed(GLFW_KEY_SPACE));

        m_pIO->AddKeyEvent(ImGuiKey_DownArrow, input.IsKeyPressed(GLFW_KEY_DOWN));
        m_pIO->AddKeyEvent(ImGuiKey_UpArrow, input.IsKeyPressed(GLFW_KEY_UP));
        m_pIO->AddKeyEvent(ImGuiKey_LeftArrow, input.IsKeyPressed(GLFW_KEY_LEFT));
        m_pIO->AddKeyEvent(ImGuiKey_RightArrow, input.IsKeyPressed(GLFW_KEY_RIGHT));
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
        m_pIO->AddKeyEvent(ImGuiKey_GamepadStart, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_START));

        m_pIO->AddKeyEvent(ImGuiKey_GamepadDpadLeft, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_LEFT));
        m_pIO->AddKeyEvent(ImGuiKey_GamepadDpadRight, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT));
        m_pIO->AddKeyEvent(ImGuiKey_GamepadDpadUp, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_UP));
        m_pIO->AddKeyEvent(ImGuiKey_GamepadDpadDown, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_DPAD_DOWN));

        m_pIO->AddKeyEvent(ImGuiKey_GamepadFaceDown, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_A));
        m_pIO->AddKeyEvent(ImGuiKey_GamepadFaceRight, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_B));
        m_pIO->AddKeyEvent(ImGuiKey_GamepadFaceLeft, input.IsGamepadButtonPressed(jid, GLFW_GAMEPAD_BUTTON_X));
    }
}

void UI::Update()
{
    m_pScene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
    UpdateImGuiKeyboardInputs();
    UpdateImGuiGamepadInputs();

#ifdef _DEBUG
    Input* input = SERVICE_LOCATOR.GetInput();
    if (input->IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && input->IsKeyJustPressed(GLFW_KEY_S))
    {
        m_eventListener.BroadcastEvent(new Event("Save Scene"));
    }
#endif
}

void UI::Render()
{
    m_Hovering = false;
    m_Interacting = false;
    glClear(GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderGameUI();
    RenderEngineUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::RenderGameUI()
{
    auto* uiNode = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetUINode();
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(m_pWindowProps->Width), static_cast<float>(m_pWindowProps->Height)));
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Ensure it stays at the top-left corner

    ImGui::GetStyle().FrameRounding = 8.0f;

    if (!uiNode->GetChildren().empty())
    {
        if (ImGui::Begin("GameUI", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            (m_toggleDebug ? ImGuiWindowFlags_NoInputs : 0)))
        {
            for (auto& child : uiNode->GetChildren())
            {
                RenderGameUIElement(child);
            }

            ImGui::End();
        }
    }

    ImGui::GetStyle().FrameRounding = 0.0f;
}

void UI::RenderGameUIElement(Node* node, glm::mat4 transform)
{
    auto* uiComp = node->GetComponent<UIComponent>();
    if (!uiComp)
        uiComp = node->GetComponent<UITextComponent>();
    uiComp->Update();
	
    auto* uiElement = uiComp->m_pUIElement.get();
    const glm::vec3& fontColor = uiElement->color;
    ImFont* font = uiElement->fontName[0] == '\0' ? ImGui::GetIO().Fonts->Fonts[0] : m_fontMap[uiElement->fontName];

    transform = transform * node->GetTransform()->GetTranslationMatrix();
    glm::vec2 scale = uiComp->GetScale();
    glm::vec2 pos = transform[3];

    pos.x = pos.x * m_pWindowProps->Width - (scale.x / 2.0f);
    pos.y = pos.y * m_pWindowProps->Height - (scale.y / 2.0f);

    glm::vec2 posScale[2] = { pos, scale };

    if (uiElement->type != IMGUI_ELEMENT_TYPE::TEXT) 
    {
        float fontScale = font == ImGui::GetIO().Fonts->Fonts[0] ? scale.y / 45.0f : scale.y / 95.0f;
        font->Scale = fontScale;
        ImGui::PushFont(font);
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255 * fontColor.r, 255 * fontColor.g, 255 * fontColor.b, 255));
    }

    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
    m_pRenderFunctions[uiComp->m_pUIElement->type](uiComp->m_pUIElement.get(), &posScale);

    if (uiElement->type != IMGUI_ELEMENT_TYPE::TEXT)
    {
        font->Scale = 1.0f;
        ImGui::PopStyleColor();
        ImGui::PopFont();
    }

    for (auto& child : node->GetChildren())
        RenderGameUIElement(child, transform);
}

void UI::RenderEngineUI()
{
    if (!m_toggleDebug)
        return;

    if (ImGui::BeginMainMenuBar()) {
        ImVec2 mainMenuBarSize = ImGui::GetWindowSize();
        m_mainMenuBarHeight = mainMenuBarSize.y;

        // Check if the mouse is inside or focused on the menu bar
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;

        for (auto& element : m_engineUIElements)
            m_pRenderFunctions[element->type](element.get(), nullptr);

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
        if (m_pWindowProps->Width >= 790)
        {
            ImGui::SetCursorPosX(static_cast<float>(m_pWindowProps->Width - 90));
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        }

        ImGui::EndMainMenuBar();
    }

    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Node Tree"))
        RenderNodesWindow();

    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Console"))
        RenderConsoleWindow();

    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Node Inspector"))
        RenderInspectorWindow();
}

UI& UI::GetInstance()
{
    static UI* instance = nullptr;
    if (!instance)
        instance = new UI();
    return *instance;
}

int UI::GetToggleState(const char* buttonName, bool isEngineUI, Node* parent) const
{
    if (isEngineUI)
    {
        for (auto& element : m_engineUIElements)
        {
            if (strcmp(element->name.c_str(), buttonName) == 0)
            {
                if (element->type == IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE)
                {
                    return static_cast<bool>(element->selected);
                }
                else
                {
                    std::cerr << "<UI ERROR>\nGetToggleState element type is not a BUTTON_TOGGLE.\n\n";
                    return -1;
                }
            }
        }
    }
    else
    {
        for (auto& element : SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetUINode()->GetChildren())
        {
            auto* uiComp = element->GetComponent<UIComponent>();
            if (strcmp(uiComp->m_pUIElement->name.c_str(), buttonName) == 0)
            {
                if (static_cast<IMGUI_ELEMENT_TYPE>(uiComp->GetElementType()) == IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE)
                {
                    return static_cast<bool>(uiComp->m_pUIElement->selected);
                }
                else
                {
                    std::cerr << "<UI ERROR>\nGetToggleState element type is not a BUTTON_TOGGLE.\n\n";
                    return -1;
                }
            }
            if (!element->GetChildren().empty())
            {
                int res = GetToggleState(buttonName, false, element);
                if (res != -1.0)
                {
                    return res;
                }
            }
        }
    }
    return -1;
}

int UI::GetToggleState(const char* elementName, const char* subElementName) const
{
    for (auto& element : m_engineUIElements)
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

float UI::GetSliderValue(const char* sliderName, bool isEngineUI, Node* parent) const
{
    if (isEngineUI)
    {
        for (auto& element : m_engineUIElements)
        {
            if (strcmp(element->name.c_str(), sliderName) == 0)
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
    }
    else
    {
        for (auto& element : (parent ? parent->GetChildren() : SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetUINode()->GetChildren()))
        {
            auto* uiComp = element->GetComponent<UIComponent>();
            if (uiComp && strcmp(uiComp->m_pUIElement->name.c_str(), sliderName) == 0)
            {
                if (static_cast<IMGUI_ELEMENT_TYPE>(uiComp->GetElementType()) == IMGUI_ELEMENT_TYPE::SLIDER)
                {
                    return static_cast<IMGUI_SLIDER*>(uiComp->m_pUIElement.get())->value;
                }
                else
                {
                    std::cerr << "<UI ERROR>\nGetSliderValue element must be an IMGUI_SLIDER.\n\n";
                    return -1.0f;
                }
            }
            if (!element->GetChildren().empty())
            {
                float res = GetSliderValue(sliderName, false, element);
                if (res != -1.0f)
                {
                    return res;
                }
            }
        }
    }

    //std::cerr << "<UI ERROR> slider, " << sliderName << ", not found!\n\n";
    return -1.0f;
}

float UI::GetSliderValue(const char* elementName, const char* subElementName) const
{
    for (auto& element : m_engineUIElements)
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

const std::string& UI::GetSelectedSceneName() const
{
    return m_sceneSelection.items[m_sceneSelection.selected]->name;
}

void UI::SetScenes()
{
    for (auto& name : SERVICE_LOCATOR.GetSceneManager()->GetSceneNames())
        m_sceneSelection.items.push_back(std::unique_ptr<IMGUI_ELEMENT>(new IMGUI_ELEMENT(name.c_str())));
    m_sceneSelection.selected = SERVICE_LOCATOR.GetSceneManager()->GetCurrentSceneIndex();
}

void UI::ToggleDebug()
{
    m_toggleDebug = !m_toggleDebug;
    m_toggleDebug ?
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UI::RenderButtonToggle(IMGUI_ELEMENT* element, glm::vec2(*gameUIPosScale)[2])
{
    bool isGameUIButton = gameUIPosScale ? true : false;
    IMGUI_BUTTON* button = static_cast<IMGUI_BUTTON*>(element);
    bool state = static_cast<bool>(button->selected);

    if (isGameUIButton)
        ImGui::PushStyleColor(ImGuiCol_Button, state ?
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] :
            ImGui::GetStyle().Colors[ImGuiCol_Button]);

    if ((gameUIPosScale ? ImGui::Button(element->name.c_str(), ImVec2((*gameUIPosScale)[1].x, (*gameUIPosScale)[1].y)) :
        ImGui::MenuItem(element->name.c_str(), NULL, &state))) {
#ifdef _DEBUG
        m_eventListener.BroadcastEvent(new BoolEvent(element->name, state));
#else
        state = !state;
        m_eventListener.BroadcastEvent(new BoolEvent(element->savePath, state));
#endif
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

    if (isGameUIButton)
        ImGui::PopStyleColor();
}

// 150, 60
void UI::RenderButtonAction(IMGUI_ELEMENT* element, glm::vec2(*gameUIPosScale)[2])
{
    // Display each menu item as a toggleable button
    if ((gameUIPosScale ? ImGui::Button(element->name.c_str(), ImVec2((*gameUIPosScale)[1].x, (*gameUIPosScale)[1].y)) :
        ImGui::MenuItem(element->name.c_str())))
    {
        std::cout << "Button Action Event: " << element->savePath << std::endl;
        // Send event
        m_eventListener.BroadcastEvent(new Event(element->savePath));
    }
}

void UI::RenderToggleMenu(IMGUI_ELEMENT* element, glm::vec2(*gameUIPosScale)[2])
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
                m_eventListener.BroadcastEvent(new BoolEvent(toggleItem->name, state));
            }
            toggleItem->selected = (unsigned short)state;
            // Send event
        }
        ImGui::EndMenu();
    }
}

void UI::RenderSelectionMenu(IMGUI_ELEMENT* element, glm::vec2(*gameUIScale)[2])
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

void UI::RenderActionMenu(IMGUI_ELEMENT* element, glm::vec2(*gameUIScale)[2])
{
    IMGUI_DROPDOWN_MENU* ddMenu = static_cast<IMGUI_DROPDOWN_MENU*>(element);
    if (ImGui::BeginMenu(ddMenu->name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of toggleable buttons
        for (int i = 0; i < ddMenu->items.size(); ++i) {
            IMGUI_ELEMENT* item = ddMenu->items[i].get();
            // Display each menu item as a toggleable button
            bool state = item->selected == 0 ? false : true;
            if (ImGui::MenuItem(item->name.c_str(), NULL, &state)) {
                if (state) {
                    // Send event HERE
                    m_eventListener.BroadcastEvent(new Event(item->name));

                    state = false;
                }
            }
        }
        ImGui::EndMenu();
    }
}

// Renders a slider element
void UI::RenderSlider(IMGUI_ELEMENT* element, glm::vec2(*gameUIPosScale)[2])
{
    IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(element);
    //ImFont* font = slider->fontName[0] == '\0' ? ImGui::GetIO().Fonts->Fonts[0] : m_fontMap[slider->fontName];
    if (gameUIPosScale)
        ImGui::SetNextItemWidth((*gameUIPosScale)[1].x);

    //ImGui::PushFont(font);

    if (ImGui::SliderFloat(slider->name.c_str(), &slider->value, slider->min, slider->max))
    {
#ifdef _DEBUG
        m_eventListener.BroadcastEvent(new FloatEvent(slider->name, slider->value));
#else
        m_eventListener.BroadcastEvent(new FloatEvent(slider->savePath, slider->value));
#endif
    }
	//ImGui::PopFont();
}

// Renders a slider menu
void UI::RenderSliderMenu(IMGUI_ELEMENT* element, glm::vec2(*gameUIScale)[2])
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

void UI::RenderText(IMGUI_ELEMENT* element, glm::vec2(*gameUIPosScale)[2])
{
    auto& FONT_MAP = m_fontMap;
    IMGUI_TEXT_ELEMENT* textElement = static_cast<IMGUI_TEXT_ELEMENT*>(element);
    glm::vec3& color = textElement->color;
    if (element->selected == 0) { return; }

    ImFont* font = textElement->fontName[0] == '\0' ? ImGui::GetIO().Fonts->Fonts[0] : m_fontMap[textElement->fontName];
    float fontScale = (*gameUIPosScale)[1].x;
    float fontSize = font->FontSize * fontScale;

    float textWidth = ImGui::CalcTextSize(element->savePath.c_str()).x * font->FontSize / 13.0f * font->Scale * fontScale;
    float textHeight = ImGui::CalcTextSize(element->savePath.c_str()).y * font->FontSize / 13.0f * font->Scale * fontScale;

    // Correct centering adjustment
    glm::vec2 textPos = (*gameUIPosScale)[0]; // Copy original position

    if (!textElement->anchorLeft) 
    {
        textPos.x -= textWidth / 2.0f; // Center horizontally
    }
    textPos.y -= textHeight / 2.0f; // Center vertically

    // Draw text using properly scaled font size
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    draw_list->AddText(font, font->FontSize * fontScale * font->Scale, ImVec2(textPos.x, textPos.y), IM_COL32(255 * color.r, 255 * color.g, 255 * color.b, 255), element->savePath.c_str());
}

// Renders scene selection dropdown menu
void UI::RenderSceneSelection()
{
    if (ImGui::BeginMenu(m_sceneSelection.name.c_str())) {
        // Check if the mouse is inside or focused on the window
        if (ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
            m_Hovering = true;
        // Create a list of toggleable menu items, but only one can be selected
        for (int i = 0; i < m_sceneSelection.items.size(); ++i) {
            bool is_selected = (m_sceneSelection.selected == i);
            if (ImGui::MenuItem(m_sceneSelection.items[i]->name.c_str(), NULL, &is_selected)) {
                m_sceneSelection.selected = i;
                // Perform an action when the item is selected
                ImGui::Text("%s is now SELECTED", m_sceneSelection.items[i]->name.c_str());
                m_pSelectedNode = nullptr;
            }
            else
                m_sceneSelection.items[i]->selected = 0;
        }
        ImGui::EndMenu();
    }
}

// Renders debug console window
void UI::RenderConsoleWindow()
{
    static float consoleTextSize = 16.0f;

    // Set the position and size for the bottom window
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(m_pWindowProps->Width), m_bottomWindowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, m_pWindowProps->Height - m_bottomWindowHeight), ImGuiCond_Always);

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
    static float prevRightWindowHeight = static_cast<float>(m_pWindowProps->Height); // Since height is fixed initially

    // Set the position to the left side of the window
    ImGui::SetNextWindowPos(ImVec2(m_pWindowProps->Width - m_rightWindowWidth, m_mainMenuBarHeight), ImGuiCond_Always);
    // Set the size so that it spans the full height and has adjustable width
    ImGui::SetNextWindowSize(ImVec2(m_rightWindowWidth, static_cast<float>(
        SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Console") ?
        m_pWindowProps->Height - m_bottomWindowHeight - m_mainMenuBarHeight : m_pWindowProps->Height)), ImGuiCond_Always);

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
    if (m_pSelectedNode)
    {
        Node* parent = m_pSelectedNode->GetParent();
        if (parent && m_selectedNodeIsUINode)
            RenderUINodeComponent();
        else
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
    if (m_pSelectedNode->GetName() == "Game UI")
    {
        ImGui::Text("Name: Game UI");
        ImGui::NewLine();

        static int selectedUITypeIndex = 0;

        if (ImGui::TreeNodeEx(GameUITypes[selectedUITypeIndex].first.data(),
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
            for (int i = 0; i < 4; i++)
            {
                bool isSelected = (selectedUITypeIndex == i);
                if (ImGui::Selectable(GameUITypes[i].first.data(), isSelected))
                {
                    selectedUITypeIndex = GameUITypes[i].second;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::TreePop();
        }

        if (ImGui::Button("Add UI Element"))
        {
            Node* newUINode = new Node();
            UIComponent* uiComp = GameUITypes[selectedUITypeIndex].first == "Text" ?
                newUINode->AddComponent<UITextComponent>() : newUINode->AddComponent<UIComponent>();
            uiComp->CreateUIElement(GameUITypes[selectedUITypeIndex].second);
            Node* uiNode = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetUINode();
            uiNode->AddChild(newUINode);
            std::string newNodeName = GameUITypes[selectedUITypeIndex].first.data();
            newNodeName += "_"; newNodeName += std::to_string(newUINode->GetID());
            newUINode->SetName(newNodeName);

            if (static_cast<IMGUI_ELEMENT_TYPE>(uiComp->GetElementType()) != IMGUI_ELEMENT_TYPE::TEXT)
            {
                uiComp->SetScale(glm::vec2(150.0f, 60.0f));
            }
            else
            {
                uiComp->SetScale(glm::vec2(12.0f, 0.0f));
            }
        }

        return;
    }

    ImGui::Text("ID: %d", m_pSelectedNode->GetID());

    bool edited = false;
    std::string name(25, '\0');
    name.replace(0, m_pSelectedNode->GetName().size(), m_pSelectedNode->GetName());
    ImGui::Text("Name: ");
    ImGui::SameLine();
    if (ImGui::InputText("##name", &name[0], name.size()))
        edited = true;
    if (ImGui::IsItemActive())
    {
        m_Interacting = true;
    }
    if (edited && ImGui::IsItemDeactivatedAfterEdit())
    {
        m_pSelectedNode->SetName(name);
        edited = false;
    }

    ImGui::Text("\nChildren: %d", m_pSelectedNode->GetChildren().size());
    ImGui::Separator();

    auto& components = m_pSelectedNode->GetComponents();

    for (std::unordered_map<std::string, Component*>::iterator comp = components.begin(); comp != components.end(); comp++)
    {
        auto& getters = comp->second->GetGetters();
        auto& setters = comp->second->GetSetters();

        if (ImGui::TreeNodeEx(comp->first.c_str(),
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed))
        {
            if (comp->first == ComponentNames::TRANSFORM_COMPONENT)
            {
                RenderTransformComponent();
            }
            else if (comp->first == ComponentNames::RENDER_COMPONENT)
            {
                RenderRenderComponent(static_cast<RenderComponent*>(comp->second));
            }
            else if (comp->first == ComponentNames::COLLISION_COMPONENT)
            {
                RenderCollisionComponent(static_cast<CollisionComponent*>(comp->second));
            }
            else if (comp->first == ComponentNames::SCRIPT_COMPONENT)
            {
                RenderScriptComponent(static_cast<ScriptComponent*>(comp->second));
                if (comp == components.end())
                    break;
            }
            else
            {
                for (auto getter = getters.begin(); getter != getters.end(); getter++)
                    ReadAndSetComponentData(getter, setters.find(getter->first)->second);
            }

            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            RenderDeleteButton([](std::unordered_map<std::string, Component*>::iterator comp)
                {
                    m_eventListener.BroadcastEvent(new ComponentItrEvent("Delete Component", comp));
                }, comp);

            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNodeEx("Add Component",
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed))
    {
        ImVec2 itemSize = ImGui::CalcTextSize(ComponentNames::names[m_selectedComponentIndex].data());
        ImGui::SetNextItemWidth(itemSize.x + 30.0f);

        if (ImGui::BeginCombo("##Select Component", ComponentNames::names[m_selectedComponentIndex].data()))
        {
            for (int i = 0; i < ComponentNames::Index::IndexEnd; i++)
            {
                if (components.contains(ComponentNames::names[i].data()))
                    continue;

                bool isSelected = (m_selectedComponentIndex == i);
                if (ImGui::Selectable(ComponentNames::names[i].data(), isSelected))
                {
                    m_selectedComponentIndex = static_cast<ComponentNames::Index>(i);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        if (ImGui::Button("ADD"))
        {
            auto& components = m_pSelectedNode->GetComponents();
            if (m_selectedComponentIndex != ComponentNames::Index::IndexEnd &&
                components.find(ComponentNames::names[m_selectedComponentIndex].data()) == components.end())
            {
                ComponentDictionary::ComponentTypes type = ComponentDictionary::componentStore.at(ComponentNames::names[m_selectedComponentIndex]);
                std::visit([&](auto value)
                    {
                        using ComponentType = std::remove_pointer_t<std::decay_t<decltype(value)>>;
                        try
                        {
                            m_pSelectedNode->AddComponent<ComponentType>(true);
                            m_selectedComponentIndex = ComponentNames::Index::IndexEnd;
                        }
                        catch (const std::bad_variant_access& e)
                        {
                            std::cout << "Component likely not properly defined in GameObjectSystemComponentConstants.cpp" << std::endl;
                        }
                    }, type);
            }
        }
        ImGui::TreePop();
    }

    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    if (ImGui::Button("Duplicate"))
    {
        Node* clone = DuplicateNode(m_pSelectedNode);
        if (m_pSelectedNode->GetParent())
            m_pSelectedNode->GetParent()->AddChild(clone);
        clone->SetName(m_pSelectedNode->GetName() + "_" + std::to_string(clone->GetID()));
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    RenderDeleteButton([](Node** selectedNode)
        {
            Node* parent = (*selectedNode)->GetParent();
            if (parent)
                parent->RemoveChild(*selectedNode);
            else
                SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->RemoveNode(*selectedNode);
            (*selectedNode)->Destroy();
            *selectedNode = nullptr;
        }, &m_pSelectedNode);
}

void UI::ReadAndSetComponentData(auto& getter, auto& setter)
{
    std::any value = getter->second();
    std::string nameTag = "##";
    nameTag.append(getter->first);
    if (value.type() == typeid(float))
    {
        float val = std::any_cast<float>(value);
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat(nameTag.c_str(), &val, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(val);
    }
    else if (value.type() == typeid(double))
    {
        float val = static_cast<float>(std::any_cast<double>(value));
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat(nameTag.c_str(), &val, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(static_cast<double>(val));
    }
    else if (value.type() == typeid(glm::vec2))
    {
        glm::vec2 val = std::any_cast<glm::vec2>(value);
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat2(nameTag.c_str(), &val.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(val);
    }
    else if (value.type() == typeid(glm::vec3))
    {
        glm::vec3 val = std::any_cast<glm::vec3>(value);
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat3(nameTag.c_str(), &val.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(val);
    }
    else if (value.type() == typeid(glm::dvec3))
    {
        glm::vec3 val = std::any_cast<glm::dvec3>(value);
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat3(nameTag.c_str(), &val.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(static_cast<glm::dvec3>(val));
    }
    else if (value.type() == typeid(glm::vec4))
    {
        glm::vec4 val = std::any_cast<glm::vec4>(value);
        ImGui::Text(getter->first.c_str());
        ImGui::DragFloat4(nameTag.c_str(), &val.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        setter(val);
    }
    else if (value.type() == typeid(UV_TYPE))
    {
        UV_TYPE val = std::any_cast<UV_TYPE>(value);
        if (ImGui::BeginCombo("##Select UV_TYPE", UV_TYPE_STRINGS[val].data()))
        {
            for (int i = 0; i < UV_TYPE::UV_END; i++)
            {
                bool isSelected = (val == static_cast<UV_TYPE>(i));
                if (ImGui::Selectable(UV_TYPE_STRINGS[i].data(), isSelected))
                {
                    val = static_cast<UV_TYPE>(i);
                    setter(val);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    else if (value.type() == typeid(std::string))
    {
        std::string val = std::any_cast<std::string>(value);
        if (ImGui::TreeNodeEx(getter->first.c_str(),
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::Text("Value: ");
            ImGui::SameLine();
            ImGui::Text(val.c_str());
            ImGui::NewLine();

            ImVec2 itemSize = ImGui::CalcTextSize(stringTypes[m_stringTypeIndex].data());
            ImGui::Text("Type: ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(itemSize.x + 30.0f);
            if (ImGui::BeginCombo("##Select Type", stringTypes[m_stringTypeIndex].data()))
            {
                for (int i = 0; i < StringTypes::End; i++)
                {
                    bool isSelected = (m_stringTypeIndex == i);
                    if (ImGui::Selectable(stringTypes[i].data(), isSelected))
                    {
                        m_stringTypeIndex = i;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::Separator();

            if (m_stringTypeIndex == StringTypes::Default)
            {
                std::string input(40, '\0');
                input.replace(0, val.size(), val);
                ImGui::InputText(nameTag.c_str(), &input[0], input.size());
                if (ImGui::IsItemActive())
                {
                    m_Interacting = true;
                }
                val = input;
                if (ImGui::Button("Save"))
                {
                    val.erase(std::remove(val.begin(), val.end(), '\0'), val.end());
                    setter(val);
                }
            }
            else if (m_stringTypeIndex == StringTypes::FilePath)
            {
                static std::string startPath = defaultDirectory.data();
                static std::string file = defaultDirectory.data();
                ImGui::Text(file.c_str());
                if (ImGui::TreeNodeEx("Select File",
                    ImGuiTreeNodeFlags_OpenOnArrow))
                {
                    for (const auto& entry : std::filesystem::directory_iterator(startPath))
                    {
                        if (entry.is_directory())
                        {
                            if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
                            {
                                startPath = entry.path().string();
                            }
                        }
                        else
                        {
                            bool isSelected = (file.compare(entry.path().string()) == 0);

                            if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
                            {
                                file = entry.path().string();
                                std::cout << "Selected file: " << file << std::endl;
                            }

                            if (isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    if (startPath != defaultDirectory)
                    {
                        if (ImGui::Button("BACK"))
                        {
                            startPath = defaultDirectory;
                        }
                    }
                    ImGui::TreePop();
                }
                if (ImGui::Button("Save"))
                {
                    setter(file);
                }
            }
        }
    }
}

void UI::RenderUINodeComponent()
{
    ImGui::Text("ID: %d", m_pSelectedNode->GetID());

    bool edited = false;
    std::string name = m_pSelectedNode->GetName();
    name.resize(name.size() + 10); // Ensure enough space for editing

    ImGui::Text("Name: ");
    ImGui::SameLine();
    if (ImGui::InputText("##name", &name[0], name.capacity()))
        edited = true;
    if (ImGui::IsItemActive())
    {
        m_Interacting = true;
    }
    if (edited && ImGui::IsItemDeactivatedAfterEdit())
    {
        name.erase(std::find(name.begin(), name.end(), '\0'), name.end());
        m_pSelectedNode->SetName(name);
        edited = false;
    }

    UIComponent* uiComp = m_pSelectedNode->GetComponent<UIComponent>();
    if (!uiComp)
        uiComp = m_pSelectedNode->GetComponent<UITextComponent>();

    IMGUI_ELEMENT_TYPE elementType = static_cast<IMGUI_ELEMENT_TYPE>(uiComp->GetElementType());

    // Element Type
    if (elementType != IMGUI_ELEMENT_TYPE::TEXT)
    {
        bool editedEName = false;
        std::string eventName = uiComp->GetSaveFilePath();
        eventName.resize(eventName.size() + 10); // Ensure enough space for editing

        ImGui::Text("Event: ");
        ImGui::SameLine();
        if (ImGui::InputText("##event", &eventName[0], eventName.capacity()))
            editedEName = true;
        if (ImGui::IsItemActive())
        {
            m_Interacting = true;
        }
        if (editedEName && ImGui::IsItemDeactivatedAfterEdit())
        {
            eventName.erase(std::find(eventName.begin(), eventName.end(), '\0'), eventName.end());
            uiComp->SetSaveFilePath(eventName);
            editedEName = false;
        }

        ImGui::Separator();

        static int selectedUITypeIndex = 0;
        if (ImGui::TreeNodeEx(GameUITypes[selectedUITypeIndex].first.data(),
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
            for (int i = 0; i < 3; i++)
            {
                bool isSelected = (selectedUITypeIndex == i);
                if (ImGui::Selectable(GameUITypes[i].first.data(), isSelected))
                {
                    selectedUITypeIndex = GameUITypes[i].second;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::TreePop();
        }
        if (selectedUITypeIndex != uiComp->GetElementType())
        {
            if (ImGui::Button("Change Type"))
            {
                uiComp->CreateUIElement(static_cast<IMGUI_ELEMENT_TYPE>(selectedUITypeIndex));
                elementType = static_cast<IMGUI_ELEMENT_TYPE>(uiComp->GetElementType());
            }
        }
        ImGui::NewLine();
    }
    else { ImGui::Separator(); }

    // Position
    glm::vec2 val = uiComp->GetPosition();
    ImGui::Text("Position");
    ImGui::DragFloat2("##UI pos", &val.x, 0.0001f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.3f");
    uiComp->SetPosition(val);

    // Scale
    val = uiComp->GetScale();
    ImGui::Text("Scale");
    ImGui::DragFloat2("##UI scale", &val.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    uiComp->SetScale(val);

    if (elementType == IMGUI_ELEMENT_TYPE::SLIDER)
    {
        IMGUI_SLIDER* slider = static_cast<IMGUI_SLIDER*>(uiComp->m_pUIElement.get());
        // min
        ImGui::Text("Minimum");
        ImGui::DragFloat("##slider min", &slider->min, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        // max
        ImGui::Text("Maximum");
        ImGui::DragFloat("##slider max", &slider->max, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
        // value
        ImGui::Text("Value");
        ImGui::DragFloat("##slider value", &slider->value, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    }
    else if (elementType == IMGUI_ELEMENT_TYPE::BUTTON_TOGGLE)
    {
        bool selected = uiComp->m_pUIElement->selected == 0 ? false : true;

        ImGui::PushStyleColor(ImGuiCol_Button, selected ?
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] :
            ImGui::GetStyle().Colors[ImGuiCol_Button]);

        if (ImGui::Button("Toggle State"))
        {
            uiComp->m_pUIElement->selected = selected ? 0 : 1;
        }

        ImGui::PopStyleColor();
    }

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Set font
    std::string fontName = "NONE";
    if (uiComp->GetFont()[0] != '\0') { fontName = uiComp->GetFont(); }
    ImGui::Text("Font: ");
    ImGui::SameLine();
    ImGui::Text(fontName.c_str());

    if (ImGui::BeginCombo("##Select Font", fontName.c_str()))
    {
        for (const auto& entry : std::filesystem::directory_iterator(fontDirectory))
        {
            bool isSelected = (entry.path().filename().string() == fontName);

            if (isSelected)
            {
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange highligh
            }

            if (ImGui::Selectable(entry.path().filename().string().c_str()))
            {
                uiComp->SetFont(entry.path().filename().string().c_str());
            }

            if (isSelected)
                ImGui::PopStyleColor();
        }
        if (ImGui::Selectable("NONE"))
        {
            uiComp->SetFont("");
        }
        ImGui::EndCombo();
    }
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Set font color
    glm::vec3 color = uiComp->GetColor();
    ImGui::Text("Font Color");
    ImGui::DragFloat3("##UI text color", &color.x, 0.01f, 0.0f, 1.0f, "%.2f");
    uiComp->SetColor(color);
    ImGui::Dummy(ImVec2(0.0f, 2.0f));
	
    if (elementType == IMGUI_ELEMENT_TYPE::TEXT)
    {
        ImGui::Dummy(ImVec2(0.0f, 2.0f));
        UITextComponent* textComp = static_cast<UITextComponent*>(uiComp);

        bool editedText = false;
        const std::string ConstText = textComp->GetText();
        std::string text(100, '\0');
        text.replace(0, ConstText.size(), ConstText);
        ImGui::Text("Text: ");
        ImGui::SameLine();
        if (ImGui::InputText("##text", &text[0], text.size()))
            editedText = true;
        if (ImGui::IsItemActive())
        {
            m_Interacting = true;
        }
        if (editedText && ImGui::IsItemDeactivatedAfterEdit())
        {
            textComp->SetText(text.c_str());
            editedText = false;
        }

        ImGui::Dummy(ImVec2(0.0f, 2.0f));

        bool state = textComp->IsActive();
        ImGui::PushStyleColor(ImGuiCol_Button, state ?
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] :
            ImGui::GetStyle().Colors[ImGuiCol_Button]);
        if (ImGui::Button("IsActive"))
        {
            state = !state;
            textComp->SetActive(state);
        }
        ImGui::PopStyleColor();

        state = textComp->GetAnchorLeft();
        ImGui::PushStyleColor(ImGuiCol_Button, state ?
            ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] :
            ImGui::GetStyle().Colors[ImGuiCol_Button]);
        if (ImGui::Button("Anchor Left"))
        {
            state = !state;
            textComp->SetAnchorLeft(state);
        }
        ImGui::PopStyleColor();
    }
    ImGui::Separator();

    // Duplicate Button
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    if (ImGui::Button("Duplicate"))
    {
        Node* clone = DuplicateNode(m_pSelectedNode);
        if (m_pSelectedNode->GetParent())
            m_pSelectedNode->GetParent()->AddChild(clone);
        clone->SetName(m_pSelectedNode->GetName() + "_" + std::to_string(clone->GetID()));
    }

    // Delete Button
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    RenderDeleteButton([](Node** selectedNode)
        {
            Node* parent = (*selectedNode)->GetParent();
            if (parent)
                parent->RemoveChild(*selectedNode);
            else
                SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->RemoveNode(*selectedNode);
            (*selectedNode)->Destroy();
            *selectedNode = nullptr;
        }, &m_pSelectedNode);
}

void UI::RenderTransformComponent()
{
    ImGui::Text("Position");
    glm::vec3 position = m_pSelectedNode->GetTransform()->GetPosition();
    ImGui::DragFloat3("##Position", &position.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    m_pSelectedNode->GetTransform()->SetPosition(position);
    ImGui::Text("Rotation");
    glm::vec3 rotation = m_pSelectedNode->GetTransform()->GetRotation();
    ImGui::DragFloat3("##Rotation", &rotation.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    m_pSelectedNode->GetTransform()->SetRotation(rotation);
    ImGui::Text("Scale");
    glm::vec3 scale = m_pSelectedNode->GetTransform()->GetScale();
    ImGui::DragFloat3("##Scale", &scale.x, 0.1f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), "%.2f");
    m_pSelectedNode->GetTransform()->SetScale(scale);
}

void UI::RenderRenderComponent(RenderComponent* renderComp)
{
    // Set material properties here
    auto* rM = SERVICE_LOCATOR.GetResourceManager();
    if (ImGui::TreeNodeEx("Material",
        ImGuiTreeNodeFlags_OpenOnArrow))
    {
        std::string currMat = rM->GetMaterialName(renderComp->GetMaterial());
        if (ImGui::BeginCombo("##Change", currMat.c_str()))
        {
            for (auto& mName : m_materialNames)
            {
                bool isSelected = (currMat == mName);
                if (ImGui::Selectable(mName.c_str(), isSelected))
                {
                    currMat = mName;
                    renderComp->SetMaterial(rM->GetMaterial(currMat));
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if (ImGui::TreeNodeEx("New",
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
            ImGui::Text("New Name");
            ImGui::InputText("##NewName", &matName[0], matName.size());
            if (ImGui::IsItemActive())
            {
                m_Interacting = true;
            }
            if (ImGui::Button("Create"))
            {
                auto& materials = rM->GetMaterials();
                if (!materials.contains(matName))
                {
                    Material* m = new Material();
                    std::string newName = matName;
                    newName.erase(std::remove(newName.begin(), newName.end(), '\0'), newName.end());
                    rM->AddMaterial(newName, m);
                    m_materialNames.push_back(newName);
                    renderComp->SetMaterial(m);
                }
            }
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::TreePop();
        }

        if (ImGui::Button("Save"))
        {
            std::string newName = currMat;
            newName.erase(std::remove(newName.begin(), newName.end(), '\0'), newName.end());
            rM->ExportMaterial(newName);
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNodeEx("Texture",
        ImGuiTreeNodeFlags_OpenOnArrow))
    {
        std::string diffuse = rM->GetTextureName(renderComp->GetMaterial()->GetTextureDiffuse());
        std::string specular = rM->GetTextureName(renderComp->GetMaterial()->GetTextureSpecular());
        ImGui::Text("Diffuse");
        if (ImGui::BeginCombo("##ChangeDiffuse", diffuse.c_str()))
        {
            for (auto& dName : m_textureNames)
            {
                bool isSelected = (diffuse == dName);
                if (ImGui::Selectable(dName.c_str(), isSelected))
                {
                    diffuse = dName;
                    renderComp->GetMaterial()->SetTextureDiffuse(rM->GetTexture(diffuse));
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();

            }
            ImGui::EndCombo();
        }
        if (ImGui::IsItemActive())
        {
            m_Interacting = true;
        }
        ImGui::Text("Specular");
        if (ImGui::BeginCombo("##ChangeSpecular", specular.c_str()))
        {
            for (auto& sName : m_textureNames)
            {
                bool isSelected = (specular == sName);
                if (ImGui::Selectable(sName.c_str(), isSelected))
                {
                    specular = sName;
                    renderComp->GetMaterial()->SetTextureSpecular(rM->GetTexture(specular));
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::TreePop();
    }
    if (ImGui::IsItemActive())
    {
        m_Interacting = true;
    }
    ImGui::NewLine();

    ImGui::Text("Texture Tiling");

    ImGui::Text("Position");
    glm::vec2 tilingPos = renderComp->GetTexTilingPos();
    ImGui::DragFloat2("##Position", &tilingPos.x, 0.001f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
    renderComp->SetTexTilingPos(tilingPos);

    ImGui::Text("Scale");
    glm::vec2 tilingScale = renderComp->GetTexTilingScale();
    ImGui::DragFloat2("##Scale", &tilingScale.x, 0.001f, std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
    renderComp->SetTexTilingScale(tilingScale);

    ImGui::Text("Color (RGB)");
    glm::vec3 color = renderComp->GetMaterial()->GetColor();
    color *= 20.0f;
	color = glm::clamp(color, 0.0f, 1.0f);
    ImGui::DragFloat3("##Color", &color.x, 0.001f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
	color /= 20.0f;
    renderComp->GetMaterial()->SetColor(color);

    ImGui::Text("\nShininess");
    float shininess = renderComp->GetMaterial()->GetShininess();
    ImGui::DragFloat("##s", &shininess, 0.001f, 0.0f, std::numeric_limits<float>::max(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
    renderComp->GetMaterial()->SetShininess(shininess);

    ImGui::Text("\nAlpha");
    float alpha = renderComp->GetMaterial()->GetAlpha();
    ImGui::DragFloat("##a", &alpha, 0.001f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    renderComp->GetMaterial()->SetAlpha(alpha);

}

void UI::RenderCollisionComponent(CollisionComponent* colComp)
{
    int colMask = colComp->GetCollisionMask();
    int colLayer = colComp->GetCollisionLayer();
    ImGui::Text("Mask");
    ImGui::InputInt("##mask", &colMask, 0, 512);
    ImGui::Text("Layer");
    ImGui::InputInt("##layer", &colLayer, 0, 512);

    auto* shape = colComp->GetCollisionShape();
    std::string shapeType = shape->GetShapeType();

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    if (ImGui::TreeNodeEx("Shape",
        ImGuiTreeNodeFlags_OpenOnArrow))
    {
        ImGui::Text(shapeType.c_str());
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        if (ImGui::TreeNodeEx("Change Shape",
            ImGuiTreeNodeFlags_OpenOnArrow))
        {
            for (auto& shapeName : colShapes) {
                bool isSelected = (shapeName == shapeType);
                if (ImGui::Selectable(shapeName.data(), isSelected))
                {
                    if (shapeName == colShapes[0])
                    {
                        colComp->SetCollisionShape(new CollisionShape_Sphere());
                    }
                    else if (shapeName == colShapes[1])
                    {
                        colComp->SetCollisionShape(new CollisionShape_Cuboid());
                    }
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::TreePop();
        }
        ImGui::Dummy(ImVec2(0.0f, 2.0f));
        if (shapeType == CollisionShapeConstants::SPHERE)
        {
            auto* sphere = static_cast<CollisionShape_Sphere*>(shape);
            float diameter = static_cast<float>(sphere->GetScale().x);
            ImGui::Text("Diameter");
            ImGui::DragFloat("##d", &diameter, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.2f");
            sphere->SetScale(glm::dvec3(static_cast<double>(diameter)));
        }
        else if (shapeType == CollisionShapeConstants::CUBOID)
        {
            auto* cuboid = static_cast<CollisionShape_Cuboid*>(shape);
            glm::vec3 scale = cuboid->GetScale();
            ImGui::Text("Scale");
            ImGui::DragFloat3("##Cube Scale", &scale.x, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.2f");
            cuboid->SetScale(static_cast<glm::dvec3>(scale));
        }

        ImGui::TreePop();
    }
    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    colComp->SetCollisionMask(colMask);
    colComp->SetCollisionLayer(colLayer);
}

void UI::RenderScriptComponent(ScriptComponent* scriptComp)
{
    static std::string selectedFile = "";
    static std::string currentPath = scriptDirectory.data();

    std::string scriptName = scriptComp->GetScriptFilepath().c_str();
    ImGui::Text("Current script:\n%s", scriptName.c_str());
    ImGui::NewLine();
    ImGui::Text("Selected path:");
    ImGui::TextWrapped(selectedFile.c_str());
    ImGui::NewLine();
    if (ImGui::TreeNodeEx("Select File",
        ImGuiTreeNodeFlags_OpenOnArrow))
    {
        for (const auto& entry : std::filesystem::directory_iterator(currentPath))
        {
            if (entry.is_directory())
            {
                if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
                {
                    currentPath = entry.path().string();
                }
            }
            else
            {
                bool isSelected = (selectedFile.compare(entry.path().string()) == 0);

                if (isSelected)
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange highlight


                if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
                {
                    selectedFile = entry.path().string();
                    std::cout << "Selected file: " << selectedFile << std::endl;
                }

                if (isSelected)
                    ImGui::PopStyleColor();
            }
        }
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        if (currentPath != scriptDirectory)
        {
            if (ImGui::Button("BACK"))
            {
                currentPath = scriptDirectory;
            }
        }
        ImGui::TreePop();
    }
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    if (ImGui::Button("Saved Selected"))
    {
        if (selectedFile != "")
        {
            ScriptComponent* scriptComp = m_pSelectedNode->AddComponent<ScriptComponent>();
            scriptComp->LoadScript(selectedFile);
            selectedFile = "";
            currentPath = scriptDirectory;
        }
    }
}

// Renders node tree window
void UI::RenderNodesWindow()
{
    static float goWindowTextSize = 16.0f;
    static float prevLeftWindowWidth = m_leftWindowWidth;
    static float prevLeftWindowHeight = static_cast<float>(m_pWindowProps->Height); // Since height is fixed initially

    // Set the position to the left side of the window
    ImGui::SetNextWindowPos(ImVec2(0, m_mainMenuBarHeight), ImGuiCond_Always);
    // Set the size so that it spans the full height and has adjustable width
    ImGui::SetNextWindowSize(ImVec2(m_leftWindowWidth, static_cast<float>(
        SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Console") ?
        m_pWindowProps->Height - m_bottomWindowHeight - m_mainMenuBarHeight : m_pWindowProps->Height)), ImGuiCond_Always);

    // Create a resizable window without manual repositioning
    ImGui::Begin("Node Tree", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavInputs);

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
void UI::RenderNode(Node* node, int index, bool isUINode)
{
    ImGuiTreeNodeFlags nodeFlags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

    if (m_pSelectedNode == node)
    {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
        Input* input = SERVICE_LOCATOR.GetInput();
        if (m_pSelectedNode->GetName() != "Game UI")
        {
            if (input->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
            {
                if (input->IsKeyJustPressed(GLFW_KEY_UP))
                {
                    m_eventListener.BroadcastEvent(new IndexEvent("Node Move: Backward", index));
                }
                else if (input->IsKeyJustPressed(GLFW_KEY_DOWN))
                {
                    m_eventListener.BroadcastEvent(new IndexEvent("Node Move: Forward", index));
                }
                else if (input->IsKeyJustPressed(GLFW_KEY_LEFT))
                {
                    m_eventListener.BroadcastEvent(new IndexEvent("Node Move: Sibling", index));
                }
                else if (input->IsKeyJustPressed(GLFW_KEY_RIGHT))
                {
                    m_eventListener.BroadcastEvent(new IndexEvent("Node Move: Child", index));
                }
            }
        }
    }

    if (node->GetChildren().size() == 0)
        nodeFlags |= ImGuiTreeNodeFlags_Leaf;

    std::string nodeName = node->GetName();
    if (nodeName[0] == '\0')
        nodeName = std::to_string(static_cast<int>(node->GetID()));


    if (ImGui::TreeNodeEx(nodeName.c_str(), nodeFlags)) {
        // New node is selected
        if (ImGui::IsItemClicked())
        {
            ImGui::SetKeyboardFocusHere(-1); // Prevent arrow key focus override
            m_pSelectedNode = node;
            m_selectedComponentIndex = ComponentNames::Index::IndexEnd;
            m_selectedNodeIsUINode = isUINode;
            std::cout << m_pSelectedNode->GetName() << " selected" << std::endl;
        }
        auto& nodeChildren = node->GetChildren();
        for (int i = 0; i < nodeChildren.size(); i++)
        {
            RenderNode(nodeChildren[i], i, isUINode);
        }
        ImGui::TreePop(); // Ends the tree node
    }
}

// Renders the node tree by Rendering each node at the root level
void UI::RenderSceneGraph()
{
    RenderNode(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetUINode(), 0, true);
    auto& sceneNodes = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetNodes();
    for (int i = 0; i < sceneNodes.size(); i++)
    {
        RenderNode(sceneNodes[i], i);
    }
}

void UI::CreateSphere(Event* event)
{
    GameObject* sphere = new GameObject();
    std::string name = "Sphere_";

    sphere->AddComponent<TransformComponent>();
    sphere->AddComponent<RenderComponent>();
    sphere->AddComponent<CollisionComponent>();

    auto* renderComp = sphere->GetComponent<RenderComponent>();
    renderComp->SetGeometry("Sphere");
    renderComp->SetColor(1.0f, 1.0f, 1.0f);

    auto* transComp = sphere->GetComponent<TransformComponent>();
    transComp->SetPosition(glm::vec3(0.0f));

    auto* collisionComp = sphere->GetComponent<CollisionComponent>();
    collisionComp->SetCollisionShape(new CollisionShape_Sphere());

    SERVICE_LOCATOR.GetGameObjectManager()->AddGameObject(sphere);

    name += std::to_string(static_cast<int>(sphere->GetID()));
    sphere->SetName(name);

}

void UI::CreateCube(Event* event)
{
    GameObject* cube = new GameObject();
    std::string name = "Cube_";

    cube->AddComponent<TransformComponent>();
    cube->AddComponent<RenderComponent>();
    cube->AddComponent<CollisionComponent>();

    auto* renderComp = cube->GetComponent<RenderComponent>();
    renderComp->SetGeometry("Cube");
    renderComp->SetColor(1.0f, 1.0f, 1.0f);

    auto* transComp = cube->GetComponent<TransformComponent>();
    transComp->SetPosition(glm::vec3(0.0f));

    auto* collisionComp = cube->GetComponent<CollisionComponent>();
    collisionComp->SetCollisionShape(new CollisionShape_Cuboid());

    SERVICE_LOCATOR.GetGameObjectManager()->AddGameObject(cube);

    name += std::to_string(static_cast<int>(cube->GetID()));
    cube->SetName(name);
}

void UI::SaveScene(Event* event)
{
    auto* sceneManager = SERVICE_LOCATOR.GetSceneManager();
    sceneManager->ExportScene(sceneManager->GetCurrentScene());
}

void UI::DeleteComponent(Event* event)
{
    ComponentItrEvent* componentItrEvent = static_cast<ComponentItrEvent*>(event);
    componentItrEvent->itr->second->GetOwner()->RemoveComponent(componentItrEvent->itr);
}

void UI::NodeMoveBack(Event* event)
{
    int index = static_cast<IndexEvent*>(event)->index;
    if (m_pSelectedNode->GetParent())
    {
        auto& parentChildren = m_pSelectedNode->GetParent()->m_children;
        if (index > 0)
        {
            std::swap(parentChildren[index], parentChildren[index - 1]);
        }
    }
    else
    {
        auto& nodes = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->m_nodes;
        if (index > 0)
        {
            std::swap(nodes[index], nodes[index - 1]);
        }
    }
}

void UI::NodeMoveForward(Event* event)
{
    int index = static_cast<IndexEvent*>(event)->index;
    if (m_pSelectedNode->GetParent())
    {
        auto& parentChildren = m_pSelectedNode->GetParent()->m_children;
        if (index < parentChildren.size() - 1)
        {
            std::swap(parentChildren[index], parentChildren[index + 1]);
        }
    }
    else
    {
        auto& nodes = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->m_nodes;
        if (index < nodes.size() - 1)
        {
            std::swap(nodes[index], nodes[index + 1]);
        }
    }
}

void UI::NodeMoveChild(Event* event)
{
    auto& parentChildren = m_pSelectedNode->GetParent() ? m_pSelectedNode->GetParent()->m_children
        : SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->m_nodes;

    int index = static_cast<IndexEvent*>(event)->index;
    if (index > 0)
    {
        if (index != parentChildren.size() - 1)
        {
            std::swap(parentChildren[index], parentChildren[parentChildren.size() - 1]);
        }
        //glm::mat4 prevWorldPos = m_pSelectedNode->GetWorldTransform();

        parentChildren.pop_back();
        parentChildren[index - 1]->AddChild(m_pSelectedNode);

        /*glm::mat4 newParentInverse = glm::inverse(m_pSelectedNode->GetParent()->GetWorldTransform());
        glm::vec3 newPosition = glm::vec3((newParentInverse * prevWorldPos)[3]);
        m_pSelectedNode->GetTransform()->SetPosition(newPosition);*/
    }
}

void UI::NodeMoveSibling(Event* event)
{
    if (m_pSelectedNode->GetParent() && m_pSelectedNode->GetParent()->GetName() != "Game UI")
    {
        Scene* currScene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
        Node* oldParent = m_pSelectedNode->GetParent(), * newParent = oldParent->GetParent();
        std::vector<Node*>& oldParentChildren = oldParent->m_children;

        int index = static_cast<IndexEvent*>(event)->index;

        if (index != oldParentChildren.size() - 1)
        {
            std::swap(oldParentChildren[index], oldParentChildren[oldParentChildren.size() - 1]);
        }
        //glm::mat4 prevWorldPos = m_pSelectedNode->GetWorldTransform();

        oldParentChildren.pop_back();
        newParent ?
            newParent->AddChild(m_pSelectedNode) :
            currScene->AddNode(m_pSelectedNode);
        m_pSelectedNode->m_pParent = newParent;

        /*glm::mat4 newParentInverse = newParent ? glm::inverse(m_pSelectedNode->GetParent()->GetWorldTransform()) : glm::mat4(1.0f);
        glm::vec3 newPosition = glm::vec3((newParentInverse * prevWorldPos)[3]);
        m_pSelectedNode->GetTransform()->SetPosition(newPosition);*/
    }
}

Node* UI::DuplicateNode(Node* node)
{
    Node* clone = SERVICE_LOCATOR.GetGameObjectManager()->CloneNode(node);
    if (!node->GetParent())
    {
        SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->AddNode(clone);
    }

    UIComponent* cloneUiComp = clone->GetComponent<UIComponent>();
    if (!cloneUiComp)
        cloneUiComp = clone->GetComponent<UITextComponent>();
    if (cloneUiComp)
        cloneUiComp->CreateUIElement();

    for (Node* child : node->GetChildren())
    {
        Node* childClone = DuplicateNode(child);
        clone->AddChild(childClone);
        childClone->SetName(child->GetName() + "_" + std::to_string(clone->GetID()));
    }
    return clone;
}

UI::UI()
{
    m_sceneSelection.type = IMGUI_ELEMENT_TYPE::DROPDOWN_SELECTION;
}

// Upon destruction, save the state of each element that has a save path
UI::~UI()
{
    delete(&GetInstance());
}