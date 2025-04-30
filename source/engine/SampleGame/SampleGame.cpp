#include "../pch.h"
#include "SampleGame.h"
#include "../cameramanager/CameraComponent.h"
#include "../cameramanager/CameraManager.h"
#include "../scenemanager/SceneManager.h"
#include "../resourcemanager/ResourceFactory.h"
#include "../resourcemanager/ResourceManager.h"
#include "../objectmanager/GameObjectFactory.h"
#include "../objectmanager/GameObjectManager.h"
#include "../DeserializeJSON.h"
#include "../VectorCalculations.h"
#include "../AudioManager.h"
#include "TransformComponent.h"
#include "../Input.h"
#include "../physics/PhysicsComponent.h"
#include "../physics/CollisionComponent.h"
#include "RenderComponent.h"
#include "../ParticleComponent.h"
#include "../Debug.h"
#include "../GrappleComponent.h"
#include "../physics/PhysicsManager.h"
#include "../scripting/ScriptManager.h"
#include "../scripting/ScriptComponent.h"
#include "../Image.h"
#include "../ui/Ui.h"

std::string sample::m_levelTime = "";
int sample::m_levelFinishTimesIndex = 0;
bool sample::m_levelFinished = false;
int sample::m_points = 0;
int sample::level = -1;
EventListener* sample::m_pEventListener = nullptr;
bool sample::m_pBest = false;

