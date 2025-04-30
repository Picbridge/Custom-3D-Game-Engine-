#pragma once
class ObjLoader
{
public:
    //@brief Load the OBJ file and store the vertices, indices, normals, and UV coordinates
    static bool LoadObj(const char* path, VERTEX_DATA& vertexData, NORMAL_DATA& normalData, UV_INFO& uv_info);

private:
	//@brief Compute the face normal
	static void findFaceNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData);
    
    //@brief Compute the vertex normal
    static void findVertexNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData);

	//@brief Parse the face token
	//@param token : The token to parse
	//@param pIndex : The position index
	//@param tIndex : The texture index
	//@param nIndex : The normal index
	static void parseFaceToken(const std::string& token, int& pIndex, int& tIndex, int& nIndex);
};