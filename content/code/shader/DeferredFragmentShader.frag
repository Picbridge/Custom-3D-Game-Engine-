#version 400
#define PI 3.141592

out vec4 FragColor[];

struct Material {
    vec3 color;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;     // Light position in world space
    vec3 ambient;      // Ambient color of the light
    vec3 diffuse;      // Diffuse color of the light
    vec3 specular;     // Specular color of the light
};

in vec3 FragPos;
in vec3 viewDir;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform bool hasDiffuse; 
uniform bool hasSpecular; 
uniform mat4 localModel;
uniform int textureType;

vec2 RotateUV(vec2 uv, mat4 matrix);

void main()
{
    // Initialize uv with passed TexCoords
    vec2 uv = TexCoords;

    // Rotate the UVs along with the object
    uv = RotateUV(uv, localModel);
    
    // Normalize the incoming normal and view direction
    vec3 norm = normalize(Normal);
    vec3 viewDirNormalized = normalize(viewDir);

    // Calculate light direction (light to fragment)
    vec3 lightDir = normalize(light.position - FragPos);

    // ---- Ambient Lighting ----
    vec3 ambient = (light.ambient * material.color);
    
    // Set the final color output
    FragColor[0].xyz = FragPos;
    FragColor[1].xyz = norm;
    if (hasDiffuse) 
        FragColor[2].xyz = vec3(texture(material.diffuse, uv));
    else 
        FragColor[2].xyz = material.color;
    if (hasSpecular) 
        FragColor[3].xyz = vec3(texture(material.specular, uv));
}

vec2 RotateUV(vec2 uv, mat4 matrix)
{
    vec2 res;

    mat4 inverseMatrix = inverse(matrix);

    vec4 rotatedUV = vec4(uv.x, uv.y, 0.f, 1.f); // Extend UV to vec4
    rotatedUV = inverseMatrix * rotatedUV; 

    res.x = rotatedUV.x;
    res.y = rotatedUV.y;

    return res;
}