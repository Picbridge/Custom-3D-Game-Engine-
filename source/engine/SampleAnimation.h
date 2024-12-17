#pragma once

class Skybox;
class Model;

class SampleAnimation :
    public Game
{
public:
	//@brief Constructor
	SampleAnimation() : Game() {}

	//@brief Constructor
	SampleAnimation(WINDOW_PROPS props) : Game(props) {}

	//@brief Constructor
	SampleAnimation(int width, int height, std::string title) :Game(width, height, title) {}

	void Init() override;

	void Update() override;

	void PostUpdate() override;

	void Render() override;

	void Shutdown() override;
private:
	//TODO: GameObjectMagager should be created to manage game objects
	std::vector<GameObject*> m_gameObjects;
	Skybox* m_pSkybox;
	std::unique_ptr<Model> root;
};

