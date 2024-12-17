#pragma once

class CollisionShape;
enum UV_TYPE;

class GameObjectSystemComponentConstants
{
public:
    inline static constexpr std::string_view RENDER_COMPONENT = "RenderComponent";
    inline static constexpr std::string_view TRANSFORM_COMPONENT = "TransformComponent";
	inline static constexpr std::string_view PHYSICS_COMPONENT = "PhysicsComponent";
	inline static constexpr std::string_view COLLISION_COMPONENT = "CollisionComponent";
	inline static constexpr std::string_view CONTROLLER_COMPONENT = "ControllerComponent";
	inline static constexpr std::string_view SCRIPT_COMPONENT = "ScriptComponent";
};

//CollisionShape Constants
class CollisionShapeConstants
{
public:
	inline static constexpr std::string_view SPHERE = "CollisionShape_Sphere";
	inline static constexpr std::string_view CUBOID = "CollisionShape_Cuboid";
};

class GameObjectTypeDictionary
{
public:
    using TypeVariant = std::variant<
        int, 
        float, 
        double, 
        bool, 
        std::string, 
        glm::vec2, 
        glm::vec3, 
        glm::dvec3,
        glm::vec4, 
        UV_TYPE,
        CollisionShape*
    >;

    static const inline std::unordered_map<std::string_view, TypeVariant> typeStore =
    {
        {"int", int{}},
        {"float", float{}},
        {"double", double{}},
        {"bool", bool{}},
        {"string", std::string{}},
        {"vec2", glm::vec2{}},
		{"vec3", glm::vec3{}},
		{"dvec3", glm::dvec3{}},
        {"vec4", glm::vec4{}},
		{"uvType", UV_TYPE{}},
        {"CollisionShape", static_cast<CollisionShape*>(nullptr)} 
    };

    static const inline std::unordered_map<std::type_index, std::string_view> typeIndexToName = [] {
        std::unordered_map<std::type_index, std::string_view> map;
        for (const auto& [key, value] : typeStore) {
            std::visit([&](auto&& type) {
                map[typeid(type)] = key;
                }, value);
        }
        return map;
        }();

    static TypeVariant GetTypeVariantByID(std::type_index typeIndex)
    {
        auto it = typeIndexToName.find(typeIndex);
        if (it != typeIndexToName.end()) {
            auto storeIt = typeStore.find(it->second);
            if (storeIt != typeStore.end()) {
                return storeIt->second;
            }
        }
        throw std::runtime_error("Unknown type_index: " + std::string(typeIndex.name()));
    }

    static std::string_view GetTypeNameByID(std::type_index typeIndex)
    {
        auto it = typeIndexToName.find(typeIndex);
        if (it != typeIndexToName.end()) {
            return it->second;
        }
        throw std::runtime_error("Unknown type_index: " + std::string(typeIndex.name()));
    }
};

// GameResourceConstants
class GameResourceConstants
{
public:
    inline static constexpr std::string_view VERTEX_SHADER = "vert";
    inline static constexpr std::string_view FRAGMENT_SHADER = "frag";
    inline static constexpr std::string_view GEOMETRY_SHADER = "geom";
    inline static constexpr std::string_view SHADER = "shader";
    inline static constexpr std::string_view DIFFUSE = "diffuse";
    inline static constexpr std::string_view SPECULAR = "specular";
};