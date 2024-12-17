#include "pch.h"
#include "headers.h"
#include "RenderComponent.h"
#include "Model.h"
#include "scenemanager/SceneManager.h"

static void PrintMatrix(const glm::mat4& mat) {
    std::cout << "R" << std::endl;
    std::cout << std::fixed << std::setprecision(3); // Optional formatting
    for (int i = 0; i < 4; ++i) { // Loop over rows
        for (int j = 0; j < 4; ++j) { // Loop over columns
            std::cout << std::setw(10) << mat[j][i] << " ";
        }
        std::cout << "\n"; // Newline after each row
    }
    std::cout << std::endl; // Additional newline for separation
}

RenderComponent::RenderComponent()
{
    defineMember();
}

RenderComponent::~RenderComponent()
{
    Shutdown();
}

void RenderComponent::Init()
{
    m_pMaterial = SERVICE_LOCATOR.GetResourceManager()->GetMaterial("Default");
    m_pGeometry = nullptr;

    glGenVertexArrays(1, &m_LineVAO);
    glGenBuffers(1, &m_LineVBO);

    glBindVertexArray(m_LineVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_LineVertices), m_LineVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderComponent::Update()
{
}

void RenderComponent::Shutdown()
{
    if (m_pGeometry != nullptr)
    {
        m_pGeometry->Unbind();
		// will be deleted by the ResourceManager
		m_pGeometry = nullptr;
    }

	if (m_pMaterial != nullptr)
	{
		m_pMaterial->Unbind();
		if (m_pMaterial->GetShader() != nullptr)
			m_pMaterial->GetShader()->Unuse();
        // will be deleted by the ResourceManager
		m_pMaterial = nullptr;
	}
}

void RenderComponent::Render()
{
    if (SERVICE_LOCATOR.GetUI()->GetState("Debug Options", "Wireframes", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (!m_pMaterial || !m_pGeometry)
		std::cerr << "RenderComponent::Render() - Material or Geometry not set" << std::endl;

    Transform* transform = GetOwner()->GetTransform();
    Scene* scene = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene();
    glm::mat4 shadowMatrix = SERVICE_LOCATOR.GetGameObjectManager()->GetShadowMatrix(transform->GetProjection());
    m_pMaterial->GetShader()->Use();
    m_pGeometry->Bind(m_pMaterial->GetShader());

    if (SERVICE_LOCATOR.GetUI()->GetState("Debug Options", "Normals", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
        m_pMaterial->GetShader()->SetUniform("DebugNormal", 1);
    else
        m_pMaterial->GetShader()->SetUniform("DebugNormal", 0);

    m_pMaterial->GetShader()->SetUniform("model", GetOwner()->GetWorldTransform());
    m_pMaterial->GetShader()->SetUniform("view", transform->GetView());
    m_pMaterial->GetShader()->SetUniform("projection", transform->GetProjection());
    m_pMaterial->GetShader()->SetUniform("localModel", transform->GetModel());
    m_pMaterial->GetShader()->SetUniform("light.position", scene->lightPosition);
    m_pMaterial->GetShader()->SetUniform("light.ambient", scene->lightAmbient);
    m_pMaterial->GetShader()->SetUniform("light.diffuse", scene->lightDiffuse);
    m_pMaterial->GetShader()->SetUniform("light.specular", scene->lightSpecular);
    m_pMaterial->GetShader()->SetUniform("textureType", m_pGeometry->GetUVType());
    m_pMaterial->GetShader()->SetUniform("shadowMatrix", shadowMatrix);
    m_pMaterial->GetShader()->SetUniform("lightSpaceMatrix", scene->lightSpaceMatrix);

    m_pMaterial->SetupUniformData();


    m_pMaterial->Bind();
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, scene->depthMap);
	m_pGeometry->Render();
    m_pGeometry->Unbind();
    m_pMaterial->Unbind();
    m_pMaterial->GetShader()->Unuse();

    if (SERVICE_LOCATOR.GetUI()->GetState("Debug Options", "Colliders", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
        DrawCollider();
    if (SERVICE_LOCATOR.GetUI()->GetState("Debug Options", "Velocities", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
        DrawVelocity();
}

void RenderComponent::Render(Shader* shader)
{
    if (!m_pMaterial || !m_pGeometry)
        std::cerr << "RenderComponent::Render() - Material or Geometry not set" << std::endl;

    //shader->Use();
    m_pGeometry->Bind(shader);

    shader->SetUniform("model", GetOwner()->GetWorldTransform());

    //m_pMaterial->Bind();
    m_pGeometry->Render();
    m_pGeometry->Unbind();
    //m_pMaterial->Unbind();
    //shader->Unuse();

}

void RenderComponent::DrawCollider()
{
    CollisionComponent* collision = GetOwner()->GetComponent<CollisionComponent>();
    if (collision)
    {
        CollisionShape* shape = collision->GetCollisionShape();
        ResourceManager* manager = SERVICE_LOCATOR.GetResourceManager();
        Shader* shader = manager->GetShader("Debug");
        Geometry* geometry = manager->GetGeometry("Sphere");

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(shape->GetPosition()));

        glm::vec3 rotation = glm::radians(glm::vec3(shape->GetRotation()));

        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        if (dynamic_cast<CollisionShape_Cuboid*>(shape))
        {
            Geometry* geometry = manager->GetGeometry("Cube");
            model = glm::scale(model, glm::vec3(dynamic_cast<CollisionShape_Cuboid*>(shape)->GetHalfWidth()));
        }
        if (dynamic_cast<CollisionShape_Sphere*>(shape))
        {
            glm::dvec3 scale = dynamic_cast<CollisionShape_Sphere*>(shape)->GetRadius();
            model = glm::scale(model, glm::vec3(scale));
        }

        shader->Use();
        geometry->Bind(shader);
        Transform* transform = GetOwner()->GetTransform();
        shader->SetUniform("model", model);
        shader->SetUniform("view", transform->GetView());
        shader->SetUniform("projection", transform->GetProjection());

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        geometry->Render();
        geometry->Unbind();
        shader->Unuse();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderComponent::DrawVelocity()
{
    PhysicsComponent* physics = GetOwner()->GetComponent<PhysicsComponent>();
    if (physics)
    {
        Transform* transform = GetOwner()->GetTransform();
        Shader* shader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Line");
        shader->Use();
        shader->SetUniform("WorldProjection", transform->GetProjection());
        shader->SetUniform("WorldView", transform->GetView());

        m_LineVertices[0] = transform->GetModel() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m_LineVertices[1] = transform->GetModel() * glm::vec4(physics->GetVelocity(), 1.0f);

        glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_LineVertices), m_LineVertices, GL_DYNAMIC_DRAW);

        glBindVertexArray(m_LineVAO);
        glDrawArrays(GL_LINES, 0, 2); // GL_LINES to draw a line between two vertices
        glBindVertexArray(0);

        shader->Unuse();
    }
}

void RenderComponent::SetColor(glm::vec3 color)
{
    m_pMaterial->SetColor(color);
}

void RenderComponent::SetColor(float r, float g, float b)
{
    m_pMaterial->SetColor(r, g, b);
}

void RenderComponent::SetMaterial(Material* pMaterial)
{
    m_pMaterial = pMaterial;
}

void RenderComponent::SetMaterial(const std::string name)
{
    m_pMaterial = SERVICE_LOCATOR.GetResourceManager()->GetMaterial(name);
}

void RenderComponent::SetShader(Shader* pShader)
{
    if (!m_pMaterial) return;
    m_pMaterial->SetShader(pShader);
}

void RenderComponent::SetShader(const std::string name)
{
    if (!m_pMaterial) return;
    m_pMaterial->SetShader(name);
}

void RenderComponent::SetGeometry(Geometry* pGeometry)
{
    m_pGeometry = pGeometry;
}

void RenderComponent::SetUVType(UV_TYPE type)
{
	m_pGeometry->SetUVType(type);
}

void RenderComponent::SetGeometry(const std::string& geometryName)
{
    m_pGeometry = SERVICE_LOCATOR.GetResourceManager()->GetGeometry(geometryName);
}

void RenderComponent::defineMember()
{
    m_setters["material"] = [this](std::any value) { SetMaterial(std::any_cast<const std::string&>(value)); };
    m_setters["shader"] = [this](std::any value) { SetShader(std::any_cast<const std::string&>(value)); };
    m_setters["geometry"] = [this](std::any value) { SetGeometry(std::any_cast<const std::string&>(value)); };
    m_setters["color"] = [this](std::any value) { SetColor(std::any_cast<glm::vec3>(value)); };
    m_setters["uvType"] = [this](std::any value) { SetUVType(std::any_cast<UV_TYPE>(value)); };

    m_getters["material"] = [this]() -> std::any {
        return SERVICE_LOCATOR.GetResourceManager()->GetMaterialName(m_pMaterial);
        };
    m_getters["shader"] = [this]() -> std::any {
        return SERVICE_LOCATOR.GetResourceManager()->GetShaderName(m_pMaterial->GetShader());
        };
    m_getters["geometry"] = [this]() -> std::any {
        return SERVICE_LOCATOR.GetResourceManager()->GetGeometryName(m_pGeometry);
        };
    m_getters["color"] = [this]() -> std::any { return m_pMaterial->GetColor(); };
    m_getters["uvType"] = [this]() -> std::any { return m_pGeometry->GetUVType(); };
}