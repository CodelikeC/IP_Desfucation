#include <stdio.h>
#include <string.h>

#include <stdlib.h>

// function to swap two bytes .. . 

void swap(unsigned char *a, unsigned char *b)
{
    unsigned char temp = *a ; 
    *a = *b; 
    *b = temp; 
}

// Rc 4 key scheduling algorithm (KSA)

void rc4_init(unsigned char*key, int keylen, unsigned char *S)
{
    int i, j; 
    i , j = 0 ;
    // intialize state array S with values from 0 to 255 .. 
    for (i = 0;  i < 256 ; i++)
    {
        S[i] = i;
    }

    for (i = 0 ; i < 256 ; i++)
    {
        j = (j + S[i] + key[i % keylen]) %256; 
        swap(&S[i], &S[j]); 
    }
}

// RC4 pseudo Random Generation Algorithrm (PRGA)

void rc4_crypt(unsigned char *S, unsigned char *data, int datalen)
{
    int i = 0 , j = 0, k; 
    unsigned char t ; 

    for (k = 0 ; k < datalen ; k++)
    {
        i = (i + 1) % 256; 
        j = (j + S[i]) % 256; 
        swap(&S[i], &S[j]); 
        t = (S[i] + S[j]) % 256; 

        data[k] ^= S[t]; 
    }
}

int main ()
{
    unsigned char key[] = "testkey"; 

    unsigned char encrypted_data[] = { 0xf4, 0xe7, 0xd2, 0x78, 0x2d};
    int keylen = strlen((char *) key); 
    int datalen = sizeof(encrypted_data); 
    unsigned char S[256]; 

    printf("Encrypted data"); 
    for (int i = 0 ; i < datalen; i++)
    {
        printf("&02x", encrypted_data[i]); 
    }
    printf("\n");

    rc4_init(key, keylen, S);

    rc4_crypt(S, encrypted_data, datalen); 
    printf("DECRYPTED DATA:"); 
    for (int i = 0 ; i < datalen ; i++)
    {
        printf("%c", encrypted_data[i]); 
    }
    printf(
        "\n"
    ); 
    return 0;
}