#pragma once
class GameObject;

class Renderer
{
public:
	Renderer(){}

	//@brief Initializes the renderer
	void Init();
	//@brief Updates the renderer
	void Render();
	//@brief Shuts down the renderer
	void Shutdown();

private:
	static Renderer* GetInstance();
	static std::unique_ptr<Renderer> instance;

	friend class ServiceLocator;
};