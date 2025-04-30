#version 400

in vec3 FragPos;     
in vec3 Normal;      
in vec2 TexCoords;   
in vec2 ScreenCoords;

out vec4 FragColor; // Final fragment output

// Scene color texture from prepass
uniform sampler2D prepassTexture;
uniform vec2 screenResolution;

// Light and camera properties
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float ambient;

// Cel-shading controls
uniform float Shades;       
uniform float Smoothness;   
uniform float RimThreshold; 

uniform float EdgeDiffuse;             
uniform float EdgeSpecular;            
uniform float EdgeSpecularOffset;      
uniform float EdgeDistanceAttenuation; 
uniform float EdgeShadowAttenuation;   
uniform float EdgeRim;                 
uniform float EdgeRimOffset;           

// Outline/edge thresholds
//uniform float depthThreshold;   
//uniform float normalThreshold;  
//uniform float normalEdgeBias;   
//uniform float outlineScale;     

//------------------------------------------------------
// Utility functions
float saturate(float x) {
    return clamp(x, 0.0, 1.0);
}

float Toon(float intensity, float levels) {
    float i = saturate(intensity);
    return floor(i * levels) / levels;
}

//------------------------------------------------------
// Cel-shading calculation
float CalculateCelShading(
    vec3 normal,
    vec3 viewDir,
    vec3 lightDir,
    float shadowAtten,
    float distanceAtten,
    float shades,
    float smoothness,
    float rimThreshold
)
{
    float attenuation = distanceAtten * shadowAtten;
    
    // Diffuse
    float diffuse = Toon(dot(normalize(normal), normalize(lightDir)), shades);
    diffuse *= attenuation;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(reflectDir, viewDir), 0.0), exp2(10.0 * smoothness + 1.0));
    specular *= diffuse * smoothness;
    
    // Rim lighting
    float rim = 1.0 - dot(normalize(viewDir), normalize(normal));
    rim = pow(rim, rimThreshold);
    rim *= diffuse * smoothness;
    
    diffuse  = saturate(diffuse / EdgeDiffuse);
    specular = saturate((specular - EdgeSpecularOffset) / EdgeSpecular);
    rim      = saturate((rim - EdgeRimOffset) / EdgeRim);
    
    float final = diffuse + max(specular, rim);
    return saturate(final);
}

//------------------------------------------------------
void main()
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(viewPos - FragPos);

    float shadowFactor   = EdgeShadowAttenuation;    
    float distanceFactor = EdgeDistanceAttenuation;

    float celFactor = CalculateCelShading(
        N, V, L,
        shadowFactor,
        distanceFactor,
        Shades,
        Smoothness,
        RimThreshold
    );

    vec2 screenTexCoords = clamp(gl_FragCoord.xy / screenResolution, 0.0, 1.0);
    vec3 baseColor = texture(prepassTexture, screenTexCoords).rgb;
    float alpha = texture(prepassTexture, screenTexCoords).a;
    vec3 ambientColor = baseColor * ambient;

    vec3 litColor = baseColor * lightColor * celFactor;
    vec3 finalColor = ambientColor + litColor;

    //Removed outline due to low poly. It looks crashed

    //float depthGradient = length(vec2(dFdx(gl_FragCoord.z), dFdy(gl_FragCoord.z)));
    //// normal gradient
    //vec3 gradX = dFdx(N);
    //vec3 gradY = dFdy(N);

    //float normalGradient = (length(gradX) + length(gradY)) * normalEdgeBias;

    //float scaledDepthThreshold   = depthThreshold   / outlineScale;
    //float scaledNormalThreshold  = normalThreshold  / outlineScale;

    //float depthEdge  = (depthGradient  > scaledDepthThreshold)  ? 1.0 : 0.0;
    //float normalEdge = (normalGradient > scaledNormalThreshold) ? 1.0 : 0.0;

    //// black if depth edge, white if normal edge.
    //if (depthEdge > 0.5 && normalEdge > 0.5) {
    //    finalColor = vec3(0.0);
    //}
    //else if (depthEdge > 0.5) {
    //    finalColor = vec3(0.0);
    //}
    //else if (normalEdge > 0.5) {
    //    finalColor = vec3(1.0);
    //}
    
    FragColor = vec4(finalColor, alpha);

}
