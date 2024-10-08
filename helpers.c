#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    double gray;
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            gray = (image[r][c].rgbtBlue + image[r][c].rgbtGreen + image[r][c].rgbtRed) / 3.0;
            image[r][c].rgbtBlue = round(gray);
            image[r][c].rgbtGreen = round(gray);
            image[r][c].rgbtRed = round(gray);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width / 2; c++)
        {
            temp = image[r][c];
            image[r][c] = image[r][width - c - 1];
            image[r][width - c - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temp source
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    int blueSum, greenSum, redSum;
    // blurring algo
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            int avgrgb[3] = {0, 0, 0};
            float n = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if ((((r - i) >= 0) && ((r - i) < height) &&
                         (((c - j) >= 0) && (c - j) < width)))
                    {
                        avgrgb[0] += temp[r - i][c - j].rgbtBlue;
                        avgrgb[1] += temp[r - i][c - j].rgbtGreen;
                        avgrgb[2] += temp[r - i][c - j].rgbtRed;
                        n++;
                    }
                }
            }
            image[r][c].rgbtBlue = round(avgrgb[0] / n);
            image[r][c].rgbtGreen = round(avgrgb[1] / n);
            image[r][c].rgbtRed = round(avgrgb[2] / n);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // temp source
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // edge detection algo
    float gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            float xrgb[3] = {0, 0, 0};
            float yrgb[3] = {0, 0, 0};
            int sobel[3] = {0, 0, 0};
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (((r + i) >= 0) && ((r + i) < height) && (((c + j) >= 0)) &&
                        ((c + j) < width))
                    {
                        xrgb[0] += gx[i + 1][j + 1] * temp[r + i][c + j].rgbtBlue;
                        xrgb[1] += gx[i + 1][j + 1] * temp[r + i][c + j].rgbtGreen;
                        xrgb[2] += gx[i + 1][j + 1] * temp[r + i][c + j].rgbtRed;
                        yrgb[0] += gy[i + 1][j + 1] * temp[r + i][c + j].rgbtBlue;
                        yrgb[1] += gy[i + 1][j + 1] * temp[r + i][c + j].rgbtGreen;
                        yrgb[2] += gy[i + 1][j + 1] * temp[r + i][c + j].rgbtRed;
                    }
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (sqrt((xrgb[i]) * (xrgb[i]) + (yrgb[i]) * (yrgb[i])) > 255)
                {
                    sobel[i] = 255;
                }
                else
                {
                    sobel[i] = round(sqrt((xrgb[i]) * (xrgb[i]) + (yrgb[i]) * (yrgb[i])));
                }
            }
            image[r][c].rgbtBlue = sobel[0];
            image[r][c].rgbtGreen = sobel[1];
            image[r][c].rgbtRed = sobel[2];
        }
    }
    return;
}