void sample::Init()
{
    printf("Initializing sample scene!!\n");

    SERVICE_LOCATOR.GetResourceFactory()->CreateAllResources();

	// Add scenes
	SERVICE_LOCATOR.GetSceneManager()->AddScene("main_menu");
    SERVICE_LOCATOR.GetSceneManager()->AddScene("volume_menu");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("pause_menu");
    SERVICE_LOCATOR.GetSceneManager()->AddScene("controls_menu");
    SERVICE_LOCATOR.GetSceneManager()->AddScene("level_selection");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("completed_level");

    // Add levels to this indef for them to appear in debug level selector
	// As an (un)intended consequence, this causes the colliders of all scenes to load simultanously
    // This issue is solved on Release
#ifdef _DEBUG
    SERVICE_LOCATOR.GetSceneManager()->AddScene("tutorial_1");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("scene_jihoon");
    SERVICE_LOCATOR.GetSceneManager()->AddScene("level_1");
	SERVICE_LOCATOR.GetSceneManager()->AddScene("gracelevel");
#endif

    // Populate levels
    levels.push_back("tutorial_1");
    levels.push_back("gracelevel");
    levels.push_back("level_1");
    levels.push_back("scene_jihoon");
    level++;

	// Set current scene
	SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("main_menu");

	auto* currScene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
    // Pick background audio
    SERVICE_LOCATOR.GetAudioManager()->PlaySound("music\\StockMusic.net\\EF11-SR8-RonanDavey.mp3");

    std::cout << "sample::Init()" << std::endl;

    // update physics manager
    SERVICE_LOCATOR.GetPhysicsManager()->SetGravity(glm::dvec3(0, -9.8, 0));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef _DEBUG
    glfwSetInputMode(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
    m_pEventListener = new EventListener();
    // Events
    m_pEventListener->SubscribeToEvent("Volume Settings", std::bind(&sample::GoToVolumeSettings, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Back", std::bind(&sample::BackToPrevMenu, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Exit", std::bind(&sample::ExitGame, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Resume", std::bind(&sample::StartGame, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Pause", std::bind(&sample::GoToPauseMenu, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Controls", std::bind(&sample::ControlsMenu, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("EndLevel", std::bind(&sample::EndLevel, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Next Level", std::bind(&sample::NextLevel, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Restart", std::bind(&sample::RestartLevel, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Level Selection", std::bind(&sample::LevelSelection, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Start Tutorial", std::bind(&sample::StartTutorial, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Start Level 1", std::bind(&sample::StartLevelOne, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Start Level 2", std::bind(&sample::StartLevelTwo, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Start Level 3", std::bind(&sample::StartLevelThree, this, std::placeholders::_1));
    m_pEventListener->SubscribeToEvent("Credits", std::bind(&sample::Credits, this, std::placeholders::_1));
}

void sample::Update()
{
    SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Update();

    float dt = static_cast<float>(SERVICE_LOCATOR.GetTime()->GetDeltaTime());

    if (SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName() == levels[level] &&
        SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_ESCAPE))
    {
        m_pEventListener->BroadcastEvent(new Event("Pause"));
    }

    if (m_levelFinished) 
    {
        m_timer += dt;
        if (m_timer >= 4.0f) 
        {
            m_timer = 0.0f;
			m_levelFinished = false;
            m_pEventListener->BroadcastEvent(new Event("EndLevel"));
            //m_pEventListener->BroadcastEvent(new StringEvent("NextLevel", levels[level]));
        }
    }
}

void sample::PostUpdate()
{
    SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->PostUpdate();
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void sample::Render()
{
    SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->Render();

    auto scene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
	if (scene == nullptr)
		return;
    else if (std::find(levels.begin(), levels.end(), scene->GetName()) != levels.end())
    {
        DrawReticle();
        DrawGrapple();
    }
}

void sample::Shutdown()
{
    std::cout << "sample::Shutdown()" << std::endl;
    if (m_pEventListener)
    {
        delete m_pEventListener;
        m_pEventListener = nullptr;
    }
}

void sample::StartGame(Event* event)
{
    if (m_pBest)
    {
        static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("PB"))->SetActive(false);
        m_pBest = false;
    }
    SERVICE_LOCATOR.GetSystemSettings()->SetBoolSetting("Pause", false);
    glfwSetInputMode(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
    auto* sceneManager = SERVICE_LOCATOR.GetSceneManager();
    if (!sceneManager->FindScene(levels[level]))
    { 
        sceneManager->AddScene(levels[level]);
    }
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene(levels[level]);
    
}

void sample::GoToVolumeSettings(Event* event)
{
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("volume_menu");
}

void sample::GoToPauseMenu(Event* event)
{
	SERVICE_LOCATOR.GetSystemSettings()->SetBoolSetting("Pause", true);
    m_prevMenu = "pause_menu";
    glfwSetInputMode(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("pause_menu");
}

void sample::BackToPrevMenu(Event* event)
{
	const std::string& currentSceneName = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName();
    if (currentSceneName == "pause_menu")
    {
        m_prevMenu = "level_selection";
        ShutdownCurrentGameScene();
    }
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene(m_prevMenu);
}

void sample::ControlsMenu(Event* event)
{
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("controls_menu");
}

void sample::LevelSelection(Event* event)
{
    if (m_pBest)
    {
        static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("PB"))->SetActive(false);
        m_pBest = false;
    }
    if (SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName() == "pause_menu")
    {
        m_timer = 0.0f;
        m_points = 0;
        ShutdownCurrentGameScene();
    }
    m_prevMenu = "main_menu";
    SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("level_selection");
}

void sample::StartLevelOne(Event* event)
{
    level = 1;
    StartGame(event);
}

void sample::StartLevelTwo(Event* event)
{
    level = 2;
    StartGame(event);
}

void sample::StartLevelThree(Event* event)
{
    level = 3;
    StartGame(event);
}

void sample::RestartLevel(Event* event)
{
    m_timer = 0.0f;
    m_points = 0;

    ShutdownCurrentGameScene();
    StartGame(event);
}

void sample::StartTutorial(Event* event)
{
    level = 0;
    StartGame(event);
}

void sample::ExitGame(Event* event)
{
    glfwSetWindowShouldClose(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_TRUE);
}

void sample::EndLevel(Event* event)
{
	std::string compName = std::string("L" + std::to_string(level) + "_Points");
    int mostPoints = SERVICE_LOCATOR.GetSystemSettings()->GetIntSetting(compName);
    if (m_points > mostPoints) 
    {
        SERVICE_LOCATOR.GetSystemSettings()->SetIntSetting(compName, m_points);
    }
    m_timer = 0.0f;
	m_points = 0;

    ShutdownCurrentGameScene();
	SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene("completed_level");

    compName = std::string("L" + std::to_string(level) + "_Time");
    UITextComponent* textComp = static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("Time"));
    textComp->SetText(std::string(m_levelTime + " seconds").c_str());

    if (m_pBest) 
    {
        static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("PB"))->SetActive(true);
    }

    glfwSetInputMode(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void sample::NextLevel(Event* event)
{
    if (m_pBest)
    {
        static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("PB"))->SetActive(false);
		m_pBest = false;
    }
    if (level >= levels.size() - 1)
    {
        level = 0;
        LevelSelection(event);
    }
    else
    {
        glfwSetInputMode(SERVICE_LOCATOR.GetWindowHandler()->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        level++;
        m_pEventListener->BroadcastEvent(new Event("Resume"));
    }
}

void sample::Credits(Event* event)
{
    Image* team = new Image("credits_team.jpg");
    Image* faculty = new Image("credits_faculty.jpg");
    Image* president = new Image("credits_president.jpg");
    std::vector<Image*> logos = { team, faculty, president };
    double startTime = glfwGetTime();
    int texIndex = 0;
    while (!glfwWindowShouldClose(SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext()))
    {
        if (SERVICE_LOCATOR.GetInput()->IsKeyJustPressed(GLFW_KEY_ESCAPE))
		{
			break;
		}
        double elapsed = glfwGetTime() - startTime;
        if (elapsed >= 4.0)
        {
            if (texIndex >= 2)
            {
                texIndex = 0;
            }
            else
                texIndex++;
            startTime = glfwGetTime();
        }

        float alpha = 1.0f;
        if (elapsed >= 3.0) {
            alpha = static_cast<float>(4.0 - elapsed); // fade from 1 ? 0 over final second
        }

        logos[texIndex]->DrawImage(alpha);
    }
    for (auto& logo : logos)
    {
        delete logo;
    }
}

void sample::DrawReticle()
{
    auto camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
    auto view = camera->GetViewMatrix();
    auto reticlePos = camera->GetPosition() + glm::vec3(0, 0.2, 0) + VectorCalculation::GetForwardVecFromView(view) * 0.1f;
    Debug::DrawSphere(reticlePos, glm::vec3(1, 0, 0), .001);
}

void sample::DrawGrapple()
{
    auto camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
    auto player = SERVICE_LOCATOR.GetGameObjectManager()->GetGameObject("Player");
    auto view = camera->GetViewMatrix();
    // Cast ray and draw hit point
    const auto grappleComponent = player->GetComponent<GrappleComponent>();
    auto ray = SERVICE_LOCATOR.GetCollisionManager()->CastRay(camera->GetPosition() + glm::vec3(0, 0.2, 0), VectorCalculation::GetForwardVecFromView(camera->GetViewMatrix()), grappleComponent->GetCollisionMask(), grappleComponent->GetMaxLength());
    if (ray.Hit() && ray.GetLength() > grappleComponent->GetMinLength())
    {
        Debug::DrawSphere(ray.GetHitPoint(), glm::vec3(0, 1, 0), .1f);
    }
    if (grappleComponent->IsAttached())
    {
        auto playerPos = player->GetComponent<TransformComponent>()->GetPosition();
        auto rayDir = grappleComponent->GetGrappleDirection();
        auto rayLength = grappleComponent->GetGrappleLength();
        // Normalize length to a 0-1 range
        float t = glm::clamp(float(grappleComponent->GetGrappleLength() - grappleComponent->GetMinLength()) / (grappleComponent->GetMaxLength() - grappleComponent->GetMinLength()), 0.0f, 1.0f);
        glm::vec3 color = glm::mix(glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), t);
        auto width = 10.0f * (1 - t);
        Debug::DrawRay(playerPos + 0.25f * VectorCalculation::GetRightVecFromView(view) + 0.55f * VectorCalculation::GetForwardVecFromView(view), rayDir, rayLength, color, width);
    }
}

void sample::ShutdownCurrentGameScene()
{
    SERVICE_LOCATOR.GetSceneManager()->RemoveScene(levels[level]);
    SERVICE_LOCATOR.GetCollisionManager()->Shutdown();
    SERVICE_LOCATOR.GetPhysicsManager()->Shutdown();
    SERVICE_LOCATOR.GetScriptManager()->Shutdown();
}

void sample::FinishLevel()
{
	m_levelFinished = true;
    ScriptComponent* sComp = SERVICE_LOCATOR.GetScriptManager()->FindScriptComponent("Timer");
    if (sComp) 
    {
		sComp->CallLuaFunction("SetFinishState", true);
    }
}

void sample::SetLevelTime(const std::string& time)
{
    // TODO: Add "Level Complete!" text.
    const glm::vec3 GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
    UITextComponent* timer = static_cast<UITextComponent*>(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->FindUIComponent("Timer"));
    if (!timer) { return; }
    timer->SetColor(GREEN);
    float finishTime = std::stof(time);
    std::string compName = std::string("L" + std::to_string(level) + "_Time");
    float currBest = SERVICE_LOCATOR.GetSystemSettings()->GetFloatSetting(compName);
    m_levelTime = time;
    if (currBest < 0.0f || finishTime < currBest) 
    {
        m_pBest = true;
        SERVICE_LOCATOR.GetSystemSettings()->SetFloatSetting(compName, finishTime);
    }
}

void sample::AddPoint()
{
    m_points++;
    printf("%d", m_points);
}
