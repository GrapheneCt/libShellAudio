#ifndef SHELLAUDIO_H
#define SHELLAUDIO_H

#include <stdbool.h>
#include <psp2/kernel/modulemgr.h> 
#include <psp2/kernel/threadmgr.h> 
#include <psp2/appmgr.h> 
#include <psp2/kernel/sysmem.h> 
#include <psp2/kernel/clib.h>
#include <psp2/types.h> 
#include <psp2/sysmodule.h> 
#include <psp2/io/fcntl.h> 
#include <psp2/shellutil.h> 

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SceShellSvcAudioCustomOpt {
	int flag; // set to 0 if not used
	int param1;
	int param2;
	int param3;
} SceShellSvcAudioCustomOpt;

typedef struct SceShellSvcAudioMusicPlayerStatus {
	int stat1;
	int stat2;
	int stat3;
} SceShellSvcAudioMusicPlayerStatus;

typedef enum SceShellSvcAudioCommands {
	SCE_SHELLAUDIO_DEFAULT = 0,
	SCE_SHELLAUDIO_PLAY = 0x1,
	SCE_SHELLAUDIO_STOP = 0x2,
	SCE_SHELLAUDIO_NEXT = 0x3,
	SCE_SHELLAUDIO_PREVIOUS = 0x4,
	SCE_SHELLAUDIO_SEEK = 0x11
} SceShellSvcAudioCommands;

typedef enum SceShellSvcAudioRepeatMode {
	SCE_SHELLAUDIO_REPEAT_ALL,
	SCE_SHELLAUDIO_REPEAT_ONE,
	SCE_SHELLAUDIO_REPEAT_DISABLE
} SceShellSvcAudioRepeatMode;

typedef enum SceShellSvcAudioEQMode {
	SCE_SHELLAUDIO_EQ_DISABLE,
	SCE_SHELLAUDIO_EQ_HEAVY,
	SCE_SHELLAUDIO_EQ_POP,
	SCE_SHELLAUDIO_EQ_JAZZ,
	SCE_SHELLAUDIO_EQ_UNIQUE
} SceShellSvcAudioEQMode;

typedef enum SceShellSvcAudioShuffleMode {
	SCE_SHELLAUDIO_SHUFFLE_DISABLE = 0x1,
	SCE_SHELLAUDIO_SHUFFLE_ENABLE,
} SceShellSvcAudioShuffleMode;

/* Error codes */

typedef enum SceShellSvcAudioErrorCodes {
	SCE_SHELLAUDIO_ERROR_INVALID_ARG = 0x80101900,
	SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED = 0x80101901,
	SCE_SHELLAUDIO_ERROR_ALREADY_INITIALIZED = 0x80101902,
	SCE_SHELLAUDIO_ERROR_NOT_READY = 0x80101903,
	SCE_SHELLAUDIO_ERROR_INVALID_ARG_2 = 0x80100E00
} SceShellSvcAudioErrorCodes;

/* Application BGM */

int shellAudioInitializeForBGM(int init_type);
int shellAudioSetAudioForBGM(char* path, SceShellSvcAudioCustomOpt* optParams);
int shellAudioSetParam1ForBGM(int param);
int shellAudioSetParam2ForBGM(int8_t param);
int shellAudioSendCommandForBGM(int commandId, int param_2);

/* Music Player */

int shellAudioInitializeForMusicPlayer(int init_type);
int shellAudioGetSomethingForMusicPlayer1(void* infoBuffer);
int shellAudioGetSomethingForMusicPlayer2(void* infoBuffer);
int shellAudioGetSomethingForMusicPlayer3(void* infoBuffer);
int shellAudioGetStatusForMusicPlayer(SceShellSvcAudioMusicPlayerStatus* status);
int shellAudioSendCommandForMusicPlayer(int commandId, int param_2);
int shellAudioSetSeekTimeForMusicPlayer(unsigned int time);
int shellAudioSetRepeatModeForMusicPlayer(int mode);
int shellAudioSetShuffleModeForMusicPlayer(int mode);
int shellAudioSetEQModeForMusicPlayer(int mode);
int shellAudioSetAudioForMusicPlayer(char* path, SceShellSvcAudioCustomOpt* optParams);

#ifdef __cplusplus
}
#endif

#endif