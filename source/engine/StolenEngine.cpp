#include "pch.h"
#include "headers.h"

int main()
{
	Engine* engine = Engine::GetInstance();

	std::unique_ptr<Game> game = nullptr;
	std::unique_ptr<Game> anim = nullptr;
	game = std::unique_ptr<Game>(new sample(1080, 1080, "Sample"));
	anim = std::unique_ptr<Game>(new SampleAnimation(1080, 1080, "SampleAnim"));
	engine->PushGame(game.get());
	engine->PushGame(anim.get());

	engine->Run();
}