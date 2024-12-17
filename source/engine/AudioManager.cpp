// Created by Cameron Allen (dynamic sound adjustment, proximity audio) and 
// Jackson Rollins (init Fmod setup)

#include "pch.h"
#include "AudioManager.h"
#include "DeserializeJSON.h"
#include "Camera.h"
#include "ui/UI.h"
#include "Input.h"

std::unique_ptr<AudioManager> AudioManager::mp_instance = nullptr;

// Gets AudioManager instance
AudioManager* AudioManager::GetInstance()
{
	if (mp_instance == nullptr) {
		mp_instance = std::unique_ptr<AudioManager>(new AudioManager());
	}
	return mp_instance.get();
}

// AudioManager constructor
AudioManager::AudioManager() : m_initMaxVolume(1.0f), m_maxChannels(25)
{}

// AudioManager destructor
AudioManager::~AudioManager() {

}

// Initializes AudioManager via a JSON file
void AudioManager::Init(const char* path) {
	FMOD_RESULT result;

	result = FMOD::System_Create(&mp_system);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

	result = mp_system->init(m_maxChannels, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

	result = mp_system->getMasterChannelGroup(&mp_masterChannel);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

	result = mp_system->set3DSettings(1.0, 1.0, 0.25);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

	m_masterVolume = m_musicVolume = m_sfxVolume = 0.5f;

	DeserializeJSON::LoadAudio(path);
}

// Creates a sound given a file path, mode, and initial volume
void AudioManager::CreateSound(std::string filePath, FMOD_MODE mode,
	float volume) 
{
	std::string file_path = std::string(mp_audioRootPath).append(filePath);

	FMOD::Sound* sample;
	FMOD_RESULT result;

	result = mp_system->createSound(file_path.c_str(), mode, 0, &sample);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	sample->set3DMinMaxDistance(1.0f, 60.0f);
	m_soundMap.insert({ filePath, SoundData(sample, volume, mode)});
}

// Plays a sound given a file path and optional sound position
void AudioManager::PlaySound(std::string filePath, const glm::vec3* soundPos)
{
	if (m_muted)
		return;

	FMOD_RESULT result;

	auto sound = m_soundMap.find(filePath);
	if (sound != m_soundMap.end())
	{
		FMOD::Channel* channel;
		auto* soundPtr = &(*sound);
		int index = -1;
		result = mp_system->playSound(sound->second.sound, 0, false, &channel);
		assert(result == FMOD_OK);
		result = channel->getIndex(&index);
		assert(result == FMOD_OK);
		m_activeChannels.insert({ index, soundPtr });
		if (sound->second.mode & FMOD_3D)
			channel->setVolume(sound->second.volume * m_sfxVolume);
		else
			channel->setVolume(sound->second.volume * m_musicVolume);
		if (soundPos != nullptr) 
		{
			FMOD_VECTOR sound_position = glmToFMOD(*soundPos);
			channel->set3DAttributes(&sound_position, 0);
		}
	}
	else
	{
		printf("%s not in the sound map.", filePath.c_str());
	}
}

// Sets the master volume
void AudioManager::SetMasterVolume(float volume) {
	m_masterVolume = volume;
	mp_masterChannel->setVolume(volume);
}

// Stops all sound
void AudioManager::StopSound() {
	mp_masterChannel->stop();
}

// Toggles mute
void AudioManager::ToggleMute() {
	if (m_masterVolume < 0.1f) {
		SetMasterVolume(0.1f);
		m_muted = false;
	}
	else {
		SetMasterVolume(0.0f);
		m_muted = true;
	}
}

// Sets mute
void AudioManager::SetMute(bool mute)
{
	if (mute) 
	{
		SetMasterVolume(0.0f);
		m_muted = true;
	}
	else 
	{
		SetMasterVolume(m_initMaxVolume);
		m_muted = false;
	}
}

// Updates AudioManager
void AudioManager::Update() 
{
#ifdef _DEBUG
	if (SERVICE_LOCATOR.GetUI()->GetState("Mute", IMGUI_ELEMENT_TYPE::BUTTON))
		SetMute(true);
	else if (m_muted)
		SetMute(false);
#endif // _DEBUG
	if (m_muted)
		return;

	Camera* cam = Camera::GetInstance();
	glm::vec3 lPos(cam->m_worldView[3]);

	// Forward vector normalized
	glm::vec3 forwardVec(glm::normalize(glm::vec3(
		cam->m_worldView[0][2],
		cam->m_worldView[1][2],
		cam->m_worldView[2][2]
	)));
	
	// Extract translation vector
	glm::vec3 translation = glm::vec3(cam->m_worldView[3]);

	// Extract rotation matrix (upper-left 3x3 of worldView)
	glm::mat3 rotation = glm::mat3(cam->m_worldView);

	// Invert rotation and apply to translation
	glm::vec3 cameraPosition = -glm::transpose(rotation) * translation;

	FMOD_VECTOR listenerPos = glmToFMOD(cameraPosition);
	FMOD_VECTOR listenerForward = glmToFMOD(forwardVec);
	FMOD_VECTOR listenerUp = glmToFMOD(glm::normalize(glm::vec3(
		cam->m_worldView[0][1], // Y-axis in the view matrix
		cam->m_worldView[1][1],
		cam->m_worldView[2][1]
	)));

	// Set listener attributes
	FMOD_RESULT result = mp_system->set3DListenerAttributes(0, &listenerPos, nullptr, &listenerForward, &listenerUp);
	
#ifdef _DEBUG
	m_masterVolume = SERVICE_LOCATOR.GetUI()->GetSliderValue("Volume Settings", "Master");
	m_musicVolume = SERVICE_LOCATOR.GetUI()->GetSliderValue("Volume Settings", "Music");
	m_sfxVolume = SERVICE_LOCATOR.GetUI()->GetSliderValue("Volume Settings", "SFX");
	SetMasterVolume(m_masterVolume * m_initMaxVolume);
#endif // _DEBUG

	UpdateActiveSoundChannels();
	
	// Update FMOD system
	mp_system->update();
}

// Shuts down AudioManager
void AudioManager::Shutdown() {
	for (auto& sound_entry : m_soundMap) {
		sound_entry.second.sound->release();
	}
}

// Updates active sound channels and removes inactive ones
void AudioManager::UpdateActiveSoundChannels()
{
	for (auto itr = m_activeChannels.begin(); itr != m_activeChannels.end(); itr++)
	{
		FMOD::Channel* channel = nullptr;
		FMOD_RESULT result = mp_system->getChannel(itr->first, &channel);
		if (result == FMOD_OK && channel)
		{
			bool isPlaying = false;
			channel->isPlaying(&isPlaying);
			if (isPlaying)
			{
				const float volume = itr->second->second.volume;
				const unsigned int mode = itr->second->second.mode;
				if (mode & FMOD_3D)
					channel->setVolume(volume * m_sfxVolume);
				else
					channel->setVolume(volume * m_musicVolume);
			}
			else
				itr = m_activeChannels.erase(itr);
		}
	}
}

// Converts a glm vector to an FMOD vector
FMOD_VECTOR AudioManager::glmToFMOD(glm::vec3 vec) const
{
	return FMOD_VECTOR(vec.x, vec.y, vec.z);
}
