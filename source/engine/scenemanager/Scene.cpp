#include "../pch.h"
#include "../objectmanager/GameObjectFactory.h"
#include "../resourcemanager/ResourceManager.h"
#include "../ui/UI.h"
#include "../LightComponent.h"
#include "../../include/RenderComponent.h"
#include "TransformComponent.h"
#include "../cameramanager/CameraManager.h"
#include "../cameramanager/CameraComponent.h"

Scene::~Scene()
{
	Shutdown();
	FlushNodes();
}

void Scene::Init()
{
	m_pUINode = std::make_unique<Node>();
	m_pUINode->SetName("Game UI");

	m_init = true;
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

		const auto& goMember = sceneData.FindMember("GameObject");
		const rapidjson::Value& gameObjects = goMember->value;

		if (goMember != sceneData.MemberEnd() && !gameObjects.IsNull() && !gameObjects.ObjectEmpty())
		{
			// Should load all the assets for the scene from json
			SERVICE_LOCATOR.GetGameObjectFactory()->CreateAllGameObjects(gameObjects);
		}
		const auto& ui = sceneData.FindMember("UI");
		if (ui != sceneData.MemberEnd() && !ui->value.IsNull() && !ui->value.ObjectEmpty())
		{
			// Should load all the UI nodes for the scene from json
			SERVICE_LOCATOR.GetGameObjectFactory()->CreateAllUIObjects(ui->value);
		}

		const rapidjson::Value& skybox = sceneData.FindMember("Skybox")->value;

		if (!skybox.IsNull() && !skybox.ObjectEmpty())
		{
			// Skybox loading should be done here
		}
		// TODO: Add the skybox to the scene
		std::filesystem::path skyboxPath = Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "art" / "skybox" / "Skybox_Default.png";

		m_pSkybox = std::unique_ptr<Skybox>(new Skybox(skyboxPath.string().c_str()));

		for (auto& node : m_nodes)
		{
			node->Init();
		}
	}
	else
		std::cerr << "Scene json file is empty" << std::endl;

}

void Scene::Update()
{
	for (auto& node : m_nodes)
	{
		node->Update();
	}

	// Set reference to single light component in scene
	if (light == nullptr)
	{
		for (auto& node : m_nodes)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(node);
			if (gameObject)
			{
				auto lightComp = gameObject->GetComponent<LightComponent>();
				if (lightComp != nullptr)
				{
					light = lightComp;
					break;
				}
			}
		}
	}


}

void Scene::Render()
{
	// Debug Draw
	if (SERVICE_LOCATOR.GetSystemSettings()->GetBoolSetting("ShadowMap") && light != nullptr)
	{
		// set polygon mode to fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		light->DebugDrawShadows();
	}

	for (auto& node : m_nodes)
	{
		collectRenderableNodes(node, m_nonRenderableNodes, m_opaqueBucket, m_transparentBucket);
	}

	// Final Render Pass
	for (auto& node : m_opaqueBucket)
	{
		node->Render();
	}

	// Render Skybox
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	if (m_pSkybox)
	{
		m_pSkybox->Render();
	}
	glDepthFunc(GL_LESS);

	// Render Transparent Objects
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	for (auto& node : m_transparentBucket)
	{
		node->Render();
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	for (auto& node : m_nonRenderableNodes)
	{
		node->Render();
	}

	m_opaqueBucket.clear();
	m_transparentBucket.clear();
	m_nonRenderableNodes.clear();
}

void Scene::PostUpdate()
{
	FlushNodes();
}

void Scene::Shutdown()
{
	for (auto& node : m_nodes)
		node->Shutdown();

	for (auto& node : m_nodes)
		node->Destroy();

	//m_pSkybox.get()->Shutdown();
	//m_pSkybox.get()->Destroy();
	m_pSkybox.release();
}

void Scene::AddNode(Node* node, Node* parent)
{
	if (parent) 
	{
		parent->AddChild(node);
	}
	else
	{
		m_nodes.push_back(node);
		node->SetID(++m_nodeCount);
	}
}

void Scene::DeleteNode(Node* node)
{
	if (!node)
		return;
	assert(!node->NeedsDeletion());

	auto target = std::find(m_nodes.begin(), m_nodes.end(), node);
	if (target != m_nodes.end())
		node->Destroy();
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

	auto lastNode = m_nodes.back();

	// replace the last node with deleted one unless we are deleting the last node in order to prevent access violation
	if (it != m_nodes.end()) {

		if (id != lastNode->GetID())
		{
			lastNode->SetID(id);
			*it = lastNode;
		}

		m_nodes.pop_back();
		m_nodeCount--;
	}

	return lastNode;
}

UIComponent* Scene::FindUIComponent(const std::string& name, Node* parent)
{
	auto& children = parent ? parent->GetChildren() : GetUINode()->GetChildren();
	for (auto* node : children)
	{
		UIComponent* uiComp = node->GetComponent<UIComponent>();
		if (!uiComp) { uiComp = node->GetComponent<UITextComponent>(); }
		if (uiComp && node->GetName() == name)
		{
			return uiComp;
		}
		else if (!node->GetChildren().empty())
		{
			uiComp = FindUIComponent(name, node); 
			if (uiComp) { return uiComp; }
		}
	}
	return nullptr;
}

void Scene::FlushNodes(Node* parent)
{
	std::vector<Node*>& nodes = parent ? parent->GetChildren() : m_nodes;
	if (!parent) 
	{
		for (auto& child : m_nodes) 
		{
			FlushNodes(child);
			child->Flush(parent);
		}
	}
	else 
	{
		for (auto& child : parent->GetChildren()) 
		{
			FlushNodes(child);
			child->Flush(parent);
		}
	}
}

void Scene::collectRenderableNodes(Node* node, std::vector<Node*>& nonRenderable, std::vector<Node*>& opaqueBucket, std::vector<Node*>& transparentBucket)
{
	for (auto& child : node->GetChildren())
	{
		collectRenderableNodes(child, nonRenderable, opaqueBucket, transparentBucket);
	}

	if (node->GetComponent<RenderComponent>() == nullptr)
	{
		nonRenderable.push_back(node);
	}
	else if (node->GetComponent<RenderComponent>()->GetMaterial()->GetAlpha() < 1.0f)
	{
		transparentBucket.push_back(node);
	}
	else
	{
		opaqueBucket.push_back(node);
	}
}

