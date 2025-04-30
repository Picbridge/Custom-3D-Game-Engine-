#pragma once
class GameObjectFactory
{
public:
	// @brief load in the json and create game objects based on the data loaded
	// @param source: path of the json file 
	void CreateAllGameObjects(const rapidjson::Value& gameObjects);

	// @brief load in the json and create ui objects based on the data loaded
	// @param uiObjects: rapidjson::Value of ui objects within "UI" json object
	void CreateAllUIObjects(const rapidjson::Value& uiObjects);

private:
	static GameObjectFactory* GetInstance();
	static std::unique_ptr<GameObjectFactory> instance;

	// @brief creates game object with extracted components
	// @param member: set of component name and data 
	void createGameObject(rapidjson::Value::ConstMemberIterator member, GameObject* pParent = nullptr);

	// @brief creates ui object with extracted components
	// @param member: set of component name and data
	void createUIObject(rapidjson::Value::ConstMemberIterator member, Node* pParent = nullptr);

	void deserialize(const rapidjson::Value& components, Node* gameObject);
	CollisionShape* parseCollisionShape(const rapidjson::Value& collisionShapeData);
	
	friend class ServiceLocator;
};

