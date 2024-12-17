#pragma once
class Material;

class Geometry
{
public:
    Geometry();
    Geometry(const char* path);
    ~Geometry();

    //@brief Load the geometry data from the OBJ file
    bool LoadGeometry(const char* path);

    //@brief Bind the vertex data to the buffer
	//@param material : Material to fetch shader attribute locations
    void Bind(Shader* shader);

    //@brief Unbind the vertex data from the buffer
    void Unbind();

    //@brief Clean up the buffer
    void CleanUpBuffers();

    //@brief Render the geometry
    void Render();

    void SetUVType(UV_TYPE type);

    //--------------------------------
    //Getters
    //--------------------------------
    
	//@brief Returns uv type as int
	//@return UV_TYPE : uv type
    UV_TYPE GetUVType()
    {
        return m_uvType;
    }

protected:
    UV_TYPE m_uvType;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_IBO;
    GLuint m_UV;
    GLuint m_NORMALBUFFER;

    VERTEX_DATA m_vertexData;
	NORMAL_DATA m_normalData;
    UV_INFO m_uvInfo;

	//@brief Generate buffers for the geometry on creation
    void genBuffers();
};