#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "common.h"
#include "settings.h"

void LoadSettings()
{
	Difficulty = Hard;
	JumpHeuristicEnabled = false;
	SoundEnabled = true;
	MusicEnabled = true;

 	SDFile *SettingsFile;
 	SettingsFile = pd->file->open("settings.dat", kFileReadData);
 	if(SettingsFile)
 	{
        pd->file->read(SettingsFile, &Difficulty, sizeof(Difficulty));
		pd->file->read(SettingsFile, &JumpHeuristicEnabled, sizeof(JumpHeuristicEnabled));
		pd->file->read(SettingsFile, &MusicEnabled, sizeof(MusicEnabled));
		pd->file->read(SettingsFile, &SoundEnabled, sizeof(SoundEnabled));
		pd->file->close(SettingsFile);
 	}
}

// Save the settings
void SaveSettings()
{
 	SDFile *SettingsFile;
 	SettingsFile = pd->file->open("settings.dat", kFileWrite);
 	if(SettingsFile)
 	{
		pd->file->write(SettingsFile, &Difficulty, sizeof(Difficulty));
		pd->file->write(SettingsFile, &JumpHeuristicEnabled, sizeof(JumpHeuristicEnabled));
		pd->file->write(SettingsFile, &MusicEnabled, sizeof(MusicEnabled));
		pd->file->write(SettingsFile, &SoundEnabled, sizeof(SoundEnabled));
#ifndef WINDLL
		pd->file->flush(SettingsFile);
#endif
	 	pd->file->close(SettingsFile);
 	}
}