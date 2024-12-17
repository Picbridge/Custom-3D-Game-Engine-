#pragma once
class FBO {
public:
    unsigned int fboID;
    unsigned int textureIDs[4];
    int width, height;  // Size of the texture.

    void CreateFBO(const int w, const int h);

    // Bind this FBO to receive the output of the graphics pipeline.
    void BindFBO();

    // Unbind this FBO from the graphics pipeline;  graphics goes to screen by default.
    void UnbindFBO();

    // Bind this FBO's texture to a texture unit.
    void BindTexture(const int unit, const int programId, const std::string& name);

    // Unbind this FBO's texture from a texture unit.
    void UnbindTexture(const int unit);

    // Bind multiple textures (for deferred shading)
    void BindTextures(const int unit, const int programId, const std::string& name0, const std::string& name1, const std::string& name2, const std::string& name3);
};

