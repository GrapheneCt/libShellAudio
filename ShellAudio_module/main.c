﻿#include <psp2/kernel/modulemgr.h> 
#include <psp2/kernel/clib.h>
#include <psp2/types.h> 
#include <psp2/shellsvc.h> 

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
	void* params2; 		// optional params2, ex. path to audio file
	SceSize params2Size;	// size of optional params2
	void* params3;		// optional params3
	SceSize params3Size;	// size of optional params3
} SceShellSvcAudioCustomParams;

typedef struct SceShellSvcAudioOutput {
	void* outPtr1; // usually NULL
	void* outPtr2;  // usually NULL
} SceShellSvcAudioOutput;

typedef struct SceShellSvcTable {
	void *pFunc_0x00;
	void *pFunc_0x04;
	void *pFunc_0x08;
	void *pFunc_0x0C;
	void *pFunc_0x10;
	int(*sceShellSvcAudioControl)(void *obj, int flag, SceShellSvcAudioCustomParams*, int a4_flag, SceShellSvcAudioOutput*, void *outInfo, int isOutInfoUsed);
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
0xD0007 - set some param2 for BGM
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	return ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 1, &outputInfo, 0, 0);
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	return ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &outputInfo, 0, 0);
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	void* tptr = sceShellSvcGetSvcObj();

	return ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &outputInfo, 0, 0);
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	return ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, eventId, &mainParams, 2, &outputInfo, 0, 0);
}

int shellAudioInitializeForBGM(int init_type)
{
	int ret;

	if (isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_ALREADY_INITIALIZED;
		goto end;
	}

	if (init_type != 0 && init_type != 1) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;

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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0000, &mainParams, 1, &outputInfo, 0, 0);

	if (ret != 0)
		goto end;

	global_1 = params1.tracking1;
	global_2 = params1.tracking2;
	isInitialized = 1;
	isReady = 0;

end:

	return ret;
}

int shellAudioFinishForBGM(void)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
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

int shellAudioSetAudioForBGM(char* path, SceShellSvcAudioCustomOpt* optParams)
{
	int ret, pathlen;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (path == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	if (optParams->param1 != 0 && optParams->param1 != 1) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0002, &mainParams, 3, &outputInfo, 0, 0);

	if (ret != 0)
		goto end;

	isReady = 1;

end:

	return ret;
}

int shellAudioSetVolumeForBGM(unsigned int volume)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (volume > 0x8000) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0xD0006, &mainParams, 2, &outputInfo, 0, 0);

end:

	return ret;
}

int shellAudioSetParam2ForBGM(int8_t param)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (param != 0 && param != 1 && param != 2) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	ret = shellAudioSetParam2Internal(0xD0007, param);

end:

	return ret;
}

int shellAudioSendCommandForBGM(int commandId, int param_2)
{
	int ret;

	if (!isInitialized) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_INITIALIZED;
		goto end;
	}

	if (!isReady) {
		ret = SCE_SHELLAUDIO_ERROR_NOT_READY;
		goto end;
	}

	if (commandId > 0x11 || commandId < 0) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
		goto end;
	}

	ret = shellAudioSendCommandInternal(0xD0004, commandId, param_2);

end:

	return ret;
}

int shellAudioInitializeForMusicPlayer(int init_type)
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
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
		break;
	}

	void* tptr = sceShellSvcGetSvcObj();

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30000, &mainParams, 1, &outputInfo, 0, 0);

	if (ret != 0)
		goto end;

	global_1 = params1.tracking1;
	global_2 = params1.tracking2;

end:

	return ret;
}

int shellAudioFinishForMusicPlayer(void)
{
	int ret = shellAudioFinishInternal(0x30001);

	if (ret != 0)
		goto end;

	global_1 = 0;
	global_2 = 0;

end:

	return ret;
}

int shellAudioGetSqliteBufferForMusicPlayer(void* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x828];
	sceClibMemset(&buffer, 0, 0x828);

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = &buffer;
	mainParams.params2Size = 0x828;

	SceShellSvcAudioOutput outputInfo;
	outputInfo.outPtr1 = NULL;
	outputInfo.outPtr2 = &mainParams.params2;

	void* tptr = sceShellSvcGetSvcObj();

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30003, &mainParams, 1, &outputInfo, &mainParams.params2, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, &buffer, 0x828);

end:

	return ret;
}

int shellAudioGetMetadataForMusicPlayer(SceShellSvcAudioMetadata* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x1810];
	sceClibMemset(&buffer, 0, 0x1810);

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = &buffer;
	mainParams.params2Size = 0x1810;

	SceShellSvcAudioOutput outputInfo;
	outputInfo.outPtr1 = NULL;
	outputInfo.outPtr2 = &mainParams.params2;

	void* tptr = sceShellSvcGetSvcObj();

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x3001B, &mainParams, 1, &outputInfo, &mainParams.params2, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, &buffer, 0x1810);

end:

	return ret;
}

int shellAudioGetPlaybackStatusForMusicPlayer(SceShellSvcAudioPlaybackStatus* infoBuffer)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x70];
	sceClibMemset(&buffer, 0, 0x70);

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = &buffer;
	mainParams.params2Size = 0x70;

	SceShellSvcAudioOutput outputInfo;
	outputInfo.outPtr1 = NULL;
	outputInfo.outPtr2 = &mainParams.params2;

	void* tptr = sceShellSvcGetSvcObj();

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30007, &mainParams, 1, &outputInfo, &mainParams.params2, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(infoBuffer, &buffer, 0x70);

