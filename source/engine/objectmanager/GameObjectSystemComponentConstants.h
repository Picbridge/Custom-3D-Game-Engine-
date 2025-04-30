#pragma once

class RenderComponent;
class TransformComponent;
class PhysicsComponent;
class CollisionComponent;
class CameraControllerComponent;
class ScriptComponent;
class LightComponent;
class ParticleComponent;
class CameraComponent;
class UIComponent;
class UITextComponent;
class GrappleComponent;

class LineShader;
class ShadowShader;
class DebugShader;
class ShadowDebugShader;
class DefaultShader;
class ParticleShader;
class SkyboxShader;
class CelShader;

class CollisionShape;
enum UV_TYPE;

class ComponentNames
{
public:

    inline static constexpr std::string_view RENDER_COMPONENT = "RenderComponent";
    inline static constexpr std::string_view TRANSFORM_COMPONENT = "TransformComponent";
    inline static constexpr std::string_view PHYSICS_COMPONENT = "PhysicsComponent";
    inline static constexpr std::string_view COLLISION_COMPONENT = "CollisionComponent";
    inline static constexpr std::string_view CONTROLLER_COMPONENT = "CameraControllerComponent";
    inline static constexpr std::string_view SCRIPT_COMPONENT = "ScriptComponent";
    inline static constexpr std::string_view LIGHT_COMPONENT = "LightComponent";
    inline static constexpr std::string_view PARTICLE_COMPONENT = "ParticleComponent";
    inline static constexpr std::string_view CAMERA_COMPONENT = "CameraComponent";
	inline static constexpr std::string_view UI_COMPONENT = "UIComponent";
    inline static constexpr std::string_view UI_TEXT_COMPONENT = "UITextComponent";
    inline static constexpr std::string_view GRAPPLE_COMPONENT = "GrappleComponent";

    enum Index 
    {
        RenderComponent,
        TransformComponent,
        PhysicsComponent,
        CollisionComponent,
        ControllerComponent,
        ScriptComponent,
        LightComponent,
        Particle_Component,
		CameraComponent,
        GrappleComponent,
        IndexEnd,
        // Add component enums before "IndexEnd"
        // UIComponent is an exception
        UIComponent,
		UITextComponent
    };

    static constexpr std::string_view names[Index::IndexEnd + 3] =
    {
        RENDER_COMPONENT,
        TRANSFORM_COMPONENT,
        PHYSICS_COMPONENT,
        COLLISION_COMPONENT,
        CONTROLLER_COMPONENT,
        SCRIPT_COMPONENT,
        LIGHT_COMPONENT,
        PARTICLE_COMPONENT,
        CAMERA_COMPONENT,
        GRAPPLE_COMPONENT,
        "NONE",
        // Add component names before "NONE"
        // UI_COMPONENT is an exception
        UI_COMPONENT,
        UI_TEXT_COMPONENT
    };
};

