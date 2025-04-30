#include "pch.h"
#include "ObjLoader.h"

bool ObjLoader::LoadObj(const char* path, VERTEX_DATA& vertexData, NORMAL_DATA& normalData, UV_INFO& uv_info)
{
    // Clear output buffers
    vertexData.vertex_buffer.clear();
    vertexData.index_buffer.clear();
    normalData.center.clear();
    normalData.face_normal_buffer.clear();
    normalData.vertex_normal_buffer.clear();
    uv_info.Cylindrical.clear();
    uv_info.Spherical.clear();
    uv_info.Planar.clear();
    uv_info.Cube.clear();

    // Temporary storage for file data
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;

    // For faces: we store the indices read from the file.
    std::vector<unsigned int> positionIndices;
    std::vector<unsigned int> texcoordIndices;

    // For centering/normalizing model
    glm::vec3 pos_min(std::numeric_limits<float>::max());
    glm::vec3 pos_max(-std::numeric_limits<float>::max());

    // Read the OBJ file
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::istringstream ss(line);
        std::string header;
        ss >> header;

        if (header == "v")  // Vertex
        {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_positions.push_back(vertex);

            pos_max.x = std::max(pos_max.x, vertex.x);
            pos_max.y = std::max(pos_max.y, vertex.y);
            pos_max.z = std::max(pos_max.z, vertex.z);
            pos_min.x = std::min(pos_min.x, vertex.x);
            pos_min.y = std::min(pos_min.y, vertex.y);
            pos_min.z = std::min(pos_min.z, vertex.z);
        }
        else if (header == "vt")  // Texcoordinate
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_texcoords.push_back(uv);
        }
        else if (header == "vn")  // Normal (ignored)
        {
			// Calculate normals manually for consistent shading
            continue;
        }
        else if (header == "f") // Face
        {
            std::vector<std::string> tokens;
            std::string token;
            while (ss >> token)
                tokens.push_back(token);

            std::vector<unsigned int> face_pIndices;
            std::vector<unsigned int> face_tIndices;

            for (const auto& tok : tokens)
            {
                int pIndex, tIndex, nIndex;
                parseFaceToken(tok, pIndex, tIndex, nIndex);
                if (pIndex != 0)
                    pIndex = (pIndex > 0) ? pIndex - 1 : static_cast<int>(temp_positions.size()) + pIndex;
                if (tIndex != 0)
                    tIndex = (tIndex > 0) ? tIndex - 1 : static_cast<int>(temp_texcoords.size()) + tIndex;
                face_pIndices.push_back(static_cast<unsigned int>(pIndex));
                face_tIndices.push_back(static_cast<unsigned int>(tIndex));
            }

            // Triangulate the face if it has more than 3 vertices
            for (size_t i = 1; i + 1 < face_pIndices.size(); ++i)
            {
                positionIndices.push_back(face_pIndices[0]);
                positionIndices.push_back(face_pIndices[i]);
                positionIndices.push_back(face_pIndices[i + 1]);

                if (!temp_texcoords.empty())
                {
                    texcoordIndices.push_back(face_tIndices[0]);
                    texcoordIndices.push_back(face_tIndices[i]);
                    texcoordIndices.push_back(face_tIndices[i + 1]);
                }
                else
                {
                    texcoordIndices.push_back(0);
                    texcoordIndices.push_back(0);
                    texcoordIndices.push_back(0);
                }
            }
        }
    }
    file.close();

    // Center and normalize model
    glm::vec3 center = (pos_max + pos_min) / 2.f;
    float ABSMax = -std::numeric_limits<float>::max();
    for (auto& vertex : temp_positions)
    {
        vertex -= center;
        ABSMax = std::max(ABSMax, std::max({ std::abs(vertex.x), std::abs(vertex.y), std::abs(vertex.z) }));
    }
    if (ABSMax != 0)
        for (auto& vertex : temp_positions)
            vertex /= ABSMax;

    // Precompute UVs for vertices if texture coordinates are not provided
    std::vector<glm::vec2> precomputed_uvs;
    if (temp_texcoords.empty())
    {
        precomputed_uvs.resize(temp_positions.size());
        for (size_t i = 0; i < temp_positions.size(); ++i)
        {
            const glm::vec3& vertex = temp_positions[i];
            glm::vec2 uv;
            uv.x = -vertex.x;
            uv.y = vertex.y;
            precomputed_uvs[i] = uv;
        }
    }

    // Build final arrays
    struct VertexKey {
        unsigned int posIndex;
        unsigned int texIndex;
        bool operator<(const VertexKey& other) const {
            if (posIndex != other.posIndex) return posIndex < other.posIndex;
            return texIndex < other.texIndex;
        }
    };

    std::map<VertexKey, unsigned int> vertexMap;
    std::vector<glm::vec3> final_positions;
    std::vector<glm::vec2> final_uvs;
    std::vector<unsigned int> final_indices;

    for (size_t i = 0; i < positionIndices.size(); ++i)
    {
        VertexKey key = { positionIndices[i], texcoordIndices[i] };
        auto it = vertexMap.find(key);
        unsigned int index;

        if (it != vertexMap.end())
        {
            index = it->second;
        }
        else
        {
            index = static_cast<unsigned int>(final_positions.size());
            vertexMap[key] = index;

            final_positions.push_back(temp_positions[key.posIndex]);
            if (!temp_texcoords.empty() && key.texIndex < temp_texcoords.size())
                final_uvs.push_back(temp_texcoords[key.texIndex]);
            else
                final_uvs.push_back(precomputed_uvs[key.posIndex]);
        }

        final_indices.push_back(index);
    }

    // Set vertex buffer and index buffer
    vertexData.vertex_buffer = std::move(final_positions);
    vertexData.index_buffer = std::move(final_indices);
    uv_info.Planar = std::move(final_uvs);

    // Compute face normals
    findFaceNormal(vertexData, normalData);
    // Compute vertex normals based on shading mode
    normalData.vertex_normal_buffer.resize(vertexData.vertex_buffer.size());
    findVertexNormal(vertexData, normalData);

    std::cout << "Loaded OBJ file " << path << "...\n";
    return true;
}

