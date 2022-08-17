#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>


using namespace std;

void Cal_Matrix(double mat[][3], int alpha, int beta, int gamma){
    double a = alpha / (double)360 * M_PI;
    double b = beta / (double)360 * M_PI;
    double c = gamma / (double)360 * M_PI;

    double rx[3][3] = {
        {cos(a), -sin(a), 0},
        {sin(a), cos(a),  0},
        {0,      0,       1}
    };
    double ry[3][3] = {
        {cos(b),  0, sin(b)},
        {0,       1, 0     },
        {-sin(b), 0, cos(b)}
    };
    double rz[3][3] = {
        {1, 0,      0      },
        {0, cos(c), -sin(c)},
        {0, sin(c), cos(c) }
    };

    double temp[3][3];

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            temp[i][j] = (rx[i][0] * ry[0][j] + rx[i][1] * ry[1][j] + rx[i][2] * ry[2][j]); 
    
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            mat[i][j] = (temp[i][0] * rz[0][j] + temp[i][1] * rz[1][j] + temp[i][2] * rz[2][j]);


    return;
}

void Cal_Vertex(int conv[], int vertex[], double mat[][3]){
    for(int i = 0; i < 3; i++)
        conv[i] = round(mat[i][0] * vertex[0] + mat[i][1] * vertex[1] + mat[i][2] * vertex[2]);

    return;
}

int main(void){
    ifstream fin;
    ofstream fout;
    string buf, token;
    stringstream ss;

    int alpha, beta, gamma;
    int vertex[3] = {0, 0, 0};
    int conv[3] = {0, 0, 0};
    double mat[3][3];

    cout << "input angle(ex : 0 0 180) : ";
    cin >> alpha >> beta >> gamma;

    Cal_Matrix(mat, alpha, beta, gamma);
    // cout << "matrix" << endl;
    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //         cout << mat[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    fin.open("basketball_player_fr0001_qp12_qt12.obj", ifstream::in);
    if(!fin.is_open()){
        cout << "Open input file error" << endl;
        return 0;
    }

    fout.open("basketball_player_conversion.obj", ios::out | ios::trunc);

    while(getline(fin, buf)){
        ss.str(buf);
        ss >> token;
        if(token.compare("v") != 0){
            fout << buf << endl;
            continue;
        }

        for(int i = 0; i < 3; i++){
            ss >> token;
            vertex[i] = stoi(token);
        }

        Cal_Vertex(conv, vertex, mat);
        fout << "v " << conv[0] << ' ' << conv[1] << ' ' << conv[2] << endl;

        //cout << "(" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ")" << endl;
        ss.clear();
    }

    fin.close();
    fout.close();

    return 0;
}