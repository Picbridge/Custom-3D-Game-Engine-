#include "pch.h"
#include "headers.h"
#include "RenderComponent.h"
#include "Model.h"
#include "scenemanager/SceneManager.h"
#include "LightComponent.h"

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

RenderComponent::RenderComponent() : m_pGeometry(nullptr), m_pMaterial(nullptr), m_pModel(nullptr), m_pPrepassShader(nullptr),
    m_texTilingPos(0.0f, 0.0f), m_texTilingScale(1.0f, 1.0f), m_currentWidth(0), m_currentHeight(0), m_prepassFBO(0),
    m_prepassColorTex(0), m_depthBuffer(0), m_LineVAO(0), m_LineVBO(0), m_LineVertices{ glm::vec3(0.0f), glm::vec3(0.0f) },
    m_prepassColor(0.0f, 0.0f, 0.0f, 1.0f)
{
    m_pPrepassShader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Default");
    defineMember();
}

RenderComponent::~RenderComponent()
{
    Shutdown();
}

void RenderComponent::Init()
{

    auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
    m_currentWidth = frameBuffer.Width;
    m_currentHeight = frameBuffer.Height;

    // Create full-window FBO
    glGenFramebuffers(1, &m_prepassFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_prepassFBO);

    // Color texture
    glGenTextures(1, &m_prepassColorTex);
    glBindTexture(GL_TEXTURE_2D, m_prepassColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_currentWidth, m_currentHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_prepassColorTex, 0);

    // Depth renderbuffer
    glGenRenderbuffers(1, &m_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_currentWidth, m_currentHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Prepass FBO incomplete: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Initialize line VAO and VBO (unchanged)
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

void RenderComponent::Prepass(const CameraComponent* camera)
{
    // Set polygon mode based on UI state
    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Wireframes"))
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (!m_pMaterial || !m_pGeometry)
    {
        std::cerr << "RenderComponent::Render() - Material or Geometry not set" << std::endl;
        return;
    }
    //---------------------------
    // Prepass: Render scene to FBO (render-to-texture)
    //---------------------------
    // Bind the prepass framebuffer and set the viewport accordingly
    auto curViewport = camera->GetViewport();

    if (curViewport.W != m_currentWidth || curViewport.H != m_currentHeight)
    {
        m_currentWidth = curViewport.W;
        m_currentHeight = curViewport.H;
        // Resize color texture:
        glBindTexture(GL_TEXTURE_2D, m_prepassColorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_currentWidth, m_currentHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Resize depth renderbuffer:
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_currentWidth, m_currentHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_prepassFBO);
    glViewport(curViewport.X, curViewport.Y, curViewport.W, curViewport.H);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    LightComponent* light = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetLight();
    // Use prepass shader and render geometry
    m_pPrepassShader->Use();
    m_pGeometry->Bind(m_pPrepassShader);

    if (light != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->GetDepthMap());
    }

    m_pPrepassShader->PassObject(GetOwner());
    m_pPrepassShader->PassLight(light);
    m_pPrepassShader->PassMaterial(m_pMaterial);
    m_pPrepassShader->PassShaderData();
    m_pPrepassShader->PassShaderData("tilePosition", m_texTilingPos, "tileScale", m_texTilingScale);

    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Colliders"))
        m_pPrepassShader->PassShaderData("alpha", 0.5f);
    else
        m_pPrepassShader->PassShaderData("alpha", 1.0f);

    m_pMaterial->Bind();
    m_pGeometry->Render();

    m_pGeometry->Unbind();
    m_pMaterial->Unbind();
    m_pPrepassShader->Unuse();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // draw colliders and velocities if enabled
    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Colliders"))
        DrawCollider();
    if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("Velocities"))
        DrawVelocity();
}

void RenderComponent::Render()
{
    //---------------------------
    // Custom Pass: Render scene using custom shader with prepass texture as base color
    //---------------------------
    LightComponent* light = SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->GetLight();
    auto customShader = m_pMaterial->GetShader();
    customShader->Use();
    m_pGeometry->Bind(customShader);

    if (light != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->GetDepthMap());
    }

    // pass prepass texture to shader only if the custom shader is intended to use it
    if (customShader->HasUniform("prepassTexture"))
    {

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_prepassColorTex);
        customShader->PassShaderData("prepassTexture", 1, "screenResolution", glm::vec2(m_currentWidth, m_currentHeight));
    }

    customShader->PassObject(GetOwner());
    customShader->PassLight(light);
    customShader->PassMaterial(m_pMaterial);
    customShader->PassShaderData();

    m_pMaterial->Bind();
    m_pGeometry->Render();
    m_pGeometry->Unbind();
    m_pMaterial->Unbind();
    customShader->Unuse();
}



void RenderComponent::Render(Shader* shader)
{
    if (!m_pMaterial || !m_pGeometry)
        std::cerr << "RenderComponent::Render() - Material or Geometry not set" << std::endl;

    //shader->Use();
    m_pGeometry->Bind(shader);

    shader->PassShaderData("model", GetOwner()->GetWorldTransform());
    //shader->PassObject(GetOwner());
    //m_pMaterial->Bind();
    m_pGeometry->Render();
    m_pGeometry->Unbind();
    //m_pMaterial->Unbind();
    //shader->Unuse();

}

void RenderComponent::DrawCollider()
{

}

void RenderComponent::DrawVelocity()
{
    PhysicsComponent* physics = GetOwner()->GetComponent<PhysicsComponent>();
    if (physics)
    {
        Transform* transform = GetOwner()->GetTransform();
        Shader* shader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Line");
        shader->Use();
        shader->PassShaderData("WorldProjection", transform->GetProjection(), "WorldView", transform->GetView());

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
    if (!m_pMaterial) return;
    m_pMaterial->SetColor(color);
}

void RenderComponent::SetColor(float r, float g, float b)
{
    if (!m_pMaterial) return;
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

void RenderComponent::SetGeometry(const std::string& geometryName)
{
    m_pGeometry = SERVICE_LOCATOR.GetResourceManager()->GetGeometry(geometryName);
}

void RenderComponent::SetUVType(UV_TYPE type)
{
    if (!m_pGeometry) return;
    m_pGeometry->SetUVType(type);
}

void RenderComponent::defineMember()
{
    m_setters["material"] = [this](std::any value) { SetMaterial(std::any_cast<const std::string&>(value)); };
    m_setters["geometry"] = [this](std::any value) { SetGeometry(std::any_cast<const std::string&>(value)); };
    m_setters["textureTilingPos"] = [this](std::any value) { SetTexTilingPos(std::any_cast<glm::vec2>(value)); };
    m_setters["textureTilingScale"] = [this](std::any value) { SetTexTilingScale(std::any_cast<glm::vec2>(value)); };

    m_getters["material"] = [this]() -> std::any {
        return SERVICE_LOCATOR.GetResourceManager()->GetMaterialName(m_pMaterial);
        };
    m_getters["geometry"] = [this]() -> std::any {
        return SERVICE_LOCATOR.GetResourceManager()->GetGeometryName(m_pGeometry);
        };
    m_getters["textureTilingPos"] = [this]() -> std::any { return GetTexTilingPos(); };
    m_getters["textureTilingScale"] = [this]() -> std::any { return GetTexTilingScale(); };
}