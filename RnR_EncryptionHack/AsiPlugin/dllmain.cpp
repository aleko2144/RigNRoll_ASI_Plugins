#include "dllmain.h"

#include <windows.h>
#include <iostream>
#include "CPatch.h"

using namespace std;

void DisplayConsole(){
	AllocConsole();
	freopen("conin$","r", stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

void *__cdecl DecryptPkqt(void *pkqtFile, int inputSize, void *outputPtr, void *internalKeyPtr, void *internalSignaturePtr){
	typedef void * (__cdecl * DecryptPkqt)(void *pkqtFile, int inputSize, void *outputPtr, void *internalKeyPtr, void *internalSignaturePtr);
	return DecryptPkqt(0x11C7E30)(pkqtFile, inputSize, outputPtr, internalKeyPtr, internalSignaturePtr);
}

char * copyData(void *_this, void *a2, void *a3, int a4, int a5){
	typedef char * (__thiscall * copyData)(void *_this, void *a2, void *a3, int a4, int a5);
	return copyData(0x40A660)(_this, a2, a3, a4, a5);
}

char is_in_retail_mode(){
	typedef char (* is_in_retail_mode)();
	return is_in_retail_mode(0x1359300)();
}

bool __cdecl is_rnr_encrypt_format(const char *a1, signed int a2){
	return true;
}

DWORD *__cdecl decryptionFunction(void *fileBufPtr, int fileSize, void *outputPtr){
	DWORD *result;

	signed int header = *(int*)fileBufPtr;
	
	//Rig'n'Roll encrypted files
	if (header == 33554432){
		//returns pointer to pointer to decoded data
		result = (DWORD *)DecryptPkqt((int *)fileBufPtr + 1, fileSize - 4, (void **)outputPtr, (void*)0x1BEB888, (void*)0x1BEBB50);
		return result;
	}

	//make a copy of source data from fileBufPtr to outputBufPtr?
	copyData(outputPtr, *((void **)outputPtr + 1), fileBufPtr, (int)fileBufPtr + fileSize, (int)&fileSize);
	result = *( DWORD **)outputPtr;
	
	//Rig'n'Roll Gold Edition simple XOR encrypted files
	if (header == -1777004219) {
		//decode data skipping first 4 bytes (header)
		int dataSize = fileSize - 4;
		for (int i = 0; i < dataSize; i++){
			BYTE* data = (BYTE*)result;
			data[i] = data[i+4] ^ 0xAAu;
		}

		//fill last 4 bytes with zero
		for (int k = 0; k < 4; k++){
			BYTE* data = (BYTE*)result;
			data[dataSize + k] = 0;
		}
		return result;
	}

	//otherwise file have no any encryption, so just return file contents without any operations
	return result;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			//main decryption function
			CPatch::RedirectJump(0x11C8110, &decryptionFunction);
			//always true because otherwise game will not work with decrypted files
			CPatch::RedirectJump(0x11C7DF0, &is_rnr_encrypt_format);

			//DisplayConsole();

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			//uncomment to see parser error output in console window
			//system("pause");
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

	return TRUE;
}