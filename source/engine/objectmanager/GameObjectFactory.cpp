#include "../pch.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../physics/PhysicsComponent.h"
#include "../physics/CollisionComponent.h"
#include "../physics/CollisionManager.h"
#include "../physics/CollisionShape_Sphere.h"
#include "../ControllerComponent.h"
#include "../ScriptComponent.h"
#include "../SampleGame/SampleGame.h"



std::unique_ptr<GameObjectFactory> GameObjectFactory::instance = nullptr;

GameObjectFactory* GameObjectFactory::GetInstance()
{
    if (!instance)
        instance = std::unique_ptr<GameObjectFactory>(new GameObjectFactory());
    return instance.get();
}

void GameObjectFactory::CreateAllGameObjects(const rapidjson::Value& gameObjects)
{
	std::vector<std::thread> threads;
	for (rapidjson::Value::ConstMemberIterator it = gameObjects.MemberBegin(); it != gameObjects.MemberEnd(); ++it)
		threads.push_back(std::thread([this, it]() { this->createGameObject(it); }));
		
	for (auto& th : threads) 
	{
		if (th.joinable()) 
		{ 
			th.join(); 
		}
	}
}

void GameObjectFactory::createGameObject(rapidjson::Value::ConstMemberIterator member, GameObject* pParent)
{
    GameObject* gameObject = new GameObject();
    gameObject->SetName(member->name.GetString());
    if (pParent)
		pParent->AddChild(gameObject);

    // Iterate through the components of the game object
    const rapidjson::Value& components = member->value["Components"];
    deserialize(components, gameObject);

  
	const rapidjson::Value& children = member->value["Children"];
  std::vector<std::thread> threads;
  
    for (rapidjson::Value::ConstMemberIterator it = children.MemberBegin(); it != children.MemberEnd(); ++it)
        threads.push_back(std::thread([this, it, gameObject]() { this->createGameObject(it, gameObject); }));
  
    for (auto& th : threads) 
    {
      if (th.joinable()) 
      { 
        th.join(); 
      }
    }

    SERVICE_LOCATOR.GetGameObjectManager()->AddGameObject(gameObject);
}

