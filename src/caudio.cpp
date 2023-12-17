#include <pd_api.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "caudio.h"
#include "pd_helperfuncs.h"

#define SND_Max 100
#define MUS_Max 20

bool CAudio_DebugInfo, CAudio_SoundEnabled=true, CAudio_MusicEnabled=true;
int CAudio_VolumeMusic = 128, CAudio_VolumeSound = 128;
SamplePlayer* CAudio_Sounds[SND_Max];
FilePlayer* CAudio_Music[MUS_Max];
bool CAudio_GlobalSoundEnabled = true;
float CAudio_MusicBufferLen = 1.0f;

void CAudio_Init(bool ADebugInfo, float MusicBufferLen)
{
	CAudio_DebugInfo = ADebugInfo;
	CAudio_GlobalSoundEnabled = true;
	pd->system->logToConsole("Audio Succesfully initialised!\n");
	
	for (int i=0; i < SND_Max; i++)
		CAudio_Sounds[i] = NULL;

	for (int i=0; i < MUS_Max; i++)
		CAudio_Music[i] = NULL;
	
	CAudio_MusicBufferLen = MusicBufferLen;
}

void CAudio_DeInit()
{
	CAudio_UnloadSounds();
	CAudio_UnloadMusics();
	if (CAudio_GlobalSoundEnabled)
		CAudio_GlobalSoundEnabled = false;
		
}

// set the volume of the music
void CAudio_SetVolumeMusic(const int VolumeIn)
{
	if (CAudio_GlobalSoundEnabled)
	{
		CAudio_VolumeMusic = VolumeIn;
		for (int i = 0; i < MUS_Max; i++)
			if (CAudio_Music[i])
				if(pd->sound->fileplayer->isPlaying(CAudio_Music[i]))
					pd->sound->fileplayer->setVolume(CAudio_Music[i], (float)VolumeIn / 128.0f, (float)VolumeIn / 128.0f);
	}
}

// set the volume of sound
void CAudio_SetVolumeSound(const int VolumeIn)
{
	if (CAudio_GlobalSoundEnabled)
	{
		CAudio_VolumeSound = VolumeIn;
		for (int i = 0; i < SND_Max; i++)
			if (CAudio_Sounds[i])
				if(pd->sound->sampleplayer->isPlaying(CAudio_Sounds[i]))
					pd->sound->sampleplayer->setVolume(CAudio_Sounds[i], (float)VolumeIn / 128.0f, (float)VolumeIn / 128.0f);
	}
}

// increase the music volume with 4
void CAudio_IncVolumeMusic()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeMusic < 128)
		{
			CAudio_VolumeMusic += 4;
			CAudio_SetVolumeMusic(CAudio_VolumeMusic);
		}
	}
}

// increase the sound volume with 4
void CAudio_IncVolumeSound()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeSound < 128)
		{
			CAudio_VolumeSound += 4;
			CAudio_SetVolumeSound(CAudio_VolumeSound);
		}
	}
}

// decrease the volume with 4
void CAudio_DecVolumeMusic()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeMusic > 0)
		{
			CAudio_VolumeMusic -= 4;
			CAudio_SetVolumeMusic(CAudio_VolumeMusic);
		}
	}
}

// decrease the volume with 4
void CAudio_DecVolumeSound()
{
	if (CAudio_GlobalSoundEnabled)
	{
		if (CAudio_VolumeSound > 0)
		{
			CAudio_VolumeSound -= 4;
			CAudio_SetVolumeSound(CAudio_VolumeSound);
		}
	}
}

void CAudio_StopMusic()
{
	if (CAudio_GlobalSoundEnabled)
		for (int i = 0; i < MUS_Max; i++)
			if (CAudio_Music[i])
				pd->sound->fileplayer->stop(CAudio_Music[i]);
}

bool CAudio_IsMusicPlaying()
{
	if(!CAudio_GlobalSoundEnabled)
		return false;

	bool result = false;
	for (int i = 0; i < MUS_Max; i++)
		if (CAudio_Music[i])
			result = result || pd->sound->fileplayer->isPlaying(CAudio_Music[i]);

	return result;
}

void CAudio_UnLoadMusic(int MusicdID)
{
	if ((MusicdID < 0) || (MusicdID > MUS_Max) || !CAudio_GlobalSoundEnabled)
		return;

	if (CAudio_Music[MusicdID] == NULL)
		return;

	else
	{
		pd->sound->fileplayer->stop(CAudio_Music[MusicdID]);
		pd->sound->fileplayer->freePlayer(CAudio_Music[MusicdID]);
		CAudio_Music[MusicdID] = NULL;
	}
}

void CAudio_UnloadMusics()
{
	CAudio_StopMusic();
	for (int i=0; i < MUS_Max; i++)
		CAudio_UnLoadMusic(i);
}

