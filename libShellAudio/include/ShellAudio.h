﻿#ifndef SHELLAUDIO_H
#define SHELLAUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SHELLAUDIO_PRX)
#define PRX_INTERFACE __declspec (dllexport)
#else
#define PRX_INTERFACE
#endif

typedef struct SceMusicOpt {
	int flag; // set to 0 if not used
	int param1;
	int param2;
	int param3;
} SceMusicOpt;

typedef struct SceMusicInternalAppResult {
	int state;
	int time;
} SceMusicInternalAppResult;

typedef struct SceMusicPlayerServicePlayStatus {
	int stat1;
	int stat2;
	int stat3;
} SceMusicPlayerServicePlayStatus;

typedef struct SceMusicPlayerServicePlayStatusExtension {
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
} SceMusicPlayerServicePlayStatusExtension;

typedef struct SceShellAudioBGMState {
	int bgmPortOwnerId;
	int bgmPortStatus;
	int someStatus1;
	int currentState;
	int someStatus2;
} SceShellAudioBGMState;

typedef struct SceMusicPlayerServiceTrackInfo {
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
} SceMusicPlayerServiceTrackInfo;

typedef enum SceMusicEventId {
	SCE_MUSIC_EVENTID_DEFAULT = 0,
	SCE_MUSIC_EVENTID_PLAY = 0x1,
	SCE_MUSIC_EVENTID_STOP = 0x2,
	SCE_MUSIC_EVENTID_NEXT = 0x3,
	SCE_MUSIC_EVENTID_PREVIOUS = 0x4,
	SCE_MUSIC_EVENTID_SEEK = 0x11
} SceMusicEventId;

typedef enum SceMusicRepeatMode {
	SCE_MUSIC_REPEAT_ALL,
	SCE_MUSIC_REPEAT_ONE,
	SCE_MUSIC_REPEAT_DISABLE
} SceMusicRepeatMode;

typedef enum SceMusicEQMode {
	SCE_MUSIC_EQ_DISABLE,
	SCE_MUSIC_EQ_HEAVY,
	SCE_MUSIC_EQ_POP,
	SCE_MUSIC_EQ_JAZZ,
	SCE_MUSIC_EQ_UNIQUE
} SceMusicEQMode;

typedef enum SceMusicShuffleMode {
	SCE_MUSIC_SHUFFLE_DISABLE = 0x1,
	SCE_MUSIC_SHUFFLE_ENABLE
} SceMusicShuffleMode;

typedef enum SceMusicALCMode {
	SCE_MUSIC_ALC_DISABLE,
	SCE_MUSIC_ALC_ENABLE
} SceMusicALCMode;

/* Error codes */

typedef enum SceMusicErrorCodes {
	SCE_MUSIC_ERROR_INVALID_ARG = 0x80101900,
	SCE_MUSIC_ERROR_NOT_INITIALIZED = 0x80101901,
	SCE_MUSIC_ERROR_ALREADY_INITIALIZED = 0x80101902,
	SCE_MUSIC_ERROR_NOT_READY = 0x80101903,
	SCE_MUSIC_ERROR_INVALID_SERVICE_ARG = 0x80100E00
} SceMusicErrorCodes;

/* Application BGM */

/**
 * Initialize custom BGM music service.
 *
 * @param[in] init_type - 0 for BGM proxy, 1 for SceShell
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppInitialize(int init_type);

/**
 * Terminate custom BGM music service.
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppTerminate(void);

/**
 * Set path to audio file for BGM.
 *
 * @param[in] path - path to audio file
 * @param[in] path - pointer to ::SceMusicOpt struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppSetUri(char* path, SceMusicOpt* optParams);

/**
 * Set path to audio file for BGM with ext arg.
 *
 * @param[in] path - path to audio file
 * @param[in] path - pointer to ::SceMusicOpt struct
 * @param[in] extArg - extra arg
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppSetUriExt(char* path, SceMusicOpt* optParams, unsigned short extArg);

/**
 * Set audio volume for BGM.
 *
 * @param[in] volume - audio volume (max is 32768)
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppSetVolume(unsigned int volume);

/**
 * Set repeat mode for BGM.
 *
 * @param[in] mode - repeat mode
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppSetRepeatMode(int mode);

/**
 * Send event for BGM.
 *
 * @param[in] eventId - one of ::SceMusicEventId
 * @param[in] param_2 - unknown param, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppSetPlaybackCommand(int eventId, int param_2);

/**
 * Get operation result for BGM.
 *
 * @param[in] resBuffer - pointer to ::SceMusicInternalAppResult struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppGetLastResult(SceMusicInternalAppResult *resBuffer);

/**
 * Get playback status for BGM.
 *
 * @param[in] resBuffer - pointer to playback info struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicInternalAppGetPlaybackStatus(void *resBuffer);

/* Music Player */

