#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "CPatch.h"

#include "dllmain.h"

using namespace std;

void DisplayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//cout << "Mod debug window started" << endl;
}

double GetPrivateProfileFloat(const char* lpAppName, const char*  lpKeyName, const char*  lpDefault, const char*  lpFileName){
	char buf[255];
	GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, buf, sizeof(buf), lpFileName);
	return atof(buf);
}

char* lpFileName = ".\\RnR_NewGameMod.ini";

char vehicleID[128];
int  vehicleColor;

double startPosX;
double startPosY;
double startPosZ;

void updateVariables(){
	GetPrivateProfileStringA("MOD", "vehicle_ID", "FREIGHTLINER_ARGOSY", (LPSTR)vehicleID, 128, lpFileName);
	vehicleColor = GetPrivateProfileIntA("MOD", "vehicle_color", 0, lpFileName); 

	startPosX = GetPrivateProfileFloat("MOD", "startPosX", "5390", lpFileName);
	startPosY = GetPrivateProfileFloat("MOD", "startPosY", "-23870", lpFileName);
	startPosZ = GetPrivateProfileFloat("MOD", "startPosZ", "16", lpFileName);

	cout << "Variables UPDATED:\n";
	cout << "vehicle ID   =" << vehicleID << endl;
	cout << "vehicle color=" << vehicleColor << endl;
	cout << "startposition=" << startPosX << " " << startPosY << " " << startPosZ << endl;
}

//find vehicle id for singlePlayer?
int __cdecl sub_12BA380(const char *a1, int a2){
	typedef int (__cdecl * sub_12BA380)(const char *a1, int a2);
	return sub_12BA380(0x12BA380)(a1, a2);
}

int __cdecl hook_1287240(const char *a1, int a2){
	return sub_12BA380(vehicleID, vehicleColor);
	//return sub_12BA380(a1, a2);
}

const char *createPlayersRegular(){
	typedef const char* (*createPlayersRegular)();
	return createPlayersRegular(0x125CD30)();
}

double* __cdecl sub_858B30(double *a1){
	typedef double* (*sub_858B30)(double *a1);
	return sub_858B30(0x858B30)(a1);
}

double* __cdecl hook_858B30(double* a1){
	*(double*)0x21FBAA0 = startPosX;
	*(double*)0x21FBAA8 = startPosY;
	*(double*)0x21FBAB0 = startPosZ;

	//*(double*)0x1C3BD70 = startPosX;
	//*(double*)0x1C3BD78 = startPosY;
	//*(double*)0x1C3BD80 = startPosZ;



	return sub_858B30(a1);
}

//const char *hook_1265863(){
//	return createPlayersRegular();
//}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//vehicle ID
			CPatch::RedirectCall(0x1287240, &hook_1287240);

			//startposition
			//CPatch::RedirectCall(0x125F108, &hook_125F108);
			//CPatch::RedirectCall(0x125F16B, &hook_125F16B);
			//CPatch::RedirectCall(0x125F1E2, &hook_125F1E2);

			if (GetPrivateProfileIntA("MOD", "use_new_position", 0, lpFileName)){
				CPatch::SetPointer(0x858B7C, &startPosX);
				CPatch::SetPointer(0x858B85, &startPosY);
				CPatch::SetPointer(0x858B8E, &startPosZ);

				CPatch::RedirectCall(0x1259F3A, &hook_858B30);
			}

			//update variables before game start
			//CPatch::RedirectCall(0x1265863, &hook_1265863);

			DisplayConsole();
			cout << "NewGameMod v0.01 activated (22.11.2025)" << endl;
			cout << "vk.com/rnr_mods" << endl;

			updateVariables();

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	/* Return TRUE on success, FALSE on failure */
	return TRUE;
}