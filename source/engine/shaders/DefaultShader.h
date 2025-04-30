#pragma once
class DefaultShader : public Shader  
{  
public:  
   DefaultShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)  
       : Shader(vertexPath, fragmentPath, geometryPath) {}  

   void PassShaderData() override;
};
