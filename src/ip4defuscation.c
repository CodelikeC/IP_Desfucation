#include <Windows.h>
#include <stdio.h>

typedef NTSTATUS(NTAPI *fnRt)
(
    PCSTR S, 
    BOOLEAN Strict, 
    PCSTR *Terminator,
    PVOID Addr
);

BOOL Ipv4Deobfuscation(IN CHAR * Ipv4Array[], IN SIZE_T NmbrOfElements,
OUT PBYTE *ppDAddress, OUT SIZE_T *pSize)
{
    PBYTE pBuffer = NULL, TmpBuffer = NULL;
    SIZE_T sbuffsize = 0; 
    PCSTR Terminator = NULL; 
    NTSTATUS STATUS = 0; 

    // Getting RTl pv 4 string from ntdll.dll .. // 
    fnRt prtlipv4 = 
    (fnRt)GetProcAddress(GetModuleHandle(TEXT("ntdll")), "RTLipv4StringToAddressA");
    
    if (prtlipv4 == NULL)
    {
        printf("GEt Proc Address Failed with Error: %d\n", GetLastError());
        
        return FALSE;
    }
    sbuffsize = NmbrOfElements * 5; 

    // Allocating memory which will hold the deobfuscation shellcode...// 
    pBuffer = (PBYTE) HeapAlloc(GetProcessHeap() , HEAP_ZERO_MEMORY, sbuffsize); 
    if (pBuffer == NULL)
    {
        printf("Heap Alloc Failed with Error: %d\n", GetLastError()); 
        return FALSE ;
    }

    TmpBuffer = pBuffer; 

    for (SIZE_T i = 0 ; i< NmbrOfElements  ; i++)
    {
        STATUS = prtlipv4(Ipv4Array[i], FALSE, &Terminator, TmpBuffer); 

        if (STATUS != FALSE && STATUS != 0x0)
        {
            printf(Ipv4Array[i], STATUS);
            HeapFree(GetProcessHeap(), 0, pBuffer); 
            return FALSE; 
        }
        TmpBuffer += 4; 
    }
    *ppDAddress= pBuffer; 
    *pSize = sbuffsize; 

    return TRUE; 
}

int main ()
{
    // Example IPv4 Array ...// 
    CHAR *Ipv4Array[] = {
        "252.72.131.228", "240.232.192.0", "0.0.65.81", "65.80.82.81"
    };
    
    SIZE_T NmbrofElements = sizeof(Ipv4Array) / sizeof(Ipv4Array[0]) ; 
    PBYTE pDAddress = NULL;

    SIZE_T pSize = 0 ; 

    if(Ipv4Deobfuscation(Ipv4Array, NmbrofElements, &pDAddress, &pSize))
    {
        printf("Deofuscation successful, Size : %llu bytes\n", pSize); 

        // print the deofuscated shellcode .. // 
        printf("Deofuscated ShellCode:\n"); 

        for (SIZE_T i = 0 ; i < pSize ; i++)
        {
            if (i %16 == 0 && i !=0)
            {
                printf("\n"); 
            }
            printf("0x%02x", pDAddress[i]); 
        }
        printf("\n"); 

        HeapFree(GetProcessHeap(), 0, pDAddress); 
    }
    else 
    {
        printf("Failed\n"); 
    }
    return 0; 
}