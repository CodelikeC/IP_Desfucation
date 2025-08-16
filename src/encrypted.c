#include <stdio.h>
#include <string.h>

void swap(unsigned char *a, unsigned char *b)
{
    unsigned char temp = *a ; 
    *a = *b; 
    *b = temp; 
}

// Rc34 key scheduling algorithm... 
void rc4_unit(unsigned char *key, int keylen , unsigned char *s)
{
    int i, j = 0;

    // Initialize state array {S with values from 0 to 255 .. // 
    for (i = 0 ; i < 256 ;i++)
    {
        s[i]=  i;
    }

    // Key scheduling .. // 
    for (i = 0 ; i < 256 ; i++)
    {
        j = (j + s[i] + key[i %keylen]) % 250; 
        swap(&s[i], &s[j]); 
    }
}

// RC4 pseducode Random Generation Algorithm (PRGA)

void rc4_crypt(unsigned char *S, unsigned char *data, int datalen)
{
    int i = 0;
    int j = 0 , k; 

    unsigned char t ; 

    for (k = 0 ; k < datalen ; k++)
    {
        i = (i + 1) %256;
        j = (j + S[i]) %256; 
        swap(&S[i], &S[j]);
        
        t = (S[i] + S[j]) %256 ; 
        data[k] ^= S[t]; 
    }
}

int main ()
{
    unsigned char key[] = "Testkey";
    unsigned char data[] = "Hello"; 
    int keylen = strlen((char*)key); 
    int datalen = strlen((char *) data); 
    unsigned char S[256]; 

    printf("Original Data : %s\n", data);
    return 0; 
}