end:

	return ret;
}

int shellAudioGetStatusForMusicPlayer(SceShellSvcAudioMusicPlayerStatus* status)
{
	int ret;

	if (status == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0xC];
	sceClibMemset(&buffer, 0, 0xC);

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &params1;
	mainParams.params1Size = 0xC;
	mainParams.params2 = &buffer;
	mainParams.params2Size = 0xC;

	SceShellSvcAudioOutput outputInfo;
	outputInfo.outPtr1 = NULL;
	outputInfo.outPtr2 = &mainParams.params2;

	void* tptr = sceShellSvcGetSvcObj();

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x3001A, &mainParams, 1, &outputInfo, &mainParams.params2, 1);

	if (ret != 0)
		goto end;

	sceClibMemcpy(status, &buffer, 0xC);

end:

	return ret;
}

int shellAudioSendCommandForMusicPlayer(int commandId, int param_2)
{
	int ret;

	if (commandId > 0x11 || commandId < 0) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	ret = shellAudioSendCommandInternal(0x30006, commandId, param_2);

end:

	return ret;
}

int shellAudioSetSeekTimeForMusicPlayer(unsigned int time)
{
	return shellAudioSetParamInternal(0x3000B, time);
}

int shellAudioAutoSeekForMusicPlayer(unsigned int time)
{
	return shellAudioSetParamInternal(0x3000E, time);
}

int shellAudioSetRepeatModeForMusicPlayer(int mode)
{
	int ret;

	if (mode == SCE_SHELLAUDIO_REPEAT_DISABLE)
		mode = 3;

	if (mode != 0 && mode != 1 && mode != 3) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x30009, mode);

end:

	return ret;
}

int shellAudioSetShuffleModeForMusicPlayer(int mode)
{
	int ret;

	if (mode != 1 && mode != 2) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x3000A, mode);

end:

	return ret;
}

int shellAudioSetEQModeForMusicPlayer(int mode)
{
	int ret;

	if (mode > 0x4 || mode < 0) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x30010, mode);

end:

	return ret;
}

int shellAudioSetALCModeForMusicPlayer(int mode)
{
	int ret;

	if (mode != 0 && mode != 1) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	ret = shellAudioSetParamInternal(0x3000F, mode);

end:

	return ret;
}

int shellAudioSetVolumeForMusicPlayer(unsigned int volume)
{
	int ret;

	if (volume > 0x8000) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG;
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30008, &mainParams, 2, &outputInfo, 0, 0);

end:

	return ret;
}

int shellAudioLockForMusicPlayer(int8_t param)
{
	return shellAudioSetParam2Internal(0x30013, param);
}

int shellAudioSendSqliteBufferForMusicPlayer(void* infoBuffer, uint16_t param)
{
	int ret;

	if (infoBuffer == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
		goto end;
	}

	SceShellSvcCustomAudioSubParams1 params1;
	params1.unk_00 = 0;
	params1.tracking1 = global_1;
	params1.tracking2 = global_2;

	char buffer[0x828];
	sceClibMemset(&buffer, 0, 0x828);
	sceClibMemcpy(&buffer, infoBuffer, 0x828);

	SceShellSvcAudioCustomParams mainParams;
	mainParams.params1 = &param;
	mainParams.params1Size = 0x2;
	mainParams.params2 = &params1;
	mainParams.params2Size = 0xC;
	mainParams.params3 = &buffer;
	mainParams.params3Size = 0x828;

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	void* tptr = sceShellSvcGetSvcObj();

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30018, &mainParams, 3, &outputInfo, 0, 0);

end:

	return ret;
}

int shellAudioSetAudioForMusicPlayer(char* path, SceShellSvcAudioCustomOpt* optParams)
{
	int ret, pathlen;

	if (path == NULL) {
		ret = SCE_SHELLAUDIO_ERROR_INVALID_ARG_2;
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

	SceShellSvcAudioOutput outputInfo;
	sceClibMemset(&outputInfo, 0, 0x8);

	ret = ((SceShellSvcTable *)(*(uint32_t *)tptr))->sceShellSvcAudioControl(tptr, 0x30004, &mainParams, 3, &outputInfo, 0, 0);

end:

	return ret;
}

void shellAudioInitializeForShell(void)
{
	isInitialized = 1;
}

int shellAudioGetCurrentBGMState(SceShellAudioBGMState* status)
{
	return sceAppMgrGetCurrentBgmState2(status);
}

int module_stop(SceSize argc, const void *args) {
	sceClibPrintf("ShellAudio module stop\n");
	return SCE_KERNEL_STOP_SUCCESS;
}

int module_exit() {
	sceClibPrintf("ShellAudio module exit\n");
	return SCE_KERNEL_STOP_SUCCESS;
}

void _start() __attribute__((weak, alias("module_start")));
int module_start(SceSize argc, void *args) {
	sceClibPrintf("ShellAudio module start, ver. 01.10\n");
	return SCE_KERNEL_START_SUCCESS;
}
