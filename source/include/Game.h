#pragma once

class Game
{
public:
	//@brief Default constructor
	Game() : m_isRunning(true), m_title("") {}

	//@brief Constructor
	Game(WINDOW_PROPS props) : m_isRunning(true), m_title(""), m_windowProps(props) {}

	//@brief Constructor
	Game(int width, int height, std::string title) : m_isRunning(true), m_title(title)
	{
		m_windowProps.Width = width;
		m_windowProps.Height = height;
		m_windowProps.Title = title;
	}

	//@brief Destructor
	virtual ~Game() {};

	//@brief Initializes the game
	virtual void Init() = 0;
	//@brief Updates the game
	virtual void Update() = 0;
	//@brief Renders the game
	virtual void Render() = 0;
	//@brief Post update
	virtual void PostUpdate() = 0;
	//@brief Shuts down the game
	virtual void Shutdown() = 0;
	//@brief Sets the running state of the game
	void SetRunning(bool running) { m_isRunning = running; }
	//@brief Returns if the game is running
	bool IsRunning() const { return m_isRunning; }
	//@brief Returns title of the game
	const char* GetTitle() const { return m_title.c_str(); }

	//@brief Returns the window properties
	WINDOW_PROPS GetWindowProps() const { return m_windowProps; }
protected:
	bool m_isRunning;
	std::string m_title;
	WINDOW_PROPS m_windowProps;
};