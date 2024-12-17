#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;      // Fragment position in world space
out vec3 Normal;       // Normal in world space
out vec2 TexCoords;    // Texture coordinates
out vec3 viewDir;      // View direction

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;  // Camera position passed from application

void main()
{
    // Calculate the world position of the fragment
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calculate the normal in world space using the model's transformation
    Normal = normalize(mat3(model) * aNormal);
    
    // View direction (from fragment position to the camera position)
    viewDir = viewPos - FragPos;  // Calculate direction only

    // Pass texture coordinates
    TexCoords = aTexCoords;

    // Final vertex position in screen space
    gl_Position = projection * view * vec4(FragPos, 1.0);

}