void ObjLoader::findFaceNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData)
{
    normalData.center.clear();
    normalData.face_normal_buffer.clear();
    normalData.face_normal_buffer.reserve(vertexData.index_buffer.size() / 3);

    for (size_t i = 0; i < vertexData.index_buffer.size(); i += 3)
    {
        const glm::vec3& v0 = vertexData.vertex_buffer[vertexData.index_buffer[i]];
        const glm::vec3& v1 = vertexData.vertex_buffer[vertexData.index_buffer[i + 1]];
        const glm::vec3& v2 = vertexData.vertex_buffer[vertexData.index_buffer[i + 2]];

        normalData.center.push_back((v0 + v1 + v2) / 3.f);
        glm::vec3 faceNormal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        normalData.face_normal_buffer.push_back(faceNormal);
    }
}

void ObjLoader::findVertexNormal(VERTEX_DATA& vertexData, NORMAL_DATA& normalData)
{
    std::vector<glm::vec3>& normals = normalData.vertex_normal_buffer;
    std::vector<float> weights(vertexData.vertex_buffer.size(), 0.f);

    for (size_t i = 0; i < vertexData.index_buffer.size(); i += 3)
    {
        int v0 = vertexData.index_buffer[i];
        int v1 = vertexData.index_buffer[i + 1];
        int v2 = vertexData.index_buffer[i + 2];
        const glm::vec3& faceNormal = normalData.face_normal_buffer[i / 3];

        normals[v0] += faceNormal;
        normals[v1] += faceNormal;
        normals[v2] += faceNormal;
        weights[v0] += 1.f;
        weights[v1] += 1.f;
        weights[v2] += 1.f;
    }

    for (size_t i = 0; i < normals.size(); ++i)
    {
        if (weights[i] > 0.f)
        {
            normals[i] /= weights[i];
            normals[i] = glm::normalize(normals[i]);
        }
    }
}

void ObjLoader::parseFaceToken(const std::string& token, int& pIndex, int& tIndex, int& nIndex)
{
    pIndex = tIndex = nIndex = 0;
    size_t firstSlash = token.find('/');
    if (firstSlash == std::string::npos)
    {
        pIndex = std::stoi(token);
    }
    else
    {
        pIndex = std::stoi(token.substr(0, firstSlash));
        size_t secondSlash = token.find('/', firstSlash + 1);
        if (secondSlash != std::string::npos)
        {
            std::string tex = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            std::string norm = token.substr(secondSlash + 1);
            if (!tex.empty())
                tIndex = std::stoi(tex);
            if (!norm.empty())
                nIndex = std::stoi(norm);
        }
        else
        {
            std::string tex = token.substr(firstSlash + 1);
            if (!tex.empty())
                tIndex = std::stoi(tex);
        }
    }
}