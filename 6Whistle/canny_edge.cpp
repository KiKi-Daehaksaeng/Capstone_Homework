#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

//image size
#define ROW 256
#define CAL 256

//Gaussian kernel size
#define G_SIZE 5

using namespace std;

const double PI = acos(-1);
const unsigned char week = 25, strong = 255;

void gaussian_kernel(vector<vector<double> > &kernel, double sigma = 1){ 
    double sum = 0;
    int range = G_SIZE / 2;

    double normal = 1 / (2.0 * PI * sigma * sigma);
    for(int i = 0; i < G_SIZE; i++)
        for(int j = 0; j < G_SIZE; j++){
            kernel[i][j] = normal * exp(-((pow(i - range, 2) + pow(j - range, 2)) / (2 * sigma * sigma)));
            sum += kernel[i][j];
        }

    for(int i = 0; i < G_SIZE; i++)   for(int j = 0; j < G_SIZE; j++)    kernel[i][j] /= sum;
    
    return;
}

double matrix_convolution(vector<vector<double> > &kernel, unsigned char image[][CAL], int i, int j){
    int x, y;
    int range = (int)kernel.size() / 2;
    double sum = 0;
    for(int k = -range; k < range + 1; k++){
        (i + k < 0) ? x = 0 : (i + k > 255 ? x = 255 : x = i + k);
        for(int l = -range; l < range + 1; l++){
            (j + l < 0) ? y = 0 : (j + l > 255 ? y = 255 : y = j + l);
            sum += kernel[k + range][l + range] * (int)(image[x][y]);
            // cout << "x, y = " << x << ',' << y << "\t image[x][y] = " << (int)image[x][y]
            // << "\t kernel[k + (SIZE / 2)][l + (SIZE / 2)] = " << kernel[k + ((int)kernel.size() / 2)][l + ((int)kernel.size() / 2)] << "\t sum = " << sum << endl;
        }
    }
    return sum;
}

void gaussian_filter(vector<vector<double> > &kernel, unsigned char image[][CAL], unsigned char gImage[][CAL]){
    for(int i = 0; i < ROW; i++)
        for(int j = 0; j < CAL; j++)
            gImage[i][j] = (unsigned char)round(matrix_convolution(kernel, image, i, j));
}

void sobel_filter(unsigned char gImage[][CAL], double gradient[][CAL], double theta[][CAL]){
    vector<vector<double> > f_x = {{-1, 0, 1},
                                   {-2, 0, 2},
                                   {-1, 0, 1}};
    vector<vector<double> > f_y = {{1, 2, 1},
                                   {0, 0, 0},
                                   {-1, -2, -1}};
    
    double i_x, i_y;
    double g_max = 0;
    double temp;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < CAL; j++){
            i_x = matrix_convolution(f_x, gImage, i, j);
            i_y = matrix_convolution(f_y, gImage, i, j);
            
            gradient[i][j] = sqrt((i_x * i_x) + (i_y * i_y));
            if(g_max < gradient[i][j])  g_max = gradient[i][j];
            temp = atan(i_y / i_x) * 180 / PI;
            temp < 0 ? theta[i][j] = temp + 180 : theta[i][j] = temp;
        }
    }

    for(int i = 0; i < ROW; i++)    for(int j = 0; j < CAL; j++)    gradient[i][j] = round(gradient[i][j] / g_max * 255);
}

void non_max_suppression(double sImage[][CAL], double theta[][CAL], double nonMaxImage[][CAL]){
    double q, r;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < CAL - 0; j++){
            if(i == 0 || i == ROW - 1 || j == 0 || j == CAL - 1){
                nonMaxImage[i][j] = sImage[i][j];
                continue;
            }

            //angle 0
            if((0 <= theta[i][j] && theta[i][j] < 22.5) || (157.5 <= theta[i][j] && theta[i][j] < 180)){
                q = sImage[i][j + 1];
                r = sImage[i][j - 1];
            }
            //angle 45
            else if(22.5 <= theta[i][j] && theta[i][j] < 67.5){
                q = sImage[i + 1][j - 1];
                r = sImage[i - 1][j + 1];
            }
            //angle 90
            else if(67.5 <= theta[i][j] && theta[i][j] < 112.5){
                q = sImage[i + 1][j];
                r = sImage[i - 1][j];
            }
            //angle 135
            else if(112.5 <= theta[i][j] && theta[i][j] < 157.5){
                q = sImage[i - 1][j - 1];
                r = sImage[i + 1][j + 1];
            }

            ((sImage[i][j] >= q) && (sImage[i][j] >= r)) ? nonMaxImage[i][j] = sImage[i][j] : nonMaxImage[i][j] = 0;  
        }
    }
}

