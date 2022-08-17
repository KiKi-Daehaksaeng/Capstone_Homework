#define _USE_MATH_DEFINES

#include <iostream>
//#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <tuple>

using namespace std;



int main(void){
    ifstream fin;
    ofstream fout;
    string buf, token;
    stringstream ss;
    vector<tuple<int, int, int> > v;
    vector<tuple<int, int, int> > f;
    vector<tuple<double, double, double> > normal_vector;
    

    fin.open("basketball_player_fr0001_qp12_qt12.obj", ifstream::in);
    if(!fin.is_open()){
        cout << "Open input file error" << endl;
        return 0;
    }

    fout.open("normal_vector.txt", ios::out | ios::trunc);

    while(getline(fin, buf)){
        for(auto &i : buf)  if(i == '/')    i = ' ';
        ss.str(buf);
        ss >> token;
        if(token.compare("v") == 0){
            v.push_back(make_tuple(0, 0, 0));
            ss >> get<0>(v.back()) >> get<1>(v.back()) >> get<2>(v.back());
            //fout << get<0>(v.back()) << " " << get<1>(v.back()) << " " << get<2>(v.back()) << endl;
        }
        else if(token.compare("f") == 0){
            int temp;
            f.push_back(make_tuple(0, 0, 0));
            ss >> get<0>(f.back()) >> temp >> get<1>(f.back()) >> temp >> get<2>(f.back());
        }
        ss.clear();
    }

    for(auto i : f){
        tuple<int, int, int> a = v[get<0>(i)], b = v[get<1>(i)], c = v[get<2>(i)];      //vertex a, b, c
        int det[2][3] = {{get<0>(b) - get<0>(a), get<1>(b) - get<1>(a), get<2>(b) - get<2>(a)}, {get<0>(c) - get<0>(a), get<1>(c) - get<1>(a), get<2>(c) - get<2>(a)}};  //v[0] = b-a vector, v[1] = c-a vector
        int x = det[0][1] * det[1][2] - det[0][2] * det[1][1], y = det[0][2] * det[1][0] - det[0][0] * det[1][2], z = det[0][0] * det[1][1] - det[0][1] * det[1][0];    //v[0] X v[1]
             
        float d = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
        //unit normal vector
        fout << x / d << " " << y / d << " " << z / d << " " << endl;
    }

    fin.close();
    fout.close();

    return 0;
}