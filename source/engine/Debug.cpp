#include "pch.h"
#include "Debug.h"
#include "cameramanager/CameraComponent.h"
#include "cameramanager/CameraManager.h"
#include "resourcemanager/ResourceManager.h"
#include "VectorCalculations.h"

void Debug::DrawRay(glm::vec3 origin, glm::vec3 direction, float length, glm::vec3 color, float width)
{
    GLuint vao, vbo;
    glm::vec3 vertices[2] = { glm::vec3(0) };
    Shader* shader;

    // Set up projection and view matrices
    const auto camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
    const auto& projection = camera->GetProjectionMatrix();
    const auto& view = camera->GetViewMatrix();
    
    glLineWidth(width);

    // VAO VBO Setup
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set Up shader and uniforms
    shader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Line");
    shader->Use();
	shader->PassShaderData("WorldProjection", projection, "WorldView", view, "color", color);
    // Repopulate vertices and update vbo
    glm::vec3 endPoint = origin + direction * length;
    vertices[0] = origin;
    vertices[1] = endPoint;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Draw
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2); // GL_LINES to draw a line between two vertices
    glBindVertexArray(0);

    // Cleanup, Isle: my pants
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    shader->Unuse();
}

void Debug::DrawSphere(glm::dvec3 pos, glm::vec3 color, float radius)
{
    glLineWidth(1.0f);
    ResourceManager* manager = SERVICE_LOCATOR.GetResourceManager();
    Shader* shader = manager->GetShader("Debug");
    Geometry* geometry = manager->GetGeometry("Sphere");
    CameraComponent* camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos));
    model = glm::scale(model, glm::vec3(radius));

    shader->Use();
    geometry->Bind(shader);
	shader->PassShaderData("model", model, "view", camera->GetViewMatrix(), "projection", camera->GetProjectionMatrix(), "color", color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    geometry->Render();
    geometry->Unbind();
    shader->Unuse();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Debug::DrawReticle()
{
    CameraComponent* camera = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
    auto view = camera->GetViewMatrix();
}
