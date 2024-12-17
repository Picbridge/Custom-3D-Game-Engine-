#pragma once

class StressTest : public Game
{
public:
	//@brief Constructor
	StressTest() : Game() {}

	//@brief Constructor
	StressTest(WINDOW_PROPS props) : Game(props) {}

	//@brief Constructor
	StressTest(int width, int height, std::string title) :Game(width, height, title) {}

	void Init() override;

	void Update() override;

	void Render() override;

	void Shutdown() override;
private:
	//TODO: GameObjectMagager should be created to manage game objects
	std::vector<GameObject*> m_gameObjects;
	Skybox* m_pSkybox;
};

