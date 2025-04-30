#include "pch.h"
#include "headers.h"

#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	Engine* engine = Engine::GetInstance();

	std::unique_ptr<Game> game = nullptr;
	game = std::unique_ptr<Game>(new sample(0, 0, "Grappling With Ideas"));
	engine->SetGame(game.get());

	engine->Run();
}