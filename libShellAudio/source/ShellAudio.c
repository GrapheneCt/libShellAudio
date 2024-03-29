﻿#include <kernel.h>
#include <shellsvc.h> 

#include "ShellAudio.h"

typedef struct SceShellSvcCustomAudioSubParams1 {
	int unk_00; // not used (set to 0)
	int tracking1;
	int tracking2;
} SceShellSvcCustomAudioSubParams1;

typedef struct SceShellSvcCustomAudioSubParams2 {
	int flag;
	int unk_04;
	int unk_08;
	int unk_0C;
} SceShellSvcCustomAudioSubParams2;

typedef struct SceShellSvcCustomAudioSubParams3 {
	int param1;
	int param2;
} SceShellSvcCustomAudioSubParams3;

typedef struct SceShellSvcCustomAudioParams {
	void* params1;  	// optional params1
	SceSize params1Size; 	// size of optional params1
	void* params2; 		// optional params2
	SceSize params2Size;	// size of optional params2
	void* params3;		// optional params3
	SceSize params3Size;	// size of optional params3
	void* params4;		// optional params4
	SceSize params4Size;	// size of optional params4
} SceShellSvcAudioCustomParams;

typedef struct SceShellSvcTable {
	void *pFunc_0x00;
	void *pFunc_0x04;
	void *pFunc_0x08;
	void *pFunc_0x0C;
	void *pFunc_0x10;
	int(*sceShellSvcAudioControl)(void *obj, int flag, SceShellSvcAudioCustomParams*, int numOfArg, int *pRes,SceShellSvcAudioCustomParams*, int numOfOut);
	void *pFunc_0x18;
	int(*sceShellSvcAsyncMethod)(void *obj, int asyncMethodId /* more args here */);

	// more ...
} SceShellSvcTable;

extern int sceAppMgrSetBgmProxyApp(const char*);
extern int sceAppMgrGetCurrentBgmState2(SceShellAudioBGMState*);

static int isInitialized = 0;
static int isReady = 0;

static int global_1 = 0;
static int global_2 = 0;

/* events
0xD0000 - init BGM
0xD0002 - set custom BGM
0xD0006 - set audio volume
0xD0007 - set repeat mode for BGM
0xD0004 - send command (play, pause, seek etc.) for BGM

0x30000 - init Music Player (called 1 time after app starts)
0x30001 - finish Music Player
0x30003 - get some sqlite buffer
0x30004 - set custom music
0x30006 - send command (play, pause, seek etc.)
0x30007 - get playback-related stats (called in a loop)
0x30008 - set audio volume
0x30009 - set repeat mode
0x3000A - set shuffle mode
0x3000B - set seek time
0x3000E - auto seek
0x3000F - set ALC mode
0x30010 - set EQ mode
0x30011 - ???, (called when you attempt to switch tracks, but there is no next song)
0x30013 - lock music player
0x30015 - ??? (is in Music Player code, but never actually called)
0x30016 - ??? (is in Music Player code, but never actually called)
0x30017 - ??? (is in Music Player code, but never actually called)
0x30018 - send 0x30003
0x3001A - ???, always called in a loop
0x3001B - get something? (called 1 time after app starts, 2 times when switching to player UI, every time next track is selected)
*/

int shellAudioFinishInternal(int eventId)
{
	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	int res = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 1, &res, 0, 0);

	if (ret != 0)
		res = ret;

	return res;
}

int shellAudioSendCommandInternal(int eventId, int commandId, int param_2)
{
	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcCustomAudioSubParams3 params2;
	params2.param1 = commandId;
	params2.param2 = param_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params2;
	mainParams.params1Size = 0x8;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	int res = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &res, 0, 0);

	if (ret != 0)
		res = ret;

	return res;
}

int shellAudioSetParamInternal(int eventId, unsigned int param)
{
	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x4;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &res, 0, 0);

	if (ret != 0)
		res = ret;

	return res;
}

int shellAudioSetParam2Internal(int eventId, int8_t param)
{
	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x1;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	int res = 0;

	int ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &res, 0, 0);

	if (ret != 0)
		res = ret;

	return res;
}