void GameObjectFactory::deserialize(const rapidjson::Value& components, GameObject* gameObject)
{
    for (auto& comp : components.GetObject())
    {
        std::string componentName = comp.name.GetString();

        Component* component = nullptr;
        if (componentName == GameObjectSystemComponentConstants::TRANSFORM_COMPONENT)
            component = gameObject->AddComponent<TransformComponent>();
        else if (componentName == GameObjectSystemComponentConstants::RENDER_COMPONENT)
            component = gameObject->AddComponent<RenderComponent>();
        else if (componentName == GameObjectSystemComponentConstants::PHYSICS_COMPONENT)
            component = gameObject->AddComponent<PhysicsComponent>();
        else if (componentName == GameObjectSystemComponentConstants::COLLISION_COMPONENT)
            component = gameObject->AddComponent<CollisionComponent>();
        else if (componentName == GameObjectSystemComponentConstants::CONTROLLER_COMPONENT)
            component = gameObject->AddComponent<ControllerComponent>();
        else if (componentName == GameObjectSystemComponentConstants::SCRIPT_COMPONENT)
            component = gameObject->AddComponent<ScriptComponent>();
        else
        {
            std::cerr << "Unknown component type: " << componentName << std::endl;
            continue;
        }

        // Process component properties
        const auto& setters = component->GetSetters();
        for (auto& member : comp.value.GetObject())
        {
            std::string variableName = member.name.GetString();
            std::string typeName = member.value[0].GetString();

            auto name2type = GameObjectTypeDictionary::typeStore.find(typeName);
            if (name2type != GameObjectTypeDictionary::typeStore.end())
            {
                std::visit([&](auto&& type) {
                    using T = std::decay_t<decltype(type)>;
                    if constexpr (std::is_same_v<T, glm::vec2>)
                    {
                        glm::vec2 vec;
                        auto array = member.value[1].GetArray();
                        vec.x = array[0].GetFloat();
                        vec.y = array[1].GetFloat();
                        setters.at(variableName)(vec);
                    }
                    else
                    if constexpr (std::is_same_v<T, glm::vec3>)
                    {
                        glm::vec3 vec;
                        auto array = member.value[1].GetArray();
                        vec.x = array[0].GetFloat();
                        vec.y = array[1].GetFloat();
                        vec.z = array[2].GetFloat();
                        setters.at(variableName)(vec);
                    }
					else if constexpr (std::is_same_v<T, glm::dvec3>)
					{
						glm::dvec3 vec;
						auto array = member.value[1].GetArray();
						vec.x = array[0].GetDouble();
						vec.y = array[1].GetDouble();
						vec.z = array[2].GetDouble();
						setters.at(variableName)(vec);
					}
                    else if constexpr (std::is_same_v<T, glm::vec4>)
                    {
                        glm::vec4 vec;
                        auto array = member.value[1].GetArray();
                        vec.x = array[0].GetFloat();
                        vec.y = array[1].GetFloat();
                        vec.z = array[2].GetFloat();
                        vec.w = array[3].GetFloat();
                        setters.at(variableName)(vec);
                    }
                    else if constexpr (std::is_same_v<T, std::string>)
                    {
                        std::string strValue = member.value[1].GetString();
                        setters.at(variableName)(strValue);
                    }
                    else if constexpr (std::is_same_v<T, double>)
                    {
                        double doubleValue = member.value[1].GetDouble();
                        setters.at(variableName)(doubleValue);
                    }
                    else if constexpr (std::is_same_v<T, float>)
                    {
                        float floatValue = member.value[1].GetFloat();
                        setters.at(variableName)(floatValue);
                    }
                    else if constexpr (std::is_same_v<T, int>)
                    {
                        int intValue = member.value[1].GetInt();
                        setters.at(variableName)(intValue);
                    }
                    else if constexpr (std::is_same_v<T, bool>)
                    {
                        bool boolValue = member.value[1].GetBool();
                        setters.at(variableName)(boolValue);
                    }
                    else if constexpr (std::is_same_v<T, UV_TYPE>)
                    {
                        UV_TYPE uvType = static_cast<UV_TYPE>(member.value[1].GetInt());
                        setters.at(variableName)(uvType);
                    }
                    else if constexpr (std::is_same_v<T, CollisionShape*>)
                    {
                        auto collisionShape = parseCollisionShape(member.value[1].GetObject());
                        setters.at(variableName)(static_cast<CollisionShape*>(collisionShape));
                    }
                    }, name2type->second);
            }
        }
    }
}

CollisionShape* GameObjectFactory::parseCollisionShape(const rapidjson::Value& collisionShapeData)
{
    std::string shapeType = collisionShapeData["shapeType"][1].GetString();

    CollisionShape* shape = nullptr;
    if (shapeType == CollisionShapeConstants::SPHERE)
        shape = new CollisionShape_Sphere();
    else if (shapeType == CollisionShapeConstants::CUBOID)
        shape = new CollisionShape_Cuboid();
    else
    {
        std::cerr << "Unknown collision shape type: " << shapeType << std::endl;
        return nullptr;
    }

    // Populate shape properties
    const auto& shapeSetters = shape->GetSetters();
    for (auto& shapeMember : collisionShapeData.GetObject())
    {
        std::string variableName = shapeMember.name.GetString();
        std::string typeName = shapeMember.value[0].GetString();

        auto name2type = GameObjectTypeDictionary::typeStore.find(typeName);
        if (name2type != GameObjectTypeDictionary::typeStore.end())
        {
            std::visit([&](auto&& type) {
                using T = std::decay_t<decltype(type)>;
                if constexpr (std::is_same_v<T, double>)
                {
                    double doubleValue = shapeMember.value[1].GetDouble();
                    shapeSetters.at(variableName)(doubleValue);
                }
                else if constexpr (std::is_same_v<T, glm::dvec3>)
                {
                    glm::dvec3 vec;
                    auto array = shapeMember.value[1].GetArray();
                    vec.x = array[0].GetDouble();
                    vec.y = array[1].GetDouble();
                    vec.z = array[2].GetDouble();
                    shapeSetters.at(variableName)(vec);
                }
             }, name2type->second);
        }
    }

    return shape;
}
