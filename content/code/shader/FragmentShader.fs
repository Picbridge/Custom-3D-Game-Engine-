#version 400

#define PI 3.141592

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
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform bool hasDiffuse; 
uniform bool hasSpecular; 
uniform mat4 localModel;
uniform int textureType;
uniform int DebugNormal;
uniform sampler2D shadowMap;

vec2 Cube(vec3 FragPos);
vec2 Planar(vec3 FragPos);
vec2 Cylindrical(vec3 FragPos);
vec2 Spherical(vec3 FragPos);
vec2 RotateUV(vec2 uv, mat4 matrix);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    // Initialize uv with passed TexCoords
    vec2 uv = TexCoords;

    // Depending on textureType, select the appropriate UV mapping function
    // Commented out for now, UV calculation is done in CPU
    // if (textureType == 0)
    //     uv = Planar(FragPos);
    // else if (textureType == 1)
    //     uv = Cylindrical(FragPos);
    // else if (textureType == 2)
    //     uv = Spherical(FragPos);
    // else if (textureType == 3)
    //     uv = Cube(FragPos);

    // Rotate the UVs along with the object
    //uv = RotateUV(uv, localModel);
    
    // Normalize the incoming normal and view direction
    vec3 norm = normalize(Normal);
    vec3 viewDirNormalized = normalize(viewDir);

    // Calculate light direction (light to fragment)
    vec3 lightDir = normalize(light.position - FragPos);

    // ---- Ambient Lighting ----
    vec3 ambient = (light.ambient * material.color);

    // ---- Diffuse Lighting (Lambertian reflectance) ----
    vec3 diffuse = vec3(0.f); // Initialize to zero
    float diff = max(dot(norm, lightDir), 0.f);
    if (hasDiffuse) 
        diffuse = diff * light.diffuse * vec3(texture(material.diffuse, uv));
    else 
        diffuse = diff * light.diffuse * material.color;


    // ---- Specular Lighting (Phong reflection model) ----
    vec3 specular = vec3(0.f); // Initialize to zero
    if (hasSpecular) 
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDirNormalized, reflectDir), 0.f), material.shininess);
        specular = spec * light.specular * vec3(texture(material.specular, uv));
    }
    float shadow = ShadowCalculation(FragPosLightSpace);
    // Combine all the lighting components
    vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * material.color;    
    
    if (DebugNormal == 1)
        finalColor = norm;
    // Set the final color output
    FragColor = vec4(finalColor, 1.f);
}

// Cube UV Mapping
vec2 Cube(vec3 FragPos)
{
    vec2 uv;
    float absX = abs(FragPos.x);
    float absY = abs(FragPos.y);
    float absZ = abs(FragPos.z);

    // Determine the major axis direction
    if (absX >= absY && absX >= absZ)
    {
        uv.x = (FragPos.x > 0) ? -FragPos.z / absX : FragPos.z / absX;
        uv.y = FragPos.y / absX;
    }
    else if (absY >= absX && absY >= absZ)
    {
        uv.x = FragPos.x / absY;
        uv.y = (FragPos.y > 0) ? -FragPos.z / absY : FragPos.z / absY;
    }
    else
    {
        uv.x = (FragPos.z > 0) ? FragPos.x / absZ : -FragPos.x / absZ;
        uv.y = FragPos.y / absZ;
    }

    // Normalize UV to [0, 1] range
    uv.x = (uv.x + 1.f) / 2.f;
    uv.y = (uv.y + 1.f) / 2.f;

    return uv;
}

// Planar UV Mapping
vec2 Planar(vec3 FragPos)
{
    vec2 uv;
    uv.x = -FragPos.z;
    uv.y = FragPos.y;
    return uv;
}

// Cylindrical UV Mapping
vec2 Cylindrical(vec3 FragPos)
{
    float r = sqrt(FragPos.x * FragPos.x + FragPos.y * FragPos.y);
    float theta = atan(FragPos.y, FragPos.x);
    vec2 uv;
    uv.x = (theta + PI) / (2.f * PI);
    uv.y = FragPos.z;
    return uv;
}

// Spherical UV Mapping
vec2 Spherical(vec3 FragPos)
{
    float r = sqrt(FragPos.x * FragPos.x + FragPos.y * FragPos.y + FragPos.z * FragPos.z);
    float theta = atan(FragPos.y, FragPos.x);
    float phi = acos(FragPos.z / r);
    vec2 uv;
    uv.x = (theta + PI) / (2.f * PI);
    uv.y = phi / PI;
    return uv;
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