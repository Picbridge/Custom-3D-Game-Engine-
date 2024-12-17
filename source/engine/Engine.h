#pragma once
class Engine
{
public:
	static Engine* GetInstance();

	~Engine();

	//@brief Sets the game (first game pushed by default)
	//@param game : The game to set
	void SetGame(Game* game);
	//@brief pushes Game to game vector
	void PushGame(Game* game);
	Game* FindGame(std::string name);

	//@brief Runs the engine
	void Run();

private:
	unsigned int m_prevGameIndex = 0;
	static std::unique_ptr<Engine> instance;
	Game* m_pGame = nullptr;
	std::vector<Game*> m_games;

	//@brief Initializes the engine
	void init();
	//@brief Updates the engine
	void update();
	//@brief Renders the content
	void render();
	//@brief Post update
	void postUpdate();
	//@brief Shuts down the engine
	void shutdown();
	//@brief initializes all games
	void initGames();
};

