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

	//@brief Compute the UV coordinates - Planar
	//@param uv_info : The UV information
	//@param vertex : The vertex to compute the UV coordinates
	static void findPlanerUV(UV_INFO& uv_info, glm::vec3 vertex);

	//@brief Compute the UV coordinates - Cylindrical + Spherical
	//@param uv_info : The UV information
	//@param vertex : The vertex to compute the UV coordinates
	static void findCylindericalSphericalUV(UV_INFO& uv_info, glm::vec3 vertex);

	//@brief Compute the UV coordinates - Cube
	//@param uv_info : The UV information
	//@param vertex : The vertex to compute the UV coordinates
	static void findCubeUV(UV_INFO& uv_info, glm::vec3 vertex);
};