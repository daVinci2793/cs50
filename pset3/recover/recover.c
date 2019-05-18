/* Recovers lost images previously stored on a memory card with
file system architecture "FAT" of the image file format standard "jfif" */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Defines the size of a "block" of memory
#define FAT 512

// Defines convenient data types for working with Windows based architechtures
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef uint16_t WORD;

DWORD determine_file_size(FILE *a);
BYTE eof_check(BYTE *array[], int i, int size, BYTE eof);

int main(int argc, char *argv[])
{
    // Initial scrubbing of user input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Open argv[1] if able
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // Determines the size of the infile and prepares program to read file system
    DWORD size = determine_file_size(file);
    DWORD num_FAT = size / FAT;

    // Returns the file pointer to the start of file
    fseek(file, 0, SEEK_SET);

    BYTE *block[num_FAT];
    FILE *outptr;
    bool check_open = false;
    char outfile[5];
    int debug = 0;
    // Iterates over index of FAT blocks
    for (int i = 0, n = 0; i < num_FAT; i++)
    {
        // Allocates memory for reading and writing
        block[i] = (BYTE *)calloc((FAT), sizeof(BYTE));
        if (&block[i] == NULL)
        {
            fprintf(stderr, "Failed to properly allocate memory.\n");
            return 3;
        }

        // Determines if the most recently read block is the likely start of a jpeg using jfif
        debug = fread(block[i], FAT, 1, file);
        if (debug == 1 && block[i][0] == 0xff && block[i][1] == 0xd8 && block[i][2] == 0xff && (block[i][3] > 0xdf && block[i][3] < 0xf0))
        {
            sprintf(outfile, "%03i.jpg", n);
            outptr = fopen(outfile, "w");
            if (!outptr)
            {
                fprintf(stderr, "Could not open %s.\n", outfile);
                return 4;
            }
            check_open = true;
            n++;
            fwrite(block[i], FAT, 1, outptr);
        }
        // Determines if the most recently read block contains the eof marker IAW jfif
        else if (check_open == true && (eof_check(block, i, FAT - 1, 0) == 1))
        {
            fwrite(block[i], FAT, 1, outptr);
            fclose(outptr);
            check_open = false;
        }
        // Determines how to behave if most recently read block is neither the start nor end of a recovered jpeg
        else
        {
            if (check_open == true)
            {
                fwrite(block[i], FAT, 1, outptr);
                continue;
            }
            if (check_open == false)
            {
                continue;
            }
        }
    }

    //closes *.raw file
    fclose(file);

    // Frees previously allocated memory
    for (int i = 0; i < num_FAT; i++)
    {
        free(block[i]);
    }
    return 0;
}

// Function to determine the size of the *.raw file
DWORD determine_file_size(FILE *a)
{
    fseek(a, 0, SEEK_END);
    DWORD size = ftell(a);
    return size;
}

// Recursive search function to determine of the jfif end of file marker exists in a given FAT block
BYTE eof_check(BYTE *array[], int i, int size, BYTE eof)
{
    int check = 0;
    if (size == -1)
    {
        return 0;
    }
    else if (array[i][size] == 0xd9)
    {
        check = 0xd9;
        return eof_check(array, i, size - 1, check);
    }
    else if (array[i][size] == 0xff && eof == 0xd9)
    {
        return 1;
    }
    else
    {
        check = 0;
        return eof_check(array, i, size - 1, check);
    }
}
