// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // Initial scrubbing for corect user input
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames and scalar factor
    char *infile = argv[2];
    char *outfile = argv[3];
    float scalar_Factor = atof(argv[1]);

    if (scalar_Factor <= 0 || scalar_Factor > 100)
    {
        fprintf(stderr, "Scalar factor must be any real number between 1 and 100.\n");
        return 2;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // Store source and calculate destination width and height
    int source_x = bi.biWidth;
    int source_y = abs(bi.biHeight);
    int dest_x = floor(source_x * scalar_Factor);
    int dest_y = floor(source_y * scalar_Factor);

    // edit height and width to desired values
    bi.biWidth *= scalar_Factor;
    bi.biHeight *= scalar_Factor;

    // determine source and destination padding
    int source_padding = (4 - (source_x * sizeof(RGBTRIPLE)) % 4) % 4;
    int dest_padding = (4 - (dest_x * sizeof(RGBTRIPLE)) % 4) % 4;

    // Calculate new header data
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * dest_x) + dest_padding) * abs(dest_y);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Generate nearest neighbor ratio
    float xnn = (float) source_x / (float) dest_x;
    float ynn = (float) source_y / (float) dest_y;

    // redundancy checking, if a scanline has already been read do not read.
    RGBTRIPLE index_y[source_x * sizeof(RGBTRIPLE)];
    int check = -1;

    // Iterate over the destination scanlines
    for (int i = 0, biHeight = abs(dest_y); i < biHeight; i++)
    {
        // Determines the nearest neighbor for y index.
        int scanline = i * ynn;

        // If redundancy check passes, read scanline.
        if (check != scanline)
        {
            fseek(inptr, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (((sizeof(RGBTRIPLE) * source_x) + source_padding) * scanline),
                  SEEK_SET);
            fread(index_y, sizeof(RGBTRIPLE), source_x, inptr);
            check = scanline;
        }

        // Iterate over the destination pixels
        for (int j = 0; j < dest_x; j++)
        {
            // Determine the nearest neighbor for x index
            int index_x = j * xnn;
            fwrite(&index_y[index_x], sizeof(RGBTRIPLE), 1, outptr);
        }

        // write destination padding
        for (int k = 0; k < dest_padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

