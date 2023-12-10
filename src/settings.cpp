#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "common.h"
#include "settings.h"

void LoadSettings()
{
 	SDFile *SettingsFile;
 	SettingsFile = pd->file->open("settings.dat", kFileReadData);
 	if(SettingsFile)
 	{
        pd->file->read(SettingsFile, &Difficulty, sizeof(Difficulty));
		pd->file->read(SettingsFile, &JumpHeuristicEnabled, sizeof(JumpHeuristicEnabled));
		pd->file->close(SettingsFile);
 	}
 	else
 	{
        Difficulty = Hard;
        JumpHeuristicEnabled = false;
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
#ifndef WINDLL
		pd->file->flush(SettingsFile);
#endif
	 	pd->file->close(SettingsFile);
 	}
}