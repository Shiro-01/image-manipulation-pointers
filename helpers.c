#include "helpers.h"
#include <math.h>
#include <stdio.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; (i < height); i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE averge = (BYTE)(round(((double)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)) / 3.0));
            image[i][j].rgbtBlue = averge;
            image[i][j].rgbtGreen = averge;
            image[i][j].rgbtRed = averge;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; (i < height); i++)
    {
        for (int j = width, k = 0; j != k; j--, k++)
        {
            printf("%d\n", width);
            // making ssure that no swap happem if the width is even and stop it
            if (j - k == 1)
            {
                break;
            }
            // swapping the last with the first  varible
            RGBTRIPLE temp = image[i][j - 1];
            image[i][j - 1] = image[i][k];
            image[i][k] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            float totalb = 0;
            float totalg = 0;
            float totalr = 0;
            int pixcount = 0;
            int surrow[] = {row - 1, row, row + 1};
            int surrcol[] = {col - 1, col, col + 1};

            /* getrting the surrounding rows and colum that does not extend the edge */
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    // making sure all the pixel is within the frame
                    if ((surrow[r] >= 0) && (surrow[r] < height) && (surrcol[c] >= 0) && (surrcol[c] < width))
                    {
                        RGBTRIPLE pixel = image[surrow[r]][surrcol[c]];
                        pixcount++;
                        totalb += pixel.rgbtBlue;
                        totalg += pixel.rgbtGreen;
                        totalr += pixel.rgbtRed;
                    }
                }
            }

            // copying to temp file
            temp[row][col].rgbtBlue = (BYTE)round(totalb / pixcount);
            temp[row][col].rgbtGreen = (BYTE)round(totalg / pixcount);
            temp[row][col].rgbtRed = (BYTE)round(totalr / pixcount);
        }
    }

    // copying the new image to its original source.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = temp[row][col];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //declearing the sobal arrays GX & GY.
    int Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    RGBTRIPLE temp[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // intializing the factors
            double Gyb = 0;
            double Gxb = 0;

            double Gyg = 0;
            double Gxg = 0;

            double Gyr = 0;
            double Gxr = 0;

            // getting the surrounf4ding pixels
            int surrow[] = {row - 1, row, row + 1};
            int surrcol[] = {col - 1, col, col + 1};
            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    // making sure all the pixel is within the frame
                    if ((surrow[r] >= 0) && (surrow[r] < height) && (surrcol[c] >= 0) && (surrcol[c] < width))
                    {
                        // calculating the index of GX and Gy for both color
                        RGBTRIPLE pixel = image[surrow[r]][surrcol[c]];
                        Gxb += Gx[r][c] * (double) pixel.rgbtBlue;
                        Gxg += Gx[r][c] * (double) pixel.rgbtGreen;
                        Gxr += Gx[r][c] * (double) pixel.rgbtRed;

                        Gyb += Gy[r][c] * (double) pixel.rgbtBlue;
                        Gyg += Gy[r][c] * (double) pixel.rgbtGreen;
                        Gyr += Gy[r][c] * (double) pixel.rgbtRed;
                    }
                }
            }
            //gettong the sqaur root.
            double blue = round(sqrt(Gxb * Gxb + Gyb * Gyb));
            double green = round(sqrt(Gxg * Gxg + Gyg * Gyg));
            double red = round(sqrt(Gxr * Gxr + Gyr * Gyr));

            // copying to temp
            temp[row][col].rgbtBlue = blue > 255 ? 255 : blue;
            temp[row][col].rgbtGreen = green > 255 ? 255 : green;
            temp[row][col].rgbtRed = red > 255 ? 255 : red;
        }
    }


    // copying the new image to its original source.
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = temp[row][col];
        }
    }
    return;
}