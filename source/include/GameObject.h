#pragma once
class GameObject : public Node
{
public:
	GameObject() : m_isAlive(true) {}
	~GameObject();

	// @brief Initialize all the components added
	void Init();
	//@brief Update the current object
	void Update();
	//@brief Draw the current object
	void Render();
	void Render(Shader* shader);
	//@brief Destroy the current object
	void SetDead(bool isDead);

	const bool IsDead() const { return !m_isAlive; }
private:
	bool m_isAlive;
};