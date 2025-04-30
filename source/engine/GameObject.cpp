#include "pch.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "cameramanager/CameraManager.h"
#include "cameramanager/CameraComponent.h"
#include "ServiceLocator.h"
#include "physics/CollisionComponent.h"
#include "resourcemanager/ResourceManager.h"
#include "objectmanager/GameObjectManager.h"

std::vector<std::string> componentTypesToPass = { "ScriptComponent", "PhysicsComponent", "CollisionComponent" };

GameObject::~GameObject()
{
    for (auto& [key, component] : m_components)
    {
        if (component != nullptr)
        {
            component->SetOwner(nullptr);
            delete component;
        }
    }
    m_components.clear();
    SERVICE_LOCATOR.GetGameObjectManager()->DeleteGameObject(this);
}

void GameObject::Update()
{
    if (m_needsDeletion) return;

    for (std::pair<const std::string, Component*> component_pair : m_components)
    {
        Component* component = component_pair.second;
        if (component == nullptr) { continue; }

        // If a component is on the list of ComponentTypes, skip it
        std::string componentType = component_pair.first;
        if (std::find(componentTypesToPass.begin(), componentTypesToPass.end(), componentType) != componentTypesToPass.end()) 
        { 
            continue; 
        }
        component->Update();
    }
}

void GameObject::Init()
{
}

void GameObject::Render()
{
    //const std::vector<Node*>& children = GetChildren();
    //for (auto& child : children)
    //{
    //    child->Render();
    //}

    auto renderComponent = GetComponent<RenderComponent>();

    if (renderComponent != nullptr)
    {
        auto cameras = SERVICE_LOCATOR.GetCameraManager()->GetCamerasForRendering();
        auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
        for (auto& camera : cameras)
        {
            if (camera->IsActive())
            {
				SERVICE_LOCATOR.GetCameraManager()->SetMainCamera(camera);
				auto viewport = camera->GetViewport();

				renderComponent->Prepass(camera);
				//glViewport(viewport.X, viewport.Y, viewport.W, viewport.H);
                renderComponent->Render();
            }
        }
        glViewport(0, 0, frameBuffer.Width, frameBuffer.Height);
    }

    auto collision = GetComponent<CollisionComponent>();

    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Colliders") && collision != nullptr)
    {
        if (collision)
        {
            CollisionShape* shape = collision->GetCollisionShape();
            ResourceManager* manager = SERVICE_LOCATOR.GetResourceManager();
            Shader* shader = manager->GetShader("Debug");
            Geometry* geometry = manager->GetGeometry("Sphere");
            CameraComponent* camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();

            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(shape->GetPosition()));

            glm::vec3 rotation = glm::radians(glm::vec3(shape->GetRotation()));

            model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

            if (dynamic_cast<CollisionShape_Cuboid*>(shape))
            {
                geometry = manager->GetGeometry("Cube");
                glm::dvec3 scale = dynamic_cast<CollisionShape_Cuboid*>(shape)->GetHalfWidth();
                model = glm::scale(model, glm::vec3(scale));
            }
            if (dynamic_cast<CollisionShape_Sphere*>(shape))
            {
                glm::dvec3 scale = dynamic_cast<CollisionShape_Sphere*>(shape)->GetRadius();
                model = glm::scale(model, glm::vec3(scale));
            }

            shader->Use();
            geometry->Bind(shader);
            Transform* transform = this->GetTransform();
            shader->PassShaderData("model", model, "view", camera->GetViewMatrix(), "projection", camera->GetProjectionMatrix(), "color", glm::vec3(1, 0, 0));

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            geometry->Render();
            geometry->Unbind();
            shader->Unuse();
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void GameObject::Render(Shader* shader)
{
    const std::vector<Node*>& children = GetChildren();
    for (auto& child : children)
    {
        child->Render(shader);
    }

    auto renderComponent = GetComponent<RenderComponent>();

    if (renderComponent != nullptr)
    {
        auto cameras = SERVICE_LOCATOR.GetCameraManager()->GetCamerasForRendering();
        auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;

        for (auto& camera : cameras)
        {
            if (camera->IsActive())
            {
                SERVICE_LOCATOR.GetCameraManager()->SetMainCamera(camera);
                auto viewport = camera->GetViewport();
                glViewport(viewport.X, viewport.Y, viewport.W, viewport.H);
                renderComponent->Render(shader);
                glViewport(0, 0, frameBuffer.Width, frameBuffer.Height);
            }
        }
    }
}

void GameObject::SetDead(bool isDead)
{
    m_isAlive = !isDead;
}