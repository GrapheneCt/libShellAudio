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

typedef struct SceShellSvcAudioPlaybackStatus {
	int unk_00;
	int unk_04;
	int unk_08;
	int unk_0C;
	int currentState;
	int unk_14;
	int bgmPortStatus;
	int unk_1C;
	int unk_20;
	int unk_24;
	int unk_28;
	unsigned int currentTime;
	char unk_30[0x40];
} SceShellSvcAudioPlaybackStatus;

typedef struct SceShellAudioBGMState {
	int bgmPortOwnerId;
	int bgmPortStatus;
	int someStatus1;
	int currentState;
	int someStatus2;
} SceShellAudioBGMState;

typedef struct SceShellSvcAudioMetadata {
	int unk_00;
	int unk_04;
	int duration;
	char title[0x400];
	char unk_40C[0x400];
	char album[0x400];
	char unk_C0C[0x400];
	char artist[0x400];
	char unk_140C[0x400];
	int unk_180C;
} SceShellSvcAudioMetadata;

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
	SCE_SHELLAUDIO_SHUFFLE_ENABLE
} SceShellSvcAudioShuffleMode;

typedef enum SceShellSvcAudioALCMode {
	SCE_SHELLAUDIO_ALC_DISABLE,
	SCE_SHELLAUDIO_ALC_ENABLE
} SceShellSvcAudioALCMode;

typedef enum SceShellSvcAudioPlaybackState {
	SCE_SHELLAUDIO_PBSTATE_MUSIC = 0x1,
	SCE_SHELLAUDIO_PBSTATE_SHELLBGM = 0x2
} SceShellSvcAudioPlaybackState;

typedef enum SceShellSvcAudioBGMPortState {
	SCE_SHELLAUDIO_BGM_PORT_SHELL = 0,
	SCE_SHELLAUDIO_BGM_PORT_MUSIC_PLAYER = 0x80
} SceShellSvcAudioBGMPortState;

/* Error codes */

typedef enum SceShellSvcAudioErrorCodes {
	SCE_SHELLAUDIO_ERROR_INVALID_ARG = 0x80101900,
	SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED = 0x80101901,
	SCE_SHELLAUDIO_ERROR_ALREADY_INITIALIZED = 0x80101902,
	SCE_SHELLAUDIO_ERROR_NOT_READY = 0x80101903,
	SCE_SHELLAUDIO_ERROR_INVALID_ARG_2 = 0x80100E00
} SceShellSvcAudioErrorCodes;

/* SceShell (for SceShell plugins only) */

/**
 * Initialize ShellAudio for SceShell.
 *
 */
void shellAudioInitializeForShell(void);

/* Application BGM */

/**
 * Initialize ShellAudio for BGM.
 *
 * @param[in] init_type - 0 for BGM proxy, 1 for SceShell
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioInitializeForBGM(int init_type);

/**
 * Finish ShellAudio for BGM.
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioFinishForBGM(void);

/**
 * Set path to audio file for BGM.
 *
 * @param[in] path - path to audio file
 * @param[in] path - pointer to ::SceShellSvcAudioCustomOpt struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetAudioForBGM(char* path, SceShellSvcAudioCustomOpt* optParams);

/**
 * Set audio volume for BGM.
 *
 * @param[in] volume - audio volume (max is 32768)
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetVolumeForBGM(unsigned int volume);

/**
 * Set param2 for BGM.
 *
 * @param[in] param - param2
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetParam2ForBGM(int8_t param);

/**
 * Send command for BGM.
 *
 * @param[in] commandId - one of ::SceShellSvcAudioCommands
 * @param[in] param_2 - unknown param, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSendCommandForBGM(int commandId, int param_2);

/* Music Player */

/**
 * Initialize ShellAudio for music player.
 *
 * @param[in] init_type - unknown, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioInitializeForMusicPlayer(int init_type);

/**
 * Finish ShellAudio for BGM.
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioFinishForMusicPlayer(void);

/**
 * Get Sqlite commands buffer for music player.
 *
 * @param[out] infoBuffer - pointer to buffer to store output, size is 0x828
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioGetSqliteBufferForMusicPlayer(void* infoBuffer);

/**
 * Get track metadata for music player.
 *
 * @param[out] infoBuffer - pointer to ::SceShellSvcAudioMetadata struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioGetMetadataForMusicPlayer(SceShellSvcAudioMetadata* infoBuffer);

/**
 * Get playback status for music player.
 *
 * @param[out] infoBuffer - pointer to ::SceShellSvcAudioPlaybackStatus struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioGetPlaybackStatusForMusicPlayer(SceShellSvcAudioPlaybackStatus* infoBuffer);

/**
 * Get some unknown status for music player.
 *
 * @param[out] status - pointer to ::SceShellSvcAudioMusicPlayerStatus struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioGetStatusForMusicPlayer(SceShellSvcAudioMusicPlayerStatus* status);

/**
 * Send command for music player.
 *
 * @param[in] commandId - one of ::SceShellSvcAudioCommands
 * @param[in] param_2 - unknown param, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSendCommandForMusicPlayer(int commandId, int param_2);

/**
 * Set time to seek for music player, issue before shellAudioSendCommandForMusicPlayer().
 *
 * @param[in] time - time to seek in milliseconds
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetSeekTimeForMusicPlayer(unsigned int time);

/**
 * Seek for music player, performed automatically.
 *
 * @param[in] time - time to seek in milliseconds
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioAutoSeekForMusicPlayer(unsigned int time);

/**
 * Set repeat mode for music player.
 *
 * @param[in] mode - one of ::SceShellSvcAudioRepeatMode
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetRepeatModeForMusicPlayer(int mode);

/**
 * Set shuffle mode for music player.
 *
 * @param[in] mode - one of ::SceShellSvcShuffleRepeatMode
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetShuffleModeForMusicPlayer(int mode);

/**
 * Set EQ mode for music player.
 *
 * @param[in] mode - one of ::SceShellSvcAudioEQMode
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetEQModeForMusicPlayer(int mode);

/**
 * Set ALC (Dynamic normalizer) mode for music player.
 *
 * @param[in] mode - one of ::SceShellSvcAudioALCMode
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetALCModeForMusicPlayer(int mode);

/**
 * Set audio volume for music player. This function writes to registry.
 *
 * @param[in] volume - audio volume (max is 32768)
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetVolumeForMusicPlayer(unsigned int volume);

/**
 * Lock music player controls and playback until application is terminated for music player.
 *
 * @param[in] param - unknown param, set to 1
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioLockForMusicPlayer(int8_t param);

/**
 * Send Sqlite commands buffer for music player.
 *
 * @param[in] infoBuffer - pointer to sqlite commands buffer, size is 0x828
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSendSqliteBufferForMusicPlayer(void* infoBuffer, uint16_t param);

/**
 * Set path to audio file for music player.
 *
 * @param[in] path - path to audio file
 * @param[in] path - pointer to ::SceShellSvcAudioCustomOpt struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioSetAudioForMusicPlayer(char* path, SceShellSvcAudioCustomOpt* optParams);

/**
 * Get current bgm status.
 *
 * @param[out] status - pointer to ::SceShellAudioBGMState struct
 *
 * @return SCE_OK, <0 on error.
 */
int shellAudioGetCurrentBGMState(SceShellAudioBGMState* status);

#ifdef __cplusplus
}
#endif

#endif
