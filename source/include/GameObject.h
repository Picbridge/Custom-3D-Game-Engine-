#pragma once  
#include "../engine/Node.h"  
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

   // @brief Set the current object to be active or inactive  
   void SetActive(bool isActive) { m_isActive = isActive; }  

   // @brief Check if the object is active  
   const bool IsActive() const { return m_isActive; }  

private:  
   bool m_isAlive;  
   bool m_isActive = true;  
};