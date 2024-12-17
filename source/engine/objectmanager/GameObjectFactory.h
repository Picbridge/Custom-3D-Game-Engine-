#pragma once
class GameObjectFactory
{
public:
	// @brief load in the json and create game objects based on the data loaded
	// @param source: path of the json file 
	void CreateAllGameObjects(const rapidjson::Value& gameObjects);

private:
	static GameObjectFactory* GetInstance();
	static std::unique_ptr<GameObjectFactory> instance;

	// @brief creates game object with extracted components
	// @param member: set of component name and data 
	void createGameObject(rapidjson::Value::ConstMemberIterator member, GameObject* pParent = nullptr);

	void deserialize(const rapidjson::Value& components, GameObject* gameObject);
	CollisionShape* parseCollisionShape(const rapidjson::Value& collisionShapeData);
	
	friend class ServiceLocator;
};

