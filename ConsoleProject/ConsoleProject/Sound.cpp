#include "Sound.h"
#include "ConsoleRenderer.h"
#include <stdio.h>

namespace Sound
{
	void Initialize(FMOD::System* system, int maxChannel, unsigned int Fmod_flag)
	{
		if (FMOD::System_Create(&system) != FMOD_OK)
		{
			ConsoleRenderer::print((char*)"System_Create fail");
		}
		system->init(maxChannel, Fmod_flag, nullptr);
	}

	bool LoadSound(FMOD::System* system, char* baseDir, char* fileName, unsigned int mod, FMOD::Sound* sound)
	{
		char filename[200];
		sprintf_s(filename, sizeof(filename), "%s/%s", baseDir, fileName);

		if (system->createSound(filename, mod, nullptr, &sound) != FMOD_OK) 
		{
			ConsoleRenderer::print((char*)"Sound::LoadSound()  createSound fail");
			return false;
		}

		return true;
	}

	void SetSoundMod(FMOD::Sound* sound, unsigned int mod)
	{
		if(sound)
			sound->setMode(mod);
	}

	void PlaySoundWithVolume(FMOD::System* system, FMOD::Sound* sound, FMOD::Channel* channel, float volume)
	{
		system->playSound(sound, nullptr, false, &channel);
		channel->setVolume(volume);  // 볼륨 조절
	}

	void ReleaseSystem(FMOD::System* system)
	{
		system->close();
		system->release();
	}

	void ReleaseSound(FMOD::Sound* sound)
	{
		sound->release();
	}

	void ReleaseChannel(FMOD::Channel* channel)
	{
		channel->stop();
	}
}