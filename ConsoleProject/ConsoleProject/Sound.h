#pragma once
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

namespace Sound
{
	void Initialize(FMOD::System* system, int maxChannel, unsigned int Fmod_flag);
	bool LoadSound(FMOD::System* system, char* baseDir, char* fileName, unsigned int mod, FMOD::Sound* sound);
	void SetSoundMod(FMOD::Sound* sound, unsigned int mod);
	void PlaySoundWithVolume(FMOD::System* system, FMOD::Sound* sound, FMOD::Channel* channel, float volume);
	void ReleaseSystem(FMOD::System* system);
	void ReleaseSound(FMOD::Sound* sound);
	void ReleaseChannel(FMOD::Channel* channel);
};