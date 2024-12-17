#include "pch.h"
#include "Engine.h"
#include "headers.h"

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
	for (auto& game : m_games)
		delete(game);
}

void Engine::SetGame(Game* game)
{
	if (!game)
	{
		std::cout << "No game set" << std::endl;
		return;
	}
	m_pGame = game;
	SERVICE_LOCATOR.GetWindowHandler()->Props = m_pGame->GetWindowProps();
}

void Engine::PushGame(Game* game)
{
	UI* ui = SERVICE_LOCATOR.GetUI();
	m_games.push_back(game);
	ui->PushGame(game->GetTitle());
	if (!m_pGame)
		SetGame(game);
}

Game* Engine::FindGame(std::string name)
{
	Game* game = NULL;
	for (int i = 0; i < m_games.size(); ++i)
	{
		if (strcmp(m_games[i]->GetTitle(), name.c_str()))
		{
			game = m_games[i];
		}
	}
	return game;
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
	SERVICE_LOCATOR.GetTime()->Init(1.0f / 60.0f);
	SERVICE_LOCATOR.GetWindowHandler()->Init();
	SERVICE_LOCATOR.GetRenderer()->Init();
	context = SERVICE_LOCATOR.GetWindowHandler()->GetCurrentContext();
	SERVICE_LOCATOR.GetInput()->Init(context);
	SERVICE_LOCATOR.GetAudioManager()->Init("../../content/code/json_files/Audio.json");
	
	UI* ui = SERVICE_LOCATOR.GetUI();
	ui->Init(context);
	ui->SetDebug(false);

	initGames();

    std::cout << "Engine Initialized" << std::endl;
}

void Engine::update()
{
	Input* input = SERVICE_LOCATOR.GetInput();
	UI* ui = SERVICE_LOCATOR.GetUI();
	
	SERVICE_LOCATOR.GetAudioManager()->Update();
	SERVICE_LOCATOR.GetWindowHandler()->Update();
	input->Update();

#ifdef _DEBUG
	if (input->IsKeyJustPressed(GLFW_KEY_P) ||
		input->IsGamepadButtonJustPressed(0, GLFW_GAMEPAD_BUTTON_BACK)) {
		ui->ToggleDebug();
	}
	ui->Update();
#endif

	SERVICE_LOCATOR.GetTime()->Update();

#ifdef _DEBUG
	if (!ui->GetIsPaused())
#endif
	SERVICE_LOCATOR.GetPhysicsManager()->Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime());

	SERVICE_LOCATOR.GetCollisionManager()->Update();
	SERVICE_LOCATOR.GetEventHandler()->Update();
	Camera::GetInstance()->Update();
	SERVICE_LOCATOR.GetScriptManager()->Update(SERVICE_LOCATOR.GetTime()->GetDeltaTime());

#ifdef _DEBUG
	if (!ui->GetIsPaused())
#endif
	m_pGame->Update();
}

void Engine::render()
{
    //TODO: Should be replaced after the scene manager is implemented
	SERVICE_LOCATOR.GetRenderer()->Render();
	m_pGame->Render();
	SERVICE_LOCATOR.GetUI()->Render();
}

void Engine::postUpdate()
{
	SERVICE_LOCATOR.GetInput()->GetInstance().PostUpdate();
	SERVICE_LOCATOR.GetWindowHandler()->SwapBuffers();
	m_pGame->PostUpdate();
    if (SERVICE_LOCATOR.GetWindowHandler()->ShouldClose())
        m_pGame->SetRunning(false);

	unsigned int currGameIndex = SERVICE_LOCATOR.GetUI()->GetGameIndex();
	if (m_prevGameIndex != currGameIndex)
	{
		SetGame(m_games[currGameIndex]);
		m_prevGameIndex = currGameIndex;
	}
}

void Engine::shutdown()
{
	m_pGame->Shutdown();
	SERVICE_LOCATOR.GetRenderer()->Shutdown();
	SERVICE_LOCATOR.GetUI()->Shutdown();
	SERVICE_LOCATOR.GetWindowHandler()->Shutdown();
	SERVICE_LOCATOR.GetAudioManager()->Shutdown();
	exit(EXIT_SUCCESS);
	std::cout << "Engine Shutdown" << std::endl;
	//delete instance;
}

void Engine::initGames()
{
	if (m_games.size() > 0)
		for (auto& game : m_games)
			game->Init();
}
