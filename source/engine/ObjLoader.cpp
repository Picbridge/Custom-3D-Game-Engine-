#include "pch.h"
#include "ObjLoader.h"

bool ObjLoader::LoadObj(const char* path, VERTEX_DATA& vertexData, NORMAL_DATA& normalData, UV_INFO& uv_info)
{
    vertexData.vertex_buffer.clear();
    vertexData.index_buffer.clear();
    normalData.center.clear();
    normalData.face_normal_buffer.clear();

    glm::vec3 pos_min(std::numeric_limits<float>::max());
    glm::vec3 pos_max(-std::numeric_limits<float>::max());

    std::ifstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream ss(line);
        std::string header;
        ss >> header;

        if (header == "v") {  // Vertex
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertexData.vertex_buffer.push_back(vertex);

            pos_max.x = std::max(pos_max.x, vertex.x);
            pos_max.y = std::max(pos_max.y, vertex.y);
            pos_max.z = std::max(pos_max.z, vertex.z);
            pos_min.x = std::min(pos_min.x, vertex.x);
            pos_min.y = std::min(pos_min.y, vertex.y);
            pos_min.z = std::min(pos_min.z, vertex.z);

			// Compute UV coordinates on CPU
			findCylindericalSphericalUV(uv_info, vertex);
			findPlanerUV(uv_info, vertex);
			findCubeUV(uv_info, vertex);
        }
        else if (header == "f") // Face
        {  
            int vertexIndex[3];
            ss >> vertexIndex[0] >> vertexIndex[1] >> vertexIndex[2];
            vertexData.index_buffer.push_back(vertexIndex[0] - 1);
            vertexData.index_buffer.push_back(vertexIndex[1] - 1);
            vertexData.index_buffer.push_back(vertexIndex[2] - 1);
        }
    }

    // Center the model and normalize its size
    pos_max = (pos_max + pos_min) / 2.f;
    float ABSMax = -std::numeric_limits<float>::max();
    for (glm::vec3& vertex : vertexData.vertex_buffer) 
    {
        vertex -= pos_max;  // Centering the model
        ABSMax = std::max(ABSMax, std::max({ std::abs(vertex.x), std::abs(vertex.y), std::abs(vertex.z) }));
    }

	if (ABSMax != 0) // Avoid division by zero
        for (glm::vec3& vertex : vertexData.vertex_buffer)
            vertex /= ABSMax;  // Normalizing the size

    // Compute vertex normals
	findFaceNormal(vertexData, normalData);
    findVertexNormal(vertexData, normalData);
    file.close();

    std::cout << "Loaded OBJ file " << path << "...\n";
    return true;
}

void ObjLoader::findFaceNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData)
{
    std::vector<glm::vec3> vertex, v1, v2;

    for (unsigned int i = 0; i < vertexData.index_buffer.size(); ++i)
    {
        vertex.push_back(vertexData.vertex_buffer[vertexData.index_buffer[i]]);
    }
    for (unsigned int i = 0; i < vertex.size(); i += 3)
    {
        normalData.center.push_back(glm::vec3((vertex[i] + vertex[i + 1] + vertex[i + 2]) / 3.f));

        v1.push_back(vertex[i + 1] - vertex[i]);
        v2.push_back(vertex[i + 2] - vertex[i]);
    }
    for (unsigned int i = 0; i < v2.size(); ++i)
    {
        normalData.face_normal_buffer.push_back(glm::normalize(glm::cross(v1[i], v2[i])));
    }
}

void ObjLoader::findVertexNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData)
{
    normalData.vertex_normal_buffer.clear();
    normalData.vertex_normal_buffer.resize(vertexData.vertex_buffer.size());

    std::vector<std::vector<glm::vec3>> vertexNormals(vertexData.vertex_buffer.size());

    // Iterate over the index buffer and associate face normals with each vertex
    for (size_t i = 0; i < vertexData.index_buffer.size(); i += 3)
    {
        int v0 = vertexData.index_buffer[i];
        int v1 = vertexData.index_buffer[i + 1];
        int v2 = vertexData.index_buffer[i + 2];

        glm::vec3 faceNormal = normalData.face_normal_buffer[i / 3];

        // Add the face normal to each vertex's normal list if it's not already there
        auto& normalsV0 = vertexNormals[v0];
        if (std::find(normalsV0.begin(), normalsV0.end(), faceNormal) == normalsV0.end())
            normalsV0.push_back(faceNormal);

        auto& normalsV1 = vertexNormals[v1];
        if (std::find(normalsV1.begin(), normalsV1.end(), faceNormal) == normalsV1.end())
            normalsV1.push_back(faceNormal);

        auto& normalsV2 = vertexNormals[v2];
        if (std::find(normalsV2.begin(), normalsV2.end(), faceNormal) == normalsV2.end())
            normalsV2.push_back(faceNormal);
    }

    // Calculate the average normal for each vertex
    for (size_t i = 0; i < vertexData.vertex_buffer.size(); ++i)
    {
        glm::vec3 averageNormal(0.f);

        for (const auto& normal : vertexNormals[i])
            averageNormal += normal;

        if (!vertexNormals[i].empty())
            averageNormal /= static_cast<float>(vertexNormals[i].size());

        normalData.vertex_normal_buffer[i] = glm::normalize(averageNormal);
    }
}

void ObjLoader::findPlanerUV(UV_INFO& uv_info, glm::vec3 vertex)
{
	glm::vec2 uv;
	uv.x = -vertex.x;
	uv.y = vertex.y;
	uv_info.Planar.push_back(uv);
}

void ObjLoader::findCylindericalSphericalUV(UV_INFO& uv_info, glm::vec3 vertex)
{
	glm::vec2 uv;
	float r = vertex.x * vertex.x + vertex.y * vertex.y;
    float theta = (vertex.x != 0) ? std::atan(vertex.y / vertex.x) : (vertex.y > 0 ? PI / 2.f : -PI / 2.f);

    uv.x = (theta + PI) / (2.f * PI);
	uv.y = vertex.z;
	uv_info.Cylindrical.push_back(uv);

    r += vertex.z * vertex.z;
    float phi = std::acos(vertex.z / r);

    uv.y = phi / PI;
    uv_info.Spherical.push_back(uv);
}

void ObjLoader::findCubeUV(UV_INFO& uv_info, glm::vec3 vertex)
{
    glm::vec2 uv;

    float absX = std::abs(vertex.x);
    float absY = std::abs(vertex.y);
    float absZ = std::abs(vertex.z);

    // Determine the major axis direction
    if (absX >= absY && absX >= absZ)
    {
        //+X
        uv.x = (vertex.x > 0) ? -vertex.z / absX : vertex.z / absX;
        uv.y = vertex.y / absX;
    }
    else if (absY >= absX && absY >= absZ)
    {
        //+Y
        uv.x = vertex.x / absY;
        uv.y = (vertex.y > 0) ? -vertex.z / absY : vertex.z / absY;
    }
    else
    {
        //+Z
        uv.x = (vertex.z > 0) ? vertex.x / absZ : -vertex.x / absZ;
        uv.y = vertex.y / absZ;
    }

    // Normalize UV to [0, 1] range
    uv.x = (uv.x + 1.f) / 2.f;
    uv.y = (uv.y + 1.f) / 2.f;

    uv_info.Cube.push_back(uv);
}