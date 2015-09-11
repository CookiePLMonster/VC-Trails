﻿#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// Target XP
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include <cstdio>

#include "MemoryMgr.h"

struct SMenuEntry
{
	WORD	action;
	char	description[8];
	BYTE	style;
	BYTE	targetMenu;
	WORD	posX;
	WORD	posY;
	WORD	align;
};

static_assert(sizeof(SMenuEntry) == 0x12, "SMenuEntry error!");

const SMenuEntry	displaySettingsPatch[] = {
							/*Trails*/
							{ 9, "FED_TRA", 0, 4, 40, 153, 1 },
							/*Subtitles*/
							{ 10, "FED_SUB", 0, 4, 40, 178, 1 },
							/*Widescreen*/
							{ 11, "FED_WIS", 0, 4, 40, 202, 1 },
							/*Map Legend*/
							{ 31, "MAP_LEG", 0, 4, 40, 228, 1 },
							/*Radar Mode*/
							{ 32, "FED_RDR", 0, 4, 40, 253, 1 },
							/*HUD Mode*/
							{ 33, "FED_HUD", 0, 4, 40, 278, 1 },
							/*Resolution*/
							{ 43, "FED_RES", 0, 4, 40, 303, 1 },
							/*Restore Defaults*/
							{ 47, "FET_DEF", 0, 4, 320, 328, 3 },
							/*Back*/
							{ 34, "FEDS_TB", 0, 33, 320, 353, 3 }
					};

void LoadSetFile(unsigned char* pTrails)
{
	FILE* hFile = fopen("gta_vc.set", "rb");
	if ( hFile )
	{
		fseek(hFile, 0x5BA, SEEK_SET);
		fread(pTrails, 1, 1, hFile);
		if ( *pTrails == 's' )
			*pTrails = 0;
		fclose(hFile);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);
	if(reason == DLL_PROCESS_ATTACH)
	{
		using namespace MemoryVP;

		if( *(DWORD*)0x667BF5 == 0xB85548EC ) // VC 1.0
		{
			int				pMenuEntries = *(int*)0x4966A0 + 0x3C8;
			unsigned char*	pTrailsOptionEnabled = *(unsigned char**)0x48FEB0;

			Patch<const void*>(0x4902D2, pTrailsOptionEnabled);
			memcpy((void*)pMenuEntries, displaySettingsPatch, sizeof(displaySettingsPatch));

			((void(*)())0x48E020)();	// chdir user dir
			LoadSetFile(pTrailsOptionEnabled);
			((void(*)(const char*))0x48E030)("");	// chdir root dir
		}
		else if ( *(DWORD*)0x667C45 == 0xB85548EC )	// VC 1.1
		{
			int				pMenuEntries = *(int*)0x4966B0 + 0x3C8;
			unsigned char*	pTrailsOptionEnabled = *(unsigned char**)0x48FEC0;

			Patch<const void*>(0x4902E2, pTrailsOptionEnabled);
			memcpy((void*)pMenuEntries, displaySettingsPatch, sizeof(displaySettingsPatch));

			((void(*)())0x48E030)();	// chdir user dir
			LoadSetFile(pTrailsOptionEnabled);
			((void(*)(const char*))0x48E040)("");	// chdir root dir
		}
		else if ( *(DWORD*)0x666BA5 == 0xB85548EC ) // VC Steam
		{
			int				pMenuEntries = *(int*)0x4965B0 + 0x3C8;
			unsigned char*	pTrailsOptionEnabled = *(unsigned char**)0x48FDC0;

			Patch<const void*>(0x4901E2, pTrailsOptionEnabled);
			memcpy((void*)pMenuEntries, displaySettingsPatch, sizeof(displaySettingsPatch));

			((void(*)())0x48DF10)();	// chdir user dir
			LoadSetFile(pTrailsOptionEnabled);
			((void(*)(const char*))0x48DF20)("");	// chdir root dir
		}
	}
	return TRUE;
}