/**
 * Initialize music player service.
 *
 * @param[in] init_type - unknown, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceInitialize(int init_type);

/**
 * Terminate music player service.
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceTerminate(void);

/**
 * Get Sqlite track list buffer for music player.
 *
 * @param[out] infoBuffer - pointer to buffer to store output, size is 0x828
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceGetTrackList(void* infoBuffer);

/**
 * Get track metadata for music player.
 *
 * @param[out] infoBuffer - pointer to ::SceMusicPlayerServiceTrackInfo struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceGetTrackInfo(SceMusicPlayerServiceTrackInfo* infoBuffer);

/**
 * Get playback status for music player.
 *
 * @param[out] infoBuffer - pointer to ::SceMusicPlayerServicePlayStatus struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceGetPlayStatusExtension(SceMusicPlayerServicePlayStatusExtension* infoBuffer);

/**
 * Get some unknown status for music player.
 *
 * @param[out] status - pointer to ::SceShellSvcAudioMusicPlayerStatus struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceGetCurrentPlayStatus(SceMusicPlayerServicePlayStatus* status);

/**
 * Send event for music player.
 *
 * @param[in] eventId - one of ::SceMusicCoreEventId
 * @param[in] param_2 - unknown param, set to 0
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSendEvent(int eventId, int param_2);

/**
 * Set time to seek for music player, issue before sceMusicPlayerServiceSendEvent().
 *
 * @param[in] time - time to seek in milliseconds
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetSeekTime(unsigned int time);

/**
 * Seek for music player, performed automatically.
 *
 * @param[in] time - time to seek in milliseconds
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSeek(unsigned int time);

/**
 * Set repeat mode for music player.
 *
 * @param[in] mode - one of ::SceMusicPlayerServiceRepeatMode
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetRepeatMode(int mode);

/**
 * Set shuffle mode for music player.
 *
 * @param[in] mode - one of ::SceMusicPlayerServiceShuffleMode
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetShuffle(int mode);

/**
 * Set EQ mode for music player.
 *
 * @param[in] mode - one of ::SceMusicPlayerServiceEQMode
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetEQ(int mode);

/**
 * Set ALC (Dynamic normalizer) mode for music player.
 *
 * @param[in] mode - one of ::SceMusicPlayerServiceALCMode
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetALC(int mode);

/**
 * Set audio volume for music player. This function writes to registry.
 *
 * @param[in] volume - audio volume (max is 32768)
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetAudioVolume(unsigned int volume);

/**
 * Lock music player controls and playback until application is terminated for music player.
 *
 * @param[in] param - unknown param, set to 1
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceLock(int8_t param);

/**
 * Send Sqlite track list buffer for music player.
 *
 * @param[in] infoBuffer - pointer to sqlite commands buffer, size is 0x828
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceSetTrackList(void* infoBuffer, uint16_t param);

/**
 * Set path to audio file for music player.
 *
 * @param[in] path - path to audio file
 * @param[in] path - pointer to ::SceShellSvcAudioCustomOpt struct
 *
 * @return SCE_OK, <0 on error.
 */
PRX_INTERFACE int sceMusicPlayerServiceOpen(char* path, SceMusicOpt* optParams);

#ifdef __cplusplus
}
#endif

#endif