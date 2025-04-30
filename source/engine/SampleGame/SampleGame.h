#pragma once
class ParticleComponent;

enum CollisionType
{
	OBSTACLE = 1 << 0,
	OBJECT = 1 << 1
};

class sample : public Game
{
public:
	//@brief Constructor
	sample() : Game() {}

	//@brief Constructor
	sample(WINDOW_PROPS props) : Game(props) {}

	//@brief Constructor
	sample(int width, int height, std::string title) : Game(width, height, title) {}

	void Init() override;

	void Update() override;

	void PostUpdate() override;

	void Render() override;

	void Shutdown() override;

	static void FinishLevel();
	static void SetLevelTime(const std::string& time);
	static void AddPoint();
private:
	void StartGame(Event* event);
	void GoToVolumeSettings(Event* event);
	void GoToPauseMenu(Event* event);
	void BackToPrevMenu(Event* event);
	void ControlsMenu(Event* event);
	void LevelSelection(Event* event);
	void StartTutorial(Event* event);
	void StartLevelOne(Event* event);
	void StartLevelTwo(Event* event);
	void StartLevelThree(Event* event);
	void RestartLevel(Event* event);
	void ExitGame(Event* event);
	void EndLevel(Event* event);
	void NextLevel(Event* event);
	void Credits(Event* event);

	void DrawReticle();
	void DrawGrapple();
	void ShutdownCurrentGameScene();

	static EventListener* m_pEventListener;

	bool m_hasInitGame = false;
	static bool m_levelFinished;
	std::string m_prevMenu = "main_menu";

	const char* m_levelNames[3] = { "Tutorial", "Level 1", "Level 2" };
	static std::string m_levelTime;
	static int m_levelFinishTimesIndex;
	static bool m_pBest;

	float m_timer = 0.0f;
	static int m_points;

	std::vector<std::string> levels;
	static int level;
};