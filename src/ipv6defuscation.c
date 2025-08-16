#include <Windows.h>
#include <stdio.h>

typedef NTSTATUS (NTAPI * fnRtIpv6StringtoAddressA)
(
    PCSTR S,
    PCSTR *Terminator, 
    PVOID Addr
);

BOOL Ipv6Dfuscation(IN CHAR * Ipv6Array[], IN SIZE_T NmbrOfElements, 
OUT PBYTE *ppDAddress, OUT SIZE_T *pDSIZE)
{
    PBYTE pBuffer = NULL, TmpBuffer = NULL; 
    SIZE_T sBuffsize = 0; 
    PCSTR Terminator = NULL; 
    NTSTATUS STATUS = 0; 

    fnRtIpv6StringtoAddressA pRtLIpv6StringToAddressA;
}