void threshold(double nonMaxImage[][CAL], unsigned char thresholdImage[][CAL], double lowThresholdRatio = 0.05, double highThresholdRatio = 0.09){
    double maxImage = 0, highThreshold, lowThreshold;

    for(int i = 0; i < ROW; i++)    for(int j = 0; j < CAL; j++)    if(maxImage < nonMaxImage[i][j])    maxImage = nonMaxImage[i][j];
    highThreshold = maxImage * highThresholdRatio;
    lowThreshold = highThreshold * lowThresholdRatio;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < CAL; j++){
            if(nonMaxImage[i][j] >= highThreshold)  thresholdImage[i][j] = strong;
            else if(nonMaxImage[i][j] >= lowThreshold)  thresholdImage[i][j] = week;
        }
    }
}

unsigned char check_strong(unsigned char thresholdImage[][CAL], int i, int j){
    for(int k = -1; k < 2; k++)
        for(int l = -1; l < 2; l++)
            if(thresholdImage[i + k][j + l] == strong)  return strong;

    return 0;
}

void hysteresis(unsigned char thresholdImage[][CAL], unsigned char cannyEdgeImage[][CAL]){
    for(int i = 1; i < ROW - 1; i++){
        for(int j = 1; j < CAL - 1; j++){
            if(thresholdImage[i][j])    cannyEdgeImage[i][j] = check_strong(thresholdImage, i, j);
        }
    }


}


int main(void){
    ifstream fin;
    ofstream fgout, fsout, fnsout, fthout, fceout;

    unsigned char image[ROW][CAL], gImage[ROW][CAL] = { 0 }, thresholdImage[ROW][CAL] = { 0 }, cannyEdgeImage[ROW][CAL] = { 0 };
    double gradient[ROW][CAL] = { 0 }, theta[ROW][CAL] = { 0 }, nonMaxImage[ROW][CAL] = { 0 };

    vector<vector<double> > kernel(G_SIZE, vector<double>(G_SIZE)); 

    //Read image data(RAW format)
    fin.open("gLenna256_256.raw", ifstream::binary);
    if(!fin.is_open()){
        cout << "Open input file error" << endl;
        return 0;
    }
    for(auto &i : image)    for(auto &j : i)    fin.get((char&)j);
    //for(int i = 0; i < 10; i++) cout << (int)image[i][0] << ' ' << (int)image[i][1] << endl;

    //find gaussian kernel(filter)
    gaussian_kernel(kernel);
    //for(auto &i : kernel) for(auto &j : i) cout << j << " ";

    //filtering using gaussian filter
    gaussian_filter(kernel, image, gImage);

    fgout.open("gLenna256_256_gaussian.raw", ofstream::binary | ofstream::trunc);
    for(auto &i : gImage)   for(auto &j : i)    fgout.put(j);

    //filtering using sobel filter
    sobel_filter(gImage, gradient, theta);

    fsout.open("gLenna256_256_sobel.raw", ofstream::binary | ofstream::trunc);
    for(auto &i : gradient)   for(auto &j : i)  fsout.put((unsigned char)j);

    //Non-Max Suppression
    non_max_suppression(gradient, theta, nonMaxImage);

    fnsout.open("gLenna256_256_non_max.raw", ofstream::binary | ofstream::trunc);
    for(auto &i : nonMaxImage)   for(auto &j : i)   fnsout.put((unsigned char)j);

    //Threshold
    threshold(nonMaxImage, thresholdImage);

    fthout.open("gLenna256_256_threshold.raw", ofstream::binary | ofstream::trunc);
    for(auto &i : thresholdImage)   for(auto &j : i)    fthout.put((unsigned char)j);

    //Canny Edge Detection
    hysteresis(thresholdImage, cannyEdgeImage);

    fceout.open("gLenna256_256_canny_edge.raw", ofstream::binary | ofstream::trunc);
    for(auto &i : cannyEdgeImage)   for(auto &j : i)    fceout.put((unsigned char)j);

    fin.close();
    fgout.close();
    fsout.close();
    fnsout.close();
    fthout.close();
    fceout.close();

    return 0;
}