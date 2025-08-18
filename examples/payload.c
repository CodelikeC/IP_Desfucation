#include <Windows.h>
#include <stdio.h>

VOID MsgBoxPayload()
{
    MessageBoxA(NULL, "Xaloman", "WOW !", MB_OK|MB_ICONINFORMATION); 
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch(dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            MsgBoxPayload();
            break; 
        }; 

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("[!] Missing Argument; DLL payloaf To Run\n"); 
        return -1; 
    }

    printf("[i] Injecting \"%s\" To the Local Process Of Pid : %d \n", argv[1], GetCurrentProcessId());
    printf("[+] Loading Dll.."); 

    if (LoadLibraryA(argv[1]) == NULL)
    {
        printf("[!] LoadLibraryA failed with Error: %d\n", GetLastError());
        return - 1; 
    }

    printf("[+] Done ! \n"); 
    printf("[#] Press <Enter> To Quit...."); 
    getchar(); 
    
    return 0;
}