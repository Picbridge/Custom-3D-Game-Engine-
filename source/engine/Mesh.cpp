#include "pch.h"


Mesh::Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
{
    vertices = _vertices;
    indices = _indices;
    textures = _textures;

    transform.position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    transform.rotation = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    transform.scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    setupMesh();
}

void Mesh::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos)
{
    glLineWidth(2);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.SetUniform(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    SetUpUniforms(shader, projection, view, lightPos);
    // draw mesh
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.Unuse();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::SetUpUniforms(Shader target, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos)
{
    glm::mat4 model;

    model = glm::mat4(1.0);

    //transform.rotation = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    //transform.position = glm::translate(glm::mat4(1.0f), sin((float)glfwGetTime()) * glm::vec3(0.0f, 0.0f, 0.0f));

    model = transform.position * transform.rotation * transform.scale;

    /*glm::vec4 position = projection * view * glm::vec4(model[3]);
    std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;*/

    target.Use();
    target.SetUniform("WorldProjection", projection);
    target.SetUniform("WorldView", view);
    target.SetUniform("WorldInverse", glm::inverse(view));
    target.SetUniform("Model", model);
    target.SetUniform("NormalTr", glm::inverse(model));
    target.SetUniform("lightPos", lightPos);
}

