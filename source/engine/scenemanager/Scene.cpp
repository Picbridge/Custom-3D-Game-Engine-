#include "../pch.h"
#include "../objectmanager/GameObjectFactory.h"
#include "../resourcemanager/ResourceManager.h"
#include "../ui/UI.h"

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

Scene::~Scene()
{
	Shutdown();
}

void Scene::Init()
{
	FILE* fp;
	fopen_s(&fp, m_sceneSource.c_str(), "rb");
	if (!fp)
	{
		std::cout << "Failed to load data source" << std::endl;
		exit(EXIT_FAILURE);
	}

	char readBuffer[8192];
	rapidjson::FileReadStream inputStream(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document sceneDoc;
	sceneDoc.ParseStream(inputStream);
	fclose(fp);

	if (sceneDoc.HasParseError())
		std::cerr << "Error parsing the scene json file" << std::endl;

	if (sceneDoc.IsObject() && sceneDoc.MemberCount() > 0)
	{
		auto scene = sceneDoc.MemberBegin();
		const std::string sceneName = scene->name.GetString();
		const rapidjson::Value& sceneData = scene->value;
		if (sceneData.IsNull() || sceneData.ObjectEmpty())
			return;

		const rapidjson::Value& gameObjects = sceneData.FindMember("GameObject")->value;

		if (!gameObjects.IsNull() && !gameObjects.ObjectEmpty())
		{
			// Should load all the assets for the scene from json
			SERVICE_LOCATOR.GetGameObjectFactory()->CreateAllGameObjects(gameObjects);

			// Need to initialize the skybox
			//TODO: Skybox path should be included to the json file and stored in resource manager. 
			//Skybox image should be loaded from the resource manager by name(string)
			const rapidjson::Value& skybox = sceneData.FindMember("Skybox")->value;
		}
		
		const rapidjson::Value& skybox = sceneData.FindMember("Skybox")->value;

		if (!skybox.IsNull() && !skybox.ObjectEmpty())
		{
			// Skybox loading should be done here
		}
		// TODO: Add the skybox to the scene
		m_pSkybox = std::unique_ptr<Skybox>(new Skybox("../../content/art/skybox/NightSky.png"));
		for (auto& node : m_nodes)
			node->Init();
	}
	else
		std::cerr << "Scene json file is empty" << std::endl;

	// Light Setup
	lightPosition = glm::vec3(0.0f, 9.0f, 0.0f);
	lightSpecular = glm::vec3(1.0f);
	lightDiffuse = glm::vec3(0.75f);
	lightAmbient = lightDiffuse * glm::vec3(0.2f);
	// Light Matrix
	near_plane = 1.0f; far_plane = 20.0f;
	glm::mat4 lightProjection = glm::perspective(90.0f, SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	//glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane)
	glm::mat4 lightView = glm::lookAt(lightPosition,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;

	// FBO for shadow map setup
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader* shadowDebug = SERVICE_LOCATOR.GetResourceManager()->GetShader("ShadowDebug");
	//shadowDebug->Use();
	//shadowDebug->SetUniform("depthMap", 0);

}

void Scene::Update()
{
	for (auto& node : m_nodes)
		node->Update();
}

void Scene::Render()
{
	// Shadow Map Pass
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	Shader* shadow = SERVICE_LOCATOR.GetResourceManager()->GetShader("Shadow");
	shadow->Use();
	shadow->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

	// Render all game objects from light's perspective
	for (auto& node : m_nodes)
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(node);
		if (gameObject)
		{
			gameObject->Render(shadow);
		}
		else
			continue;
	}
	shadow->Unuse();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset viewport for scene or debugging
	auto frameBuffer = SERVICE_LOCATOR.GetWindowHandler()->FrameBuffer;
	glViewport(0, 0, frameBuffer.Width, frameBuffer.Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Debug Draw
	if (SERVICE_LOCATOR.GetUI()->GetState("Debug Options", "ShadowMap", IMGUI_ELEMENT_TYPE::DROPDOWN_TOGGLE))
	{
		Shader* shadowDebug = SERVICE_LOCATOR.GetResourceManager()->GetShader("ShadowDebug");
		shadowDebug->Use();
		shadowDebug->SetUniform("near_plane", near_plane);
		shadowDebug->SetUniform("far_plane", far_plane);
		SERVICE_LOCATOR.GetResourceManager()->GetTexture("Brick_diff");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();
		shadowDebug->Unuse();
	}

	
	// Final Render Pass
	for (auto& node : m_nodes)
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(node);
		if (gameObject)
		{
			gameObject->Render();
		}
		else
			node->Render();
	}
		
}

void Scene::PostUpdate()
{
    for (auto& node : m_nodes)
        node->Flush();
}

void Scene::Shutdown()
{
	for (auto& node : m_nodes)
		node->Shutdown();

    for (auto& node : m_nodes)
        node->Destroy();
}

void Scene::AddNode(Node* node)
{
    m_nodes.push_back(node);
    node->SetID(++m_nodeCount);
}

void Scene::DeleteNode(Node* node)
{
	auto target = RemoveNode(node);
	if (target)
        target->Destroy();
}

Node* Scene::RemoveNode(Node* node)
{
	// if the node has a parent, it is not a root node
    if (node->GetParent())
		return nullptr;
    auto id = node->GetID();
    //
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
        [id](Node* node) { return node->GetID() == id; });

    // replace the last node with deleted one unless we are deleting the last node in order to prevent access violation
    if (it != m_nodes.end()) {
        auto lastNode = m_nodes.back();

        if (id != lastNode->GetID())
        {
            lastNode->SetID(id);
            *it = lastNode;
        }

        m_nodes.pop_back();
        m_nodeCount--;
    }

    return *it;
}