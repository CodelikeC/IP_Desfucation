#include <Windows.h>

NTSTATUS SystemFunction032
(
    struct ustring *data, 
    const struct using * key
); 

typedef struct{
    DWORD Length;
    DWORD MaximumLength; 
    PVOID Buffer ; 
}USTRING;

// fnSystemFunction032 SystemFunction032 = (fnSystemFunction032);