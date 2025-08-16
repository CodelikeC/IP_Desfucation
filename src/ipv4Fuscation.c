#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

char *GenerateIpv4(int a, int b, int c,  int d)
{
    char *output = (char *) malloc(5); 

    if (output == NULL)
    {
        return NULL; 
    }

    sprintf_s(output, 16, "%d.%d.%d.%d", a,b,c,d);
    return output; 
};

BOOL GenerateIpv4Output(unsigned char *pShellcode, SIZE_T ShellcodeSize)
{
    if (pShellcode == NULL || ShellcodeSize == 0 || ShellcodeSize %4 != 0)
    {
        return FALSE;
    }

    printf("Char * IPV$ Array[%d] = {\n\t}", (int) (ShellcodeSize / 4));

    int counter = 0 ; 
    char *IP = NULL; 

    for (SIZE_T i  = 0 ; i < ShellcodeSize ; i+= 4)
    {
        counter ++; 
        IP = GenerateIpv4(pShellcode[i], pShellcode[i + 1], pShellcode[i + 2], pShellcode[i + 3]);
        
        if (IP == NULL)
        {
            return FALSE;
        }

        if (i == ShellcodeSize - 4)
        {
            printf("\"%s\"", IP);
        }

        else 
        {
            printf("\"%s\"", IP); 
        }

        free(IP); 

        if (counter % 8 == 0)
        {
            printf("\n\t") ; 
        }
    }

    printf("\n}; \n\n"); 
    return TRUE;
}

int main()
{
    printf("IPV4 fuscation"); 

    unsigned char shellcode[] =
    {
    0xFC, 0x48, 0x83, 0xE4, 0xF0, 0xE8, 0xC0, 0x00,
    0x00, 0x00, 0x41, 0x51, 0x41, 0x50, 0x52 , 0x51
    };
    
    SIZE_T shellcodeSize = sizeof(shellcode); 

    GenerateIpv4Output(shellcode, shellcodeSize); 
    return 0 ; 
}