// CollisionShape Constants
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
        const char*,
        std::string, 
        glm::vec2, 
        glm::vec3, 
        glm::dvec3,
        glm::vec4, 
        UV_TYPE,
        CollisionShape*,
        Viewport
    >;

    static const inline std::unordered_map<std::string_view, TypeVariant> typeStore =
    {
        {"int", int{}},
        {"float", float{}},
        {"double", double{}},
        {"bool", bool{}},
        {"const char*", static_cast<const char*>(nullptr)},
        {"string", std::string{}},
        {"vec2", glm::vec2{}},
		{"vec3", glm::vec3{}},
		{"dvec3", glm::dvec3{}},
        {"vec4", glm::vec4{}},
		{"uvType", UV_TYPE{}},
        {"CollisionShape", static_cast<CollisionShape*>(nullptr)},
        {"Viewport", Viewport{}}
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

class ShaderNames
{
public:
	inline static constexpr std::string_view LINE_SHADER = "LineShader";
	inline static constexpr std::string_view SHADOW_SHADER = "ShadowShader";
	inline static constexpr std::string_view DEBUG_SHADER = "DebugShader";
	inline static constexpr std::string_view SHADOW_DEBUG_SHADER = "ShadowDebugShader";
	inline static constexpr std::string_view DEFAULT_SHADER = "DefaultShader";
	inline static constexpr std::string_view PARTICLE_SHADER = "ParticleShader";
	inline static constexpr std::string_view SKYBOX_SHADER = "SkyboxShader";
	inline static constexpr std::string_view CEL_SHADER = "CelShader";
};

class ShaderDictionary
{
public:
    using ShaderTypes = std::variant<
        LineShader*,
        ShadowShader*,
        DebugShader*,
        ShadowDebugShader*,
		DefaultShader*,
        ParticleShader*,
		SkyboxShader*,
        CelShader*
    >;
    
	static const inline std::unordered_map<std::string_view, ShaderTypes> shaderStore =
	{
		{ ShaderNames::LINE_SHADER, static_cast<LineShader*>(nullptr) },
		{ ShaderNames::SHADOW_SHADER, static_cast<ShadowShader*>(nullptr) },
		{ ShaderNames::DEBUG_SHADER, static_cast<DebugShader*>(nullptr) },
		{ ShaderNames::SHADOW_DEBUG_SHADER, static_cast<ShadowDebugShader*>(nullptr) },
		{ ShaderNames::DEFAULT_SHADER, static_cast<DefaultShader*>(nullptr) },
		{ ShaderNames::PARTICLE_SHADER, static_cast<ParticleShader*>(nullptr) },
		{ ShaderNames::SKYBOX_SHADER, static_cast<SkyboxShader*>(nullptr) },
		{ ShaderNames::CEL_SHADER, static_cast<CelShader*>(nullptr) }
	};

    //template <std::size_t I = 0>
    //static void addShaderTypes(std::unordered_map<std::string, ShaderTypes>& map) {
    //    if constexpr (I < std::variant_size_v<ShaderTypes>) {
    //        using PtrT = std::variant_alternative_t<I, ShaderTypes>;
    //        using T = std::remove_pointer_t<PtrT>;

    //        std::string typeName = typeid(T).name();
    //        std::string prefix = "class ";
    //        if (typeName.rfind(prefix, 0) == 0) {
    //            typeName = typeName.substr(prefix.length());
    //        }

    //        map.emplace(typeName, ShaderTypes{ static_cast<PtrT>(nullptr) });
    //        addShaderTypes<I + 1>(map);
    //    }
    //}

    //static const inline std::unordered_map<std::string, ShaderTypes> shaderStore = [] {
    //    std::unordered_map<std::string, ShaderTypes> m;
    //    addShaderTypes(m);
    //    return m;
    //    }();
};

class ComponentDictionary 
{
public:
    using ComponentTypes = std::variant<
        RenderComponent*,
        TransformComponent*,
        PhysicsComponent*,
        CollisionComponent*,
        CameraControllerComponent*,
        ScriptComponent*,
        LightComponent*,
        ParticleComponent*,
        CameraComponent*,
        UIComponent*,
		UITextComponent*,
		GrappleComponent*
    >;

    static const inline std::unordered_map<std::string_view, ComponentTypes> componentStore =
    {
        { ComponentNames::RENDER_COMPONENT, static_cast<RenderComponent*>(nullptr) },
        { ComponentNames::TRANSFORM_COMPONENT, static_cast<TransformComponent*>(nullptr) },
        { ComponentNames::PHYSICS_COMPONENT, static_cast<PhysicsComponent*>(nullptr) },
        { ComponentNames::COLLISION_COMPONENT, static_cast<CollisionComponent*>(nullptr) },
        { ComponentNames::CONTROLLER_COMPONENT, static_cast<CameraControllerComponent*>(nullptr) },
        { ComponentNames::SCRIPT_COMPONENT, static_cast<ScriptComponent*>(nullptr) },
		{ ComponentNames::LIGHT_COMPONENT, static_cast<LightComponent*>(nullptr) },
		{ ComponentNames::PARTICLE_COMPONENT, static_cast<ParticleComponent*>(nullptr) },
		{ ComponentNames::CAMERA_COMPONENT, static_cast<CameraComponent*>(nullptr) },
		{ ComponentNames::UI_COMPONENT, static_cast<UIComponent*>(nullptr) },
		{ ComponentNames::UI_TEXT_COMPONENT, static_cast<UITextComponent*>(nullptr) },
		{ ComponentNames::GRAPPLE_COMPONENT, static_cast<GrappleComponent*>(nullptr) }
    };
};

// GameResourceConstants
class GameResourceConstants
{
public:
    inline static constexpr std::string_view SHADER = "Shader";
	inline static constexpr std::string_view TEXTURES = "Textures";
    inline static constexpr std::string_view DIFFUSE = "Diffuse";
    inline static constexpr std::string_view SPECULAR = "Specular";
    inline static constexpr std::string_view COLOR = "Color";
	inline static constexpr std::string_view SHININESS = "Shininess";
};