#include "pch.h"

std::unique_ptr<Renderer> Renderer::instance = nullptr;

Renderer* Renderer::GetInstance()
{
	if (!instance) 
		instance = std::unique_ptr<Renderer>(new Renderer());
	
	return instance.get();
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);  // Enable depth testing before rendering starts
	std::cout << "Render System Initialized" << std::endl;
}

void Renderer::Render()
{
	//TODO: replace the temporary background color
	//glClearColor(0.7f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear both color and depth buffers before rendering
}

void Renderer::Shutdown()
{
	glDisable(GL_DEPTH_TEST);
	std::cout << "Render System Shutdown" << std::endl;
}