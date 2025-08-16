#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    char *buf, *buf2, *buf3; 

    buf = (char *) malloc(1024); 
    buf2 = (char *) malloc(1024); 
    buf3 = (char*)malloc(1024); 

    free(buf2); 

    strcpy(buf, argv[1]); 
    buf2 = (char *) malloc(1024); 
    // This was a free() in the previous example..
    printf("Done"); 
    // we will use this to take control in our exploit.. 

    return 0;
}