int CAudio_LoadMusic(std::string  FileName)
{
	int result = -1;
	
	char * FullFileName;
	pd->system->formatString(&FullFileName, "music/%s",FileName.c_str());
	if (CAudio_GlobalSoundEnabled)
		for (int i=0; i < MUS_Max; i++)
			if(CAudio_Music[i] == NULL)
			{
				FilePlayer* Tmp = pd->sound->fileplayer->newPlayer();
				if (Tmp)
				{
					if (pd->sound->fileplayer->loadIntoPlayer(Tmp, FullFileName) == 1)
					{	
						//needs to be called after loadIntoPlayer or the buffer length does not seem to be aplied		
						pd->sound->fileplayer->setBufferLength(Tmp, CAudio_MusicBufferLen);
						CAudio_Music[i] = Tmp;
						if (CAudio_DebugInfo)
							pd->system->logToConsole("Loaded Music %s\n", FullFileName);
						result = i;
					}
					else
					{
						pd->sound->fileplayer->freePlayer(Tmp);
					}
				}
				break;
			}
	
	if(result == -1)
		if(CAudio_DebugInfo)
			pd->system->logToConsole("Failed Loading Music %s", FullFileName);
	
	pd->system->realloc(FullFileName, 0);
	
	return result;
}

int CAudio_MusicSlotsUsed()
{
	int c = 0;
	for (int i=0; i < MUS_Max; i++)
	{
		if(CAudio_Music[i] != NULL)
			c++;
	}
	return c;
}

int CAudio_MusicSlotsMax()
{
	return MUS_Max;
}

void CAudio_PlayMusic(int MusicID, int loops)
{
	if ((MusicID < 0) || (MusicID > MUS_Max) || !CAudio_GlobalSoundEnabled || !CAudio_MusicEnabled)
		return;

	CAudio_StopMusic();
	pd->sound->fileplayer->setVolume(CAudio_Music[MusicID], (float)CAudio_VolumeMusic / 128.0f, (float)CAudio_VolumeMusic / 128.0f);
	pd->sound->fileplayer->play(CAudio_Music[MusicID], loops +1);
}

int CAudio_GetVolumeMusic()
{
	return CAudio_VolumeMusic;
}

int CAudio_SoundSlotsUsed()
{
	int c = 0;
	for (int i=0; i < SND_Max; i++)
	{
		if(CAudio_Sounds[i] != NULL)
			c++;
	}
	return c;
}

int CAudio_SoundSlotsMax()
{
	return SND_Max;
}


void CAudio_PlaySound(int SoundID, int loops)
{
	if ((SoundID < 0) || (SoundID > SND_Max) || !CAudio_GlobalSoundEnabled || !CAudio_SoundEnabled)
		return;
	pd->sound->sampleplayer->setVolume(CAudio_Sounds[SoundID], (float)CAudio_VolumeSound / 128.0f, (float)CAudio_VolumeSound / 128.0f);
	pd->sound->sampleplayer->play(CAudio_Sounds[SoundID], loops +1, 1.0f);
}

int CAudio_LoadSound(std::string  FileName)
{
	int result = -1;

	char* FullFileName;
	pd->system->formatString(&FullFileName, "sound/%s", FileName.c_str());

	if(CAudio_GlobalSoundEnabled)
		for (int i=0; i < SND_Max; i++)
			if(CAudio_Sounds[i] == NULL)
			{
				SamplePlayer* Tmp = pd->sound->sampleplayer->newPlayer();
				if(Tmp)
				{
					AudioSample* Sample = pd->sound->sample->load(FullFileName);
					if (Sample)
					{
						pd->sound->sampleplayer->setSample(Tmp, Sample);
						pd->sound->sampleplayer->setVolume(Tmp, (float)CAudio_VolumeSound / 128.0f, (float)CAudio_VolumeSound / 128.0f);
						CAudio_Sounds[i] = Tmp;
						if (CAudio_DebugInfo)
							pd->system->logToConsole("Loaded Sound %s\n", FullFileName);
						result = i;
					}
					else
					{
						pd->sound->sampleplayer->freePlayer(Tmp);
					}
				}
				break;
			}

	if (result == -1)
		if(CAudio_DebugInfo)
			pd->system->logToConsole("Failed Loading Sound %s\n", FullFileName);
	
	pd->system->realloc(FullFileName, 0);
	
	return result;
}

void CAudio_UnLoadSound(int SoundID)
{
	if ((SoundID < 0) || (SoundID > SND_Max) || !CAudio_GlobalSoundEnabled)
		return;

	if (CAudio_Sounds[SoundID] == NULL)
		return;
	else
	{
		pd->sound->sampleplayer->stop(CAudio_Sounds[SoundID]);
		pd->sound->sampleplayer->freePlayer(CAudio_Sounds[SoundID]);
		CAudio_Sounds[SoundID] = NULL;
	}
}

void CAudio_UnloadSounds()
{
	CAudio_StopSound();

	for (int i=0; i < SND_Max; i++)
		CAudio_UnLoadSound(i);
}

int CAudio_GetVolumeSound()
{
	return CAudio_VolumeSound;
}

void CAudio_StopSound()
{
	if(CAudio_GlobalSoundEnabled)
		for (int i = 0; i < SND_Max; i++)
			if (CAudio_Sounds[i] != NULL)
				pd->sound->sampleplayer->stop(CAudio_Sounds[i]);
}

void CAudio_SetSoundEnabled(bool Enabled)
{
	CAudio_SoundEnabled = Enabled;
	if(!CAudio_SoundEnabled)
		CAudio_StopSound();
}

void CAudio_SetMusicEnabled(bool Enabled)
{
	CAudio_MusicEnabled = Enabled;
	if(!CAudio_MusicEnabled)
		CAudio_StopMusic();
}