int sceMusicInternalAppInitialize(int init_type)
{
	int ret;

	if (isInitialized) {
		ret = SCE_MUSIC_ERROR_ALREADY_INITIALIZED;
		goto end;
	}

	if (init_type != 0 && init_type != 1) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;

		goto end;
	}

	if (init_type == 0)
		sceAppMgrSetBgmProxyApp("NPXS19999");

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = init_type;
	params1.tracking2 = 0;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0000, &mainParams, 1, &res, 0, 0);

	if (ret != 0)
		goto end;

	global_1 = params1.tracking1;
	global_2 = params1.tracking2;
	isInitialized = 1;
	isReady = 0;

	return res;

end:

	return ret;
}

int sceMusicInternalAppTerminate(void)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	ret = shellAudioFinishInternal(0xD0001);

	if (ret != 0)
		goto end;

	global_1 = 0;
	global_2 = 0;
	isInitialized = 0;
	isReady = 0;

end:

	return ret;
}

int sceMusicInternalAppSetUri(char* path, SceMusicOpt* optParams)
{
	int ret, pathlen;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (path == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	if (optParams->param1 != 0 && optParams->param1 != 1) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	pathlen = sceClibStrnlen(path, 0x401);

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcCustomAudioSubParams2 params2;
	sceClibMemset(&params2, 0, 0x10);

	if (optParams->flag != 0) {
		sceClibMemcpy(&params2, optParams, 0x10);
	}

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = path;
	mainParams.params2Size = pathlen;
	mainParams.params3 = &params2;
	mainParams.params3Size = 0x10;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0002, &mainParams, 3, &res, 0, 0);

	if (ret != 0)
		goto end;

	isReady = 1;

	return res;

end:

	return ret;
}

int sceMusicInternalAppSetUriExt(char* path, SceMusicOpt* optParams, unsigned short extArg)
{
	int ret, pathlen;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (path == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	if (optParams->param1 != 0 && optParams->param1 != 1) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	pathlen = sceClibStrnlen(path, 0x401);

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcCustomAudioSubParams2 params2;
	sceClibMemset(&params2, 0, 0x10);

	if (optParams->flag != 0) {
		sceClibMemcpy(&params2, optParams, 0x10);
	}

	unsigned short extParam = extArg;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &extParam;
	mainParams.params1Size = 0x2;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;
	mainParams.params3 = path;
	mainParams.params3Size = pathlen;
	mainParams.params4 = &params2;
	mainParams.params4Size = 0x10;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD000D, &mainParams, 4, &res, 0, 0);

	if (ret != 0)
		goto end;

	isReady = 1;

	return res;

end:

	return ret;
}

int sceMusicInternalAppSetVolume(unsigned int volume)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (volume > 0x8000) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &volume;
	mainParams.params1Size = 0x4;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0006, &mainParams, 2, &res, 0, 0);

	if (ret != 0)
		goto end;

	return res;

end:

	return ret;
}

int sceMusicInternalAppSetRepeatMode(int mode)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (mode != 0 && mode != 1 && mode != 2) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	ret = shellAudioSetParam2Internal(0xD0007, mode);

end:

	return ret;
}

int sceMusicInternalAppSetPlaybackCommand(int eventId, int param_2)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (!isReady) {
		ret = SCE_MUSIC_ERROR_NOT_READY;
		goto end;
	}

	if (eventId > 0x11 || eventId < 0) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	ret = shellAudioSendCommandInternal(0xD0004, eventId, param_2);

end:

	return ret;
}

