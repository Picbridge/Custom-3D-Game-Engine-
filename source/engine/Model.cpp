#include "pch.h"
#include "Model.h"
#include "Animation.h"
#include "resourcemanager/ResourceManager.h"
#include "ServiceLocator.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//const float PI = 3.14159f;
//const float rad = PI / 180.0f;

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

Model::~Model()
{
    for (auto anim : animations)
    {
        delete anim;
    }
    animations.clear();
}

// Draw Mesh, no bone weight implementation
void Model::Draw(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::vec3 lightPos)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, projection, view, lightPos);
}

void Model::DrawSkeleton(glm::mat4 projection, glm::mat4 view, ModelNode* node, glm::mat4 parentTransform)
{
    glLineWidth(1.0f);
    float posScale = 1;
    glm::mat4 transform = parentTransform * node->transform;
    glm::vec4 position = glm::vec4(transform[3]);

    // Draw spheres at each joint (for visibility)
    /*if (depth != 0)
    {
        sphere->modelMatrix = transform;
        sphere->scale = glm::scale(glm::mat4(1), glm::vec3(.25, .25, .25));
        sphere->Draw(projection, view, glm::vec3(1, 1, 1));
    }*/

    // Iterate over children and draw line from parent to child
    for (ModelNode* child : node->children)
    {
        // Redundant but not impacting performance
        glm::mat4 childtransform = transform * child->transform;
        glm::vec4 childposition = glm::vec4(childtransform[3]);

        DrawLine(glm::vec3(position.x, position.y, position.z) / posScale, glm::vec3(childposition.x, childposition.y, childposition.z) / posScale, projection, view);
        
        DrawSkeleton(projection, view, child, transform);
    }
}

void Model::Update(float deltaTime)
{
    UpdateHierarchy(origin);
    animations[0]->Update(deltaTime);
}

// Travel through node hierarchy, find corresponding bone
// Apply transformation
void Model::UpdateHierarchy(ModelNode* node)
{
    for (const auto& pair : boneMap)
    {
        if (pair.second.animated && node->name == pair.first)
        {
            //node->transform = glm::translate(glm::mat4(1), boneMap[node->name].keyFrames[frame].v) * rot * glm::scale(glm::mat4(1), glm::vec3(1.0f) * boneMap[node->name].keyFrames[frame].s);
            node->transform = boneMap[node->name].transform;
            break;
        }
    }
    for (ModelNode* child : node->children)
    {
        UpdateHierarchy(child);
    }
}

void Model::loadModel(std::string path)
{
    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
    // Load dummy sphere used for joints, aesthetic only
    //sphere = new RenderComponent(BuildObj("resources/sphere.obj"));
    // Load asset file
    Assimp::Importer import;
import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PopulateArmatureData);
    // Load error handler
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));


    // Set up VAO and VBO for generic line renderer
    SetUpBuffers();
    // Travel through scene to build mesh, node hierarchy, and bone map
    
    // NOTE: Not sure how to handle unique_ptrs with structs like below
    // but because the struct itself is only using library data types
    // I'm assuming it's handling memory correctly.
    origin = new ModelNode;
    processNode(scene->mRootNode, scene, origin);

    // Loop through all animations in the given file
    for (unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        this->hasAnimation = true;
        Animation* animation = new Animation(this);

        animation->duration = scene->mAnimations[i]->mDuration;
        animation->ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;

        // Loop through every channel
        for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
        {
            std::string name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

            // Loop through every keyframe
            // Ideally this would be split to keep positions, rotations, and scales 
            // seperate, but this doesn't work well with my VQS logic, so I am 
            // assuming every bone has the same number of keyframes
            for (unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
            {
                // Create temps to dump to glm
                aiVector3D tempV = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue;
                aiQuaternion tempQ = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue;
                // Convert
                glm::vec3 vec = glm::vec3(tempV.x, tempV.y, tempV.z);
                Quaternion quat(tempQ.w, glm::vec3(tempQ.x, tempQ.y, tempQ.z));
                float scale = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
                // Put in bone VQS
                boneMap[name].keyFrames.push_back(VQS(vec, quat, scale));
                boneMap[name].keyTimes.push_back(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime);
                boneMap[name].animated = true;
                boneMap[name].numKeys = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
            }
        }

        animations.push_back(animation);
    }
}

// Recursively travel through scene, building node hierarchy in my Node class
void Model::processNode(aiNode* node, const aiScene* scene, ModelNode* myNode)
{
    myNode->name = node->mName.C_Str();
    myNode->mTransformation = node->mTransformation;
    myNode->transform = AssimpToGLM(node->mTransformation);

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ModelNode* newNode = new ModelNode();
        myNode->children.push_back(newNode);
        myNode->numChildren++;
        processNode(node->mChildren[i], scene, newNode);
    }
}

// Build Mesh and Bone Map, mostly for skin rendering
// Not fully implemented due to time constraints
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        const aiBone* bone = mesh->mBones[i];
        Bone b;
        b.name = bone->mName.C_Str();

        for (unsigned int j = 0; j < bone->mNumWeights; j++)
        {
            b.weights.push_back(bone->mWeights[j].mVertexId);
        }

        boneMap[b.name] = b;
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        MeshVertex vertex;

        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

//
std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            MeshTexture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

glm::mat4 Model::AssimpToGLM(const aiMatrix4x4& assimpMatrix)
{
    return glm::mat4(
        glm::vec4(assimpMatrix.a1, assimpMatrix.b1, assimpMatrix.c1, assimpMatrix.d1),
        glm::vec4(assimpMatrix.a2, assimpMatrix.b2, assimpMatrix.c2, assimpMatrix.d2),
        glm::vec4(assimpMatrix.a3, assimpMatrix.b3, assimpMatrix.c3, assimpMatrix.d3),
        glm::vec4(assimpMatrix.a4, assimpMatrix.b4, assimpMatrix.c4, assimpMatrix.d4)
    );
}

void Model::DrawLine(glm::vec3 startPoint, glm::vec3 endPoint, glm::mat4 projection, glm::mat4 view)
{
    shader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Line");
    shader->Use();
    shader->SetUniform("WorldProjection", projection);
    shader->SetUniform("WorldView", view);

    vertices[0] = startPoint;
    vertices[1] = endPoint;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2); // GL_LINES to draw a line between two vertices
    glBindVertexArray(0);

    //glDeleteVertexArrays(1, &vao);
    //glDeleteBuffers(1, &vbo);

    shader->Unuse();
    //shader.~ShaderProgram();
}

// Buffers and shader for line rendering
void Model::SetUpBuffers()
{
    //SERVICE_LOCATOR.GetResourceManager()->AddShader("Line", new Shader("../../content/code/shader/line.vert", "../../content/code/shader/line.frag"));
    //shader->AddShader(GL_VERTEX_SHADER, "line.vert");
    //shader->AddShader(GL_FRAGMENT_SHADER, "line.frag");
    //shader->AttachShaders();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}




// Load texture using stb_image
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


