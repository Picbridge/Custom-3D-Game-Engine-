#version 400

#define PI 3.141592

uniform sampler2D shadowMap;
uniform float alpha;
struct Material {
    vec3 color;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float alpha;
};

struct Light {
    vec3 position;     // Light position in world space
    vec3 ambient;      // Ambient color of the light
    vec3 diffuse;      // Diffuse color of the light
    vec3 specular;     // Specular color of the light
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform bool hasDiffuse; 
uniform bool hasSpecular; 
uniform int DebugNormal;
uniform vec3 viewPos;  // Camera position passed from application

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.0005; // Small bias to avoid shadow acne
    //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;


    // Ensure shadow only applies if inside the light frustum
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    // Initialize uv with passed TexCoords
    vec2 uv = TexCoords;
    vec3 color;
    if (hasDiffuse)
        color = vec3(texture(material.diffuse, uv));
    else
        color = material.color;
    
    // Normalize the incoming normal and view direction
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate light direction (light to fragment)
    vec3 lightDir = normalize(light.position - FragPos);

    // ---- Ambient Lighting ----
    vec3 ambient = (light.ambient);

    // ---- Diffuse Lighting (Lambertian reflectance) ----
    vec3 diffuse = vec3(0.f); // Initialize to zero
    float diff = max(dot(norm, lightDir), 0.f);
    diffuse = diff * light.diffuse;


    // ---- Specular Lighting (Phong reflection model) ----
    vec3 specular = vec3(0.f); // Initialize to zero
    if (hasSpecular) 
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
        specular = spec * light.specular;
    }
    float shadow = ShadowCalculation(FragPosLightSpace);
    // Combine all the lighting components
    //float shadow = ShadowCalculation(FragPosLightSpace);  
    vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    if (DebugNormal == 1)
        finalColor = norm;

    if(material.alpha < 0.001)
        discard;
        
    // Set the final color output
    FragColor = vec4(finalColor, material.alpha * alpha);
}