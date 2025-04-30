#include "pch.h"
#include "Engine.h"
#include "headers.h"
#include "scenemanager/SceneManager.h"
#include "Image.h"

std::unique_ptr<Engine> Engine::instance = nullptr;

Engine* Engine::GetInstance()
{
	if (!instance) {
		instance = std::unique_ptr<Engine>(new Engine());
	}
	return instance.get();
}

Engine::~Engine()
{
    delete m_pGame;
}

void Engine::SetGame(Game* game)
{
    assert(game && "Game is null");
	m_pGame = game;
	SERVICE_LOCATOR.GetWindowHandler()->Props = m_pGame->GetWindowProps();
}

void Engine::PushGame(Game* game)
{
	SetGame(game);
}

void Engine::DrawCredits()
{
	Image* digi = new Image("intro_digipen.jpg");
	Image* fmod = new Image("intro_fmod.jpg");
	std::vector<Image*> logos = { digi, fmod };
	double startTime = glfwGetTime();
	int texIndex = 0;
	while (!glfwWindowShouldClose(SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext()))
	{
		double elapsed = glfwGetTime() - startTime;
		if (elapsed >= 3.0)
		{
			if (texIndex >= 1)
			{
				break;
			}
			else
				texIndex++;
			startTime = glfwGetTime();
		}

		float alpha = 1.0f;
		if (elapsed >= 2.0) {
			alpha = static_cast<float>(3.0 - elapsed); // fade from 1 ? 0 over final second
		}

		logos[texIndex]->DrawImage(alpha);
	}
	for (auto& logo : logos)
	{
		delete logo;
	}
}

void Engine::Run()
{
#ifdef _DEBUG
	std::cout.rdbuf(SERVICE_LOCATOR.GetUI()->GetConsoleBuffer().rdbuf());
#endif

	if (!m_pGame)
	{
		std::cout << "No game set" << std::endl;
		return;
	}

	init();

#ifndef _DEBUG
	DrawCredits();
#endif

	while (m_pGame->IsRunning())
	{
		update();
		render();
		postUpdate();
	}
	shutdown();
}

void Engine::init()
{
	GLFWwindow* context = nullptr;
	SERVICE_LOCATOR.GetSystemSettings()->Init();
	SERVICE_LOCATOR.GetTime()->Init(1.0f / 60.0f);
	SERVICE_LOCATOR.GetWindowHandler()->Init();
	SERVICE_LOCATOR.GetRenderer()->Init();
	context = SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext();
	SERVICE_LOCATOR.GetInput()->Init(context);
	SERVICE_LOCATOR.GetAudioManager()->Init();
	SERVICE_LOCATOR.GetResourceFactory()->CreateDefaultResources();
	SERVICE_LOCATOR.GetCameraManager()->Init();

#ifdef _DEBUG// Initialize engine camera for debug mode
	SERVICE_LOCATOR.GetCameraManager()->AddEngineCamera();
#endif // DEBUG

	initGames();

	UI* ui = SERVICE_LOCATOR.GetUI();
	ui->Init(context);
	ui->SetDebug(false);
	ui->SetScenes();

	std::cout << "Engine Initialized" << std::endl;
}

void Engine::update()
{
	Input* input = SERVICE_LOCATOR.GetInput();
	UI* ui = SERVICE_LOCATOR.GetUI();

	SERVICE_LOCATOR.GetEventHandler()->Update();
	SERVICE_LOCATOR.GetAudioManager()->Update();
	SERVICE_LOCATOR.GetWindowHandler()->Update();
	SERVICE_LOCATOR.GetCameraManager()->Update();
	input->Update();

#ifdef _DEBUG
	// Testing engine camera toggle
	// TODO: needs to be add to UI 
	if (input->IsKeyJustPressed(GLFW_KEY_L) && !ui->Interacting())
		SERVICE_LOCATOR.GetCameraManager()->ToggleEngineCamera();
#endif

	ui->Update();
	SERVICE_LOCATOR.GetTime()->Update();

#ifdef _DEBUG
	if ((input->IsKeyJustPressed(GLFW_KEY_P) ||
		input->IsGamepadButtonJustPressed(0, GLFW_GAMEPAD_BUTTON_BACK)) && !ui->Interacting()) {
		ui->ToggleDebug();
	}
	if (!ui->GetIsPaused()) {
#endif

	SERVICE_LOCATOR.GetPhysicsManager()->Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
	SERVICE_LOCATOR.GetCollisionManager()->Update();
	SERVICE_LOCATOR.GetScriptManager()->Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
	SERVICE_LOCATOR.GetParticleManager()->Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime());
	m_pGame->Update();

#ifdef _DEBUG
	}
#endif
}

void Engine::render()
{
	//TODO: Should be replaced after the scene manager is implemented
	SERVICE_LOCATOR.GetRenderer()->Render();
	m_pGame->Render();
	SERVICE_LOCATOR.GetParticleManager()->Render();
	SERVICE_LOCATOR.GetUI()->Render();
}

void Engine::postUpdate()
{
	SERVICE_LOCATOR.GetInput()->GetInstance().PostUpdate();
	SERVICE_LOCATOR.GetWindowHandler()->SwapBuffers();
	m_pGame->PostUpdate();
	if (SERVICE_LOCATOR.GetWindowHandler()->ShouldClose())
		m_pGame->SetRunning(false);

#ifdef _DEBUG
	if (SERVICE_LOCATOR.GetUI()->GetSelectedSceneName().compare(SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetName()) != 0)
	{
		SERVICE_LOCATOR.GetSceneManager()->SetCurrentScene(SERVICE_LOCATOR.GetUI()->GetSelectedSceneName());
	}
#endif
}

void Engine::shutdown()
{
	m_pGame->Shutdown();
	SERVICE_LOCATOR.GetRenderer()->Shutdown();
	SERVICE_LOCATOR.GetSceneManager()->Shutdown();
	SERVICE_LOCATOR.GetUI()->Shutdown();
	SERVICE_LOCATOR.GetWindowHandler()->Shutdown();
	SERVICE_LOCATOR.GetCameraManager()->Shutdown();
	SERVICE_LOCATOR.GetSystemSettings()->Shutdown();
	SERVICE_LOCATOR.GetAudioManager()->Shutdown();

	exit(EXIT_SUCCESS);
	std::cout << "Engine Shutdown" << std::endl;
	//delete instance;
}

void Engine::initGames()
{
    m_pGame->Init();
}
