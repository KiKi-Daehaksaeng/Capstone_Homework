#define _CRT_SECURE_NO_WARNINGS

#include <cmath>
#include <iostream>
#include <math.h>
#include <Windows.h>

#define ROW 256
#define COL 256

#define PI 3.1415

unsigned char STRONG = 255;
unsigned char WEAK = 25;

using namespace std;

unsigned char in_image[ROW][COL] = { 0 };
unsigned char gaussian_out_image[ROW][COL] = { 0 };
unsigned char sobel_out_image[ROW][COL] = { 0 };
unsigned char Non_Max_Suppression_out_image[ROW][COL] = { 0 };
unsigned char Threshold_out_image[ROW][COL] = { 0 };
unsigned char Hysteresis_out_image[ROW][COL] = { 0 };

double Kernel[5][5] = { 0, };
double gradient[ROW][COL] = { 0 };
double angle[ROW][COL] = { 0 };


void gaussian_kernal(double Kernel[][5])
{
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
    double sum = 0.0;

    // Make 5x5 kernel
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            r = sqrt(x * x + y * y);
            Kernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (PI * s);
            sum += Kernel[x + 2][y + 2];
        }
    }
    // Normalize the Kernel
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            Kernel[i][j] /= sum;
        }
    }

    return;
}

void gaussian_filter(double Kernel[][5], unsigned char in_image[][COL], unsigned char out_image[][COL])
{
    double newvalue = 0.0;
    double sum = 0.0;
    int kernel_row, kernel_col;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            sum += Kernel[i][j];
        }
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            newvalue = 0;
            for (kernel_row = 0; kernel_row < 5; kernel_row++)
            {
                for (kernel_col = 0; kernel_col < 5; kernel_col++)
                {
                    if (i + kernel_row - 2 < 0 || j + kernel_col - 2 < 0 || i + kernel_row + 2 > ROW || j + kernel_col + 2 > COL) continue;
                    newvalue += Kernel[kernel_row][kernel_col] * in_image[i + kernel_row - 2][j + kernel_col - 2];
                }
            }
            newvalue /= sum;
            if (i < 2 || j < 2 || i > ROW - 2 || j > COL - 2) out_image[i][j] = in_image[i][j];
            else out_image[i][j] = (BYTE)newvalue;
        }
    }
}

void Sobel_Filter(unsigned char in_image[][COL], unsigned char out_image[][COL], double gradient[][COL], double angle[][COL])
{
    int MaskSobelX[3][3] = { {-1, 0, 1 },
                            {-2, 0, 2},
                            {-1, 0, 1 } };
    int MaskSobelY[3][3] = { {1, 2, 1},
                            {0, 0, 0},
                            {-1, -2 ,-1} };

    double sobel_x, sobel_y;
    double max = 0.0;
    double newvalue;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
         
            // Matrix Calculate with Mask
            sobel_x = 0;
            for (int sobel_row = 0; sobel_row < 3; sobel_row++)
            {
                for (int sobel_col = 0; sobel_col < 3; sobel_col++)
                {
                    if (i + sobel_row - 1 < 0 || j + sobel_col - 1 < 0 || i + sobel_row + 1 > ROW || j + sobel_col + 1 > COL) continue;
                    sobel_x += MaskSobelX[sobel_row][sobel_col] * in_image[i + sobel_row - 1][j + sobel_col - 1];
                }
            }

            sobel_y = 0;
            for (int sobel_row = 0; sobel_row < 3; sobel_row++)
            {
                for (int sobel_col = 0; sobel_col < 3; sobel_col++)
                {
                    if (i + sobel_row - 1 < 0 || j + sobel_col - 1 < 0 || i + sobel_row + 1 > ROW || j + sobel_col + 1 > COL) continue;
                    sobel_y += MaskSobelY[sobel_row][sobel_col] * in_image[i + sobel_row - 1][j + sobel_col - 1];
                }
            }

            // calculate gradient
            gradient[i][j] = sqrt((sobel_x * sobel_x) + (sobel_y * sobel_y));

            if (max < gradient[i][j])  max = gradient[i][j];
            newvalue = atan(sobel_y / sobel_x) * 180 / PI;

            if (newvalue < 0) angle[i][j] = newvalue + 180;
            else angle[i][j] = newvalue;
        }
    }

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            gradient[i][j] = round(gradient[i][j] / max * COL);
            out_image[i][j] = (unsigned char)gradient[i][j];
        }
    }
    return;
}

