#include "pch.h"

#include "Camera.h"
#include "resourcemanager/ResourceManager.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"
#include "scenemanager/SceneManager.h"

Skybox::Skybox(const char* filename)
{
	m_name = "SampleSkybox";
	//TODO: load skybox from resource manager->texture manager using input string
	m_pShader = SERVICE_LOCATOR.GetResourceManager()->GetShader("Skybox");
	// Skybox cube
	m_pGeometry = SERVICE_LOCATOR.GetResourceManager()->GetGeometry("Skybox");
	m_skybox.resize(6);
	LoadSkybox(filename);
	SERVICE_LOCATOR.GetSceneManager()->GetCurrentScene()->AddNode(this);
	RemoveComponent<TransformComponent>();
}

Skybox::~Skybox()
{
	delete m_pShader;
	delete m_pGeometry;
}

void Skybox::LoadSkybox(const char* filename)
{
	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filename);
	if (error)
	{
		std::cerr << "Error decoding Skybox: " << lodepng_error_text(error) << std::endl;
		return;
	}

	struct FaceRegion
	{
		unsigned x;
		unsigned y;
	};

	FaceRegion regions[6] =
	{
		{3, 1}, // Back
		{1, 1}, // Front
		{1, 2}, // Down
		{1, 0}, // Up
		{0, 1}, // Left
		{2, 1}  // Right
	};

	// Define an array with the uniform names corresponding to each face
	const char* cubeMapFaces[6] =
	{
		"cubeMap.Back",
		"cubeMap.Front",
		"cubeMap.Down",
		"cubeMap.Up",
		"cubeMap.Left",
		"cubeMap.Right"
	};

	unsigned faceWidth = width / 4;
	unsigned faceHeight = height / 3;

	std::vector<unsigned char> face(faceWidth * faceHeight * 4);//4 for RGBA

	m_pShader->Use();
	for (GLuint i = 0; i < 6; ++i)
	{
		// Calculate the starting X and Y coordinates for each face
		unsigned startX = regions[i].x * faceWidth;
		unsigned startY = regions[i].y * faceHeight;

		extractFace(face, faceWidth, faceHeight, startX, startY, width, image);

		glGenTextures(1, &m_skybox[i]);
		glBindTexture(GL_TEXTURE_2D, m_skybox[i]);

		// set the texture
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, face.data());

		// Assign the texture unit to the uniform
		m_pShader->SetUniform(cubeMapFaces[i], i);
	}
	m_pShader->Unuse();
}

void Skybox::Render()
{
	// TODO: Maybe skybox should follow the camera? How mauch to scale? Need to discuss.
	auto model = glm::translate(glm::mat4((1.f)), glm::vec3(0, 0, 0)) *
		glm::scale(glm::mat4((1.f)), glm::vec3(200.f));

	m_pShader->Use();
	m_pGeometry->Bind(m_pShader);

	m_pShader->SetUniform("model", model);
	m_pShader->SetUniform("view", Camera::GetInstance()->m_worldView);
	m_pShader->SetUniform("projection", Camera::GetInstance()->m_worldProjection);

	for (GLuint i = 0; i < 6; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_skybox[i]);
	}

	m_pGeometry->Render();
	m_pGeometry->Unbind();
	m_pShader->Unuse();
}

void Skybox::extractFace(
	std::vector<unsigned char>& face, 
	unsigned faceWidth, 
	unsigned faceHeight, 
	unsigned startX, 
	unsigned startY, 
	unsigned imageWidth, 
	const std::vector<unsigned char>& image) 
{
	for (unsigned y = 0; y < faceHeight; ++y)
		for (unsigned x = 0; x < faceWidth; ++x)
			for (unsigned c = 0; c < 4; ++c)  // 4 channels (RGBA)
				face[4 * (y * faceWidth + x) + c] = image[4 * ((y + startY) * imageWidth + (startX + x)) + c];
}
