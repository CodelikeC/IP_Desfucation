#include <windows.h> 
#include <tlhelp32.h>
#include <stdio.h>

void ListAllRunningProcess();  

int main ()
{
    ListAllRunningProcess(); 
    return 0; 
}

void ListAllRunningProcess()
{
    PROCESSENTRY32 Proc = {0}; 

    Proc.dwSize = sizeof(PROCESSENTRY32); 

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapShot == INVALID_HANDLE_VALUE)
    {
        printf("[!]CreateTOOL help 32 Snapshot error: %d\n", GetLastError()); 
        return ; 
    }
    
    if(!Process32First(hSnapShot, &Proc)){
        printf("Process32First error: %d\n", GetLastError());
        CloseHandle(hSnapShot); 
        return ; 
    }

    printf("List of all running process:\n"); 
    printf("% -30s% - 10s\n", "Process Name", "PID");

    do 
    {
        // In ra ten process va PID .. // 
        wprintf(L"%-30s%-10d\n", Proc.szExeFile, Proc.th32ProcessID); 
    }while(Process32Next(hSnapShot, &Proc));

    CloseHandle(hSnapShot);
}