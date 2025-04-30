// Created by Cameron Allen (dynamic sound adjustment, proximity audio) and 
// Jackson Rollins (init Fmod setup)

#include "pch.h"
#include "AudioManager.h"
#include "cameramanager/CameraManager.h"
#include "cameramanager/CameraComponent.h"
#include "VectorCalculations.h"
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
AudioManager::AudioManager() : m_pEventListener(new EventListener()), m_initMaxVolume(1.0f), m_maxChannels(25)
{
}

// AudioManager destructor
AudioManager::~AudioManager()
{
}

// Initializes AudioManager via a JSON file
void AudioManager::Init() {
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

	m_pEventListener->SubscribeToEvent("Master Volume", std::bind(&AudioManager::SetMaster, this, std::placeholders::_1));
	m_pEventListener->SubscribeToEvent("Music Volume", std::bind(&AudioManager::SetMusic, this, std::placeholders::_1));
	m_pEventListener->SubscribeToEvent("SFX Volume", std::bind(&AudioManager::SetSFX, this, std::placeholders::_1));
	m_pEventListener->SubscribeToEvent("Mute", std::bind(&AudioManager::MuteVolume, this, std::placeholders::_1));

	SystemSettings* systemSettings = SERVICE_LOCATOR.GetSystemSettings();
	m_masterVolume = std::any_cast<float>(*systemSettings->GetSetting("Master Volume"));
	m_musicVolume = std::any_cast<float>(*systemSettings->GetSetting("Music Volume"));
	m_sfxVolume = std::any_cast<float>(*systemSettings->GetSetting("SFX Volume"));
	m_muted = std::any_cast<bool>(*systemSettings->GetSetting("Mute"));
	SetMasterVolume(m_masterVolume * m_initMaxVolume);

	std::filesystem::path audioPath = Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "code" / "json_files" / "Audio.json";

	DeserializeJSON::LoadAudio(audioPath.string().c_str());
	std::cout << "AudioManager Initialized\n";
}

// Creates a sound given a file path, mode, and initial volume
void AudioManager::CreateSound(std::string filePath, FMOD_MODE mode,
	float volume)
{
	std::string file_path = (Utils::GetExecutableDirectory().parent_path().parent_path().parent_path() / "content" / "audio" / filePath).string();

	FMOD::Sound* sample;
	FMOD_RESULT result;

	result = mp_system->createSound(file_path.c_str(), mode, 0, &sample);
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	sample->set3DMinMaxDistance(1.0f, 60.0f);
	m_soundMap.insert({ filePath, SoundData(sample, volume, mode) });
}

// Plays a sound given a file path and optional sound position
void AudioManager::PlaySound(std::string filePath, const glm::vec3* soundPos)
{
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
	m_muted = mute;
	SERVICE_LOCATOR.GetSystemSettings()->SetSetting("Mute", m_muted);
	if (mute)
	{
		SetMasterVolume(0.0f);
	}
	else
	{
		float master = SERVICE_LOCATOR.GetSystemSettings()->GetFloatSetting("Master Volume");
		std::cout << "setting master to: " << master * m_initMaxVolume << std::endl;
		SetMasterVolume(master * m_initMaxVolume);
	}
}

// Updates AudioManager
void AudioManager::Update()
{
	if (m_muted)
	{
		SetMasterVolume(0.0f);
		return;
	}

	CameraManager* camManager = SERVICE_LOCATOR.GetCameraManager();

	auto cam = SERVICE_LOCATOR.GetCameraManager()->GetMainCamera();
	if (cam == nullptr)
	{
		//std::cout << "No Camera!!" << std::endl;
		return;
	}
	auto view = cam->GetViewMatrix();
	glm::vec3 lPos(view[3]);

	// Forward vector normalized
	glm::vec3 forwardVec = VectorCalculation::GetForwardVec(cam->GetRotation());

	// Extract translation vector
	glm::vec3 translation = glm::vec3(view[3]);

	// Extract rotation matrix (upper-left 3x3 of worldView)
	glm::mat3 rotation = glm::mat3(view);

	// Invert rotation and apply to translation
	glm::vec3 cameraPosition = -glm::transpose(rotation) * translation;

	FMOD_VECTOR listenerPos = glmToFMOD(cameraPosition);
	FMOD_VECTOR listenerForward = glmToFMOD(forwardVec);
	FMOD_VECTOR listenerUp = glmToFMOD(glm::normalize(glm::vec3(
		view[0][1], // Y-axis in the view matrix
		view[1][1],
		view[2][1]
	)));

	// Set listener attributes
	FMOD_RESULT result = mp_system->set3DListenerAttributes(0, &listenerPos, nullptr, &listenerForward, &listenerUp);

	SetMasterVolume(m_masterVolume * m_initMaxVolume);

	UpdateActiveSoundChannels();

	// Update FMOD system
	mp_system->update();
}

// Shuts down AudioManager
void AudioManager::Shutdown() {
	for (auto& sound_entry : m_soundMap) {
		sound_entry.second.sound->release();
	}
	delete(m_pEventListener);
}

void AudioManager::SetMaster(Event* event)
{
	m_masterVolume = static_cast<FloatEvent*>(event)->value;
	SERVICE_LOCATOR.GetSystemSettings()->SetSetting("Master Volume", m_masterVolume);
}

void AudioManager::SetMusic(Event* event)
{
	m_musicVolume = static_cast<FloatEvent*>(event)->value;
	SERVICE_LOCATOR.GetSystemSettings()->SetSetting("Music Volume", m_musicVolume);
}

void AudioManager::SetSFX(Event* event)
{
	m_sfxVolume = static_cast<FloatEvent*>(event)->value;
	SERVICE_LOCATOR.GetSystemSettings()->SetSetting("SFX Volume", m_sfxVolume);
}

void AudioManager::MuteVolume(Event* event)
{
	bool state = static_cast<BoolEvent*>(event)->value;
	SetMute(state);
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