void Non_Max_Suppression(unsigned char in_image[][COL], double angle[][COL], unsigned char out_image[][COL])
{
    double q = 255.0;
    double r = 255.0;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (j - 1 < 0 || j + 1 > COL || i - 1 < 0 || i + 1 > COL) out_image[i][j] = in_image[i][j];

            //angle 0
            else if ((0 <= angle[i][j] && angle[i][j] < 22.5) || (157.5 <= angle[i][j] && angle[i][j] < 180))
            {
                q = in_image[i][j + 1];
                r = in_image[i][j - 1];
            }

            //angle 45
            else if (22.5 <= angle[i][j] && angle[i][j] < 67.5)
            {
                q = in_image[i + 1][j - 1];
                r = in_image[i - 1][j + 1];
            }

            //angle 90
            else if (67.5 <= angle[i][j] && angle[i][j] < 112.5)
            {
                q = in_image[i + 1][j];
                r = in_image[i - 1][j];
            }
            //angle 135
            else if (112.5 <= angle[i][j] && angle[i][j] < 157.5) {
                q = in_image[i - 1][j - 1];
                r = in_image[i + 1][j + 1];
            }
             
            if (in_image[i][j] >= q && in_image[i][j] >= r) out_image[i][j] = in_image[i][j];
            else out_image[i][j] = 0;
        }
    }
}

void Threshold(unsigned char in_image[][COL], unsigned char out_image[][COL])
{
    double lowThresholdRatio = 0.05;
    double highThresholdRatio = 0.09;
    
    double img_max = 0;

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (in_image[i][j] > img_max) img_max = in_image[i][j];
        }
    }
    
    double highThreshold = img_max * highThresholdRatio;
    double lowThreshold = highThreshold * lowThresholdRatio;

    // compare with Ratio to make Strong or weak
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (in_image[i][j] >= highThreshold) out_image[i][j] = STRONG;
            else if (in_image[i][j] <= lowThreshold) out_image[i][j] = WEAK;
        }
    }
}

void Hysteresis(unsigned char in_image[][COL], unsigned char out_image[][COL])
{
    unsigned char Check[ROW][COL] = { 0 };

    for (int i = 1; i < ROW-1; i++)
    {
        for (int j = 1; j < COL-1; j++)
        {
            //Check Strong or Weak
            if (in_image[i][j] == WEAK)
            {
                if ((in_image[i - 1][j - 1] == STRONG) || (in_image[i - 1][j] == STRONG) || (in_image[i - 1][j + 1] == STRONG) || (in_image[i][j - 1] == STRONG) || (in_image[i][j + 1] == STRONG) || (in_image[i + 1][j - 1] == STRONG) || (in_image[i + 1][j] == STRONG) || (in_image[i + 1][j + 1] == STRONG))
                    Check[i][j] = STRONG;
                else Check[i][j] = 0;
            }
        }
    }

    // make result 
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (Check[i][j] == STRONG) out_image[i][j] = 255;
            else out_image[i][j] = 0;
        }
    }
}

int main()
{
    FILE* infile = fopen("gLenna256_256.raw", "rb+");
    if (infile == NULL)
    {
        cout << "File open Error";
        return 0;
    }
    fread(in_image, sizeof(char), ROW * COL, infile);


    // Gaussian Filter
    gaussian_kernal(Kernel);
    gaussian_filter(Kernel, in_image, gaussian_out_image);
    FILE* gaussian_outfile = fopen("gaussian_gLenna256_256.raw", "wb+");
    fwrite(gaussian_out_image, sizeof(char), ROW * COL, gaussian_outfile);


    // Sobel Filter
    Sobel_Filter(gaussian_out_image, sobel_out_image, gradient, angle);
    FILE* sobel_outfile = fopen("sobel_gLenna256_256.raw", "wb+");
    fwrite(sobel_out_image, sizeof(char), ROW * COL, sobel_outfile);


    // Non_Max_Suppression
    Non_Max_Suppression(sobel_out_image, angle, Non_Max_Suppression_out_image);
    FILE* Non_Max_Suppression_outfile = fopen("Non_Max_Suppression_gLenna256_256.raw", "wb+");
    fwrite(Non_Max_Suppression_out_image, sizeof(char), ROW * COL, Non_Max_Suppression_outfile);

    // Threshold
    Threshold(Non_Max_Suppression_out_image, Threshold_out_image);
    FILE* Threshold_outfile = fopen("Threshold_gLenna256_256.raw", "wb+");
    fwrite(Threshold_out_image, sizeof(char), ROW * COL, Threshold_outfile);

    // Hysteresis
    Hysteresis(Threshold_out_image, Hysteresis_out_image);
    FILE* Hysteresis_outfile = fopen("Canny_Edge_Detection_gLenna256_256.raw", "wb+");
    fwrite(Hysteresis_out_image, sizeof(char), ROW * COL, Hysteresis_outfile);


    // Close File pointer
    fclose(infile);
    fclose(gaussian_outfile);
    fclose(sobel_outfile);
    fclose(Non_Max_Suppression_outfile);
    fclose(Threshold_outfile);
    fclose(Hysteresis_outfile);

    return 0;
}
