#include <stdio.h>

int main(void)
{
    int height;
    printf("Height?: ");
    scanf("%d", &height);

    for (int i = 0; i <= height; i++)
    {
        for (int j = 0, n = (height - i); j < n; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        for (int j = 0, n = (height - i); j < n; j++)
        {
            printf(" ");
        }
        printf("\n");
    }
}