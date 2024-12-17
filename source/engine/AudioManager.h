// Created by Cameron Allen (dynamic sound adjustment, proximity audio) and 
// Jackson Rollins (init Fmod setup)
#pragma once

struct SoundData 
{
	FMOD::Sound* sound;
	float volume;
	FMOD_MODE mode;
	SoundData(FMOD::Sound* _sound, float _volume = 1.0f, FMOD_MODE _mode = FMOD_DEFAULT) 
		: sound(_sound), volume(_volume), mode(_mode) {}
};

class AudioManager {
public:
	static AudioManager* GetInstance();

	AudioManager();
	~AudioManager();
	void Init(const char* path);
	void CreateSound(std::string filePath, FMOD_MODE mode,
		float volume = 1.0f);
	void PlaySound(std::string filePath, const glm::vec3* soundPos = nullptr);
	void SetMasterVolume(float volume);
	void StopSound();
	void ToggleMute();
	void SetMute(bool mute);
	void Update();
	void Shutdown();

	bool m_playMusic = true;
	bool m_playSFX = true;
	bool m_muted = false;
private:
	std::map<std::string, SoundData> m_soundMap;
	std::map<int, std::pair<const std::string, SoundData>*> m_activeChannels;
	static std::unique_ptr<AudioManager> mp_instance;
	FMOD::System* mp_system;
	FMOD::ChannelGroup* mp_masterChannel;
	const char* mp_audioRootPath = "..\\..\\content\\audio\\";
	const float m_initMaxVolume;
	const unsigned int m_maxChannels;
	float m_masterVolume;
	float m_musicVolume;
	float m_sfxVolume;

	void UpdateActiveSoundChannels();

	FMOD_VECTOR glmToFMOD(glm::vec3 vec) const;
};