int sceMusicInternalAppGetLastResult(SceMusicInternalAppResult *resBuffer)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (resBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	char buffer[0x8];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD000E, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(resBuffer, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicInternalAppGetPlaybackStatus(void *resBuffer)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_MUSIC_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (resBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_ARG;
		goto end;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	char buffer[0x40];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0005, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(resBuffer, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceInitialize(int init_type)
{
	int ret;

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = 1;

	switch (init_type) {
	case 0:
		params1.tracking2 = 0x8;
		break;
	case 1:
		params1.tracking2 = 0x10;
		break;
	case 2:
		params1.tracking2 = 0x20;
		break;
	case 3:
		params1.tracking2 = 0x40;
		break;
	case 4:
		params1.tracking2 = 0x80;
		break;
	default:
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
		break;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30000, &mainParams, 1, &res, 0, 0);

	if (ret != 0)
		goto end;

	global_1 = params1.tracking1;
	global_2 = params1.tracking2;

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceTerminate(void)
{
	int ret = shellAudioFinishInternal(0x30001);

	if (ret != 0)
		goto end;

	global_1 = 0;
	global_2 = 0;

end:

	return ret;
}

int sceMusicPlayerServiceGetTrackList(void* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x828];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30003, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceGetTrackInfo(SceMusicPlayerServiceTrackInfo* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x1810];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x3001B, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceGetPlayStatusExtension(SceMusicPlayerServicePlayStatusExtension* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x70];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30007, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceGetCurrentPlayStatus(SceMusicPlayerServicePlayStatus* status)
{
	int ret;

	if (status == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0xC];
	sceClibMemset(buffer, 0, sizeof(buffer));

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioCustomParams outputInfo;
	outputInfo.params1 = buffer;
	outputInfo.params1Size = sizeof(buffer);

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x3001A, &mainParams, 1, &res, &outputInfo, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(status, buffer, sizeof(buffer));

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceSendEvent(int eventId, int param_2)
{
	int ret;

	if (eventId > 0x11 || eventId < 0) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	ret = shellAudioSendCommandInternal(0x30006, eventId, param_2);

end:

	return ret;
}

int sceMusicPlayerServiceSetSeekTime(unsigned int time)
{
	return shellAudioSetParamInternal(0x3000B, time);
}

int sceMusicPlayerServiceSeek(unsigned int time)
{
	return shellAudioSetParamInternal(0x3000E, time);
}

int sceMusicPlayerServiceSetRepeatMode(int mode)
{
	int ret;

	if (mode == SCE_MUSIC_REPEAT_DISABLE)
		mode = 3;

	if (mode != 0 && mode != 1 && mode != 3) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x30009, mode);

end:

	return ret;
}

int sceMusicPlayerServiceSetShuffle(int mode)
{
	int ret;

	if (mode != 1 && mode != 2) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x3000A, mode);

end:

	return ret;
}

int sceMusicPlayerServiceSetEQ(int mode)
{
	int ret;

	if (mode > 0x4 || mode < 0) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x30010, mode);

end:

	return ret;
}

int sceMusicPlayerServiceSetALC(int mode)
{
	int ret;

	if (mode != 0 && mode != 1) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x3000F, mode);

end:

	return ret;
}

int sceMusicPlayerServiceSetAudioVolume(unsigned int volume)
{
	int ret;

	if (volume > 0x8000) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &volume;
	mainParams.params1Size = 0x4;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30008, &mainParams, 2, &res, 0, 0);

	if (ret != 0)
		goto end;

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceLock(int8_t param)
{
	return shellAudioSetParam2Internal(0x30013, param);
}

int sceMusicPlayerServiceSetTrackList(void* infoBuffer, uint16_t param)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x828];
	sceClibMemcpy(buffer, infoBuffer, sizeof(buffer));

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x2;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;
	mainParams.params3 = buffer;
	mainParams.params3Size = sizeof(buffer);

	void* tptr = sceShellSvcGetSvcObj();

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30018, &mainParams, 3, &res, 0, 0);

	if (ret != 0)
		goto end;

	return res;

end:

	return ret;
}

int sceMusicPlayerServiceOpen(char* path, SceMusicOpt* optParams)
{
	int ret, pathlen;

	if (path == NULL) {
		ret = SCE_MUSIC_ERROR_INVALID_SERVICE_ARG;
		goto end;
	}

	pathlen = sceClibStrnlen(path, 0x401);

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	SceShellSvcCustomAudioSubParams2 params2;
	sceClibMemset(&params2, 0, 0x10);

	if (optParams != NULL) {
		sceClibMemcpy(&params2, optParams, 0x10);
	}

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = path;
	mainParams.params2Size = pathlen;
	mainParams.params3 = &params2;
	mainParams.params3Size = 0x10;

	int res = 0;

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30004, &mainParams, 3, &res, 0, 0);

	if (ret != 0)
		goto end;

	return res;

end:

	return ret;
}

int shellAudioGetCurrentBGMState(SceShellAudioBGMState* status)
{
	return sceAppMgrGetCurrentBgmState2(status);
}

#ifdef SHELLAUDIO_PRX
int __module_stop(SceSize argc, const void *args) {
	sceClibPrintf("ShellAudio module stop\n");
	return SCE_KERNEL_STOP_SUCCESS;
}

int __module_exit() {
	sceClibPrintf("ShellAudio module exit\n");
	return SCE_KERNEL_STOP_SUCCESS;
}

int __module_start(SceSize argc, void *args) {
	sceClibPrintf("ShellAudio module start, ver. 01.20\n");
	return SCE_KERNEL_START_SUCCESS;
}
#endif
