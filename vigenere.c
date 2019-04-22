#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (!argv[1])
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        char a = argv[1][i];
        if (argc > 2 || isdigit(a))
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }
    
    string s = argv[1];
    int cipher[strlen(s)];
    int b = 0;
    for (int i = 0; i < strlen(s); i++)
    {           
        if (isupper(s[i]))
        {
            b = (int) s[i];
            cipher[i] = b;
            cipher[i] -= 65;
        }
        else
        {
            b = (int) s[i];
            cipher[i] = b;
            cipher[i] -= 97;
        }                   
    }

    string t = get_string("plaintext: ");
    int message[strlen(t)];
    int c = 0;
    for (int i = 0; i < strlen(t); i++)
    {        
        if (isupper(t[i]))
        {
            c = (int) t[i];
            message[i] = c;
            message[i] -= 65;
            
        }
        else if (isblank(t[i]))
        {
            c = (int) t[i];
            message[i] = c;
            message[i] -= 32;
        }
        else
        {
            c = (int) t[i];
            message[i] = c;
            message[i] -= 97;
        }                   
    }

    int key[strlen(t)];
    if (strlen(s) < strlen(t))
    {
        for (int i = 0, n = strlen(t), j = 0; i < n; i ++, j++)
        {
            if (j == strlen(s))
            {
                j = 0;
            }
            key[i] = cipher[j];
        }
    }
    printf("ciphertext: ");

    int cryptograph[strlen(t)];
    char output[strlen(t)];    
    if (strlen(s) < strlen(t))
    {
        for (int i = 0, n = strlen(t); i < n; i++)
        {
            if (isupper(t[i]))
            {
                cryptograph[i] = ((message[i] + key[i]) % 26) + 65;
                char d = (char) cryptograph[i];
                output[i] = d;
            }
            else if (isblank(t[i]))
            {
                cryptograph[i] = ((message[i] + key[i]) % 26) + 32;
                char d = (char) cryptograph[i];
                output[i] = d;
            }
            else
            {
                cryptograph[i] = ((message[i] + key[i]) % 26) + 97;
                char d = (char) cryptograph[i];
                output[i] = d;
            }  
            printf("%c", output[i]);
        }
        printf("\n");
    }
    else
    {
        for (int i = 0, n = strlen(t); i < n; i++)
        {
            if (isupper(t[i]))
            {
                cryptograph[i] = ((message[i] + cipher[i]) % 26) + 65;
                char d = (char) cryptograph[i];
                output[i] = d;
            }
            else if (isblank(t[i]))
            {
                cryptograph[i] = ((message[i] + cipher[i]) % 26) + 32;
                char d = (char) cryptograph[i];
                output[i] = d;
            }
            else
            {
                cryptograph[i] = ((message[i] + cipher[i]) % 26) + 97;
                char d = (char) cryptograph[i];
                output[i] = d;
            }  
            printf("%c", output[i]);
        }
        printf("\n");
    }
}
