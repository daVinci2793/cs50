    
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
    string t = get_string("plaintext: ");   
    string s = argv[1];
    int n = strlen(s);
    int m = strlen(t);     
    int message[m];
    int c = 0;
    for (int i = 0; i < m; i++)
    {        
        c = (int) t[i];
        message[i] = c;           
    }
    
    int cipher[n];
    int b = 0;
    int key[m];
    for (int i = 0, j = 0; i < m; i ++, j++)    
    {           
        if (!isalpha(t[i]))
        {
            j = (j - 1);
        } 
        if (j == n || j < 0)
        {
            j = 0;
        }
        b = (int) s[j];
        cipher[j] = (b - 97);
        key[i] = tolower(cipher[j]);
    }
    printf("ciphertext: ");

    int cryptograph[m];
    char output[m];
    char d = 0;
    {
        for (int i = 0; i < m; i++)
        {
            if (isupper(t[i]))
            {
                cryptograph[i] = ((message[i] - 65 + key[i]) % 26) + 65;
                d = (char) cryptograph[i];
                output[i] = d;
            }
            else if (ispunct(t[i]))
            {
                cryptograph[i] = t[i];
                d = (char) cryptograph[i];
                output[i] = d;
            }
            else if (isblank(t[i]))
            {
                cryptograph[i] = 32;
                d = (char) cryptograph[i];
                output[i] = d;
            }
            else
            {
                cryptograph[i] = ((message[i] - 97 + key[i]) % 26) + 97;
                d = (char) cryptograph[i];
                output[i] = d;
            }  
            printf("%c", output[i]);
        }
        printf("\n");
       }
}
