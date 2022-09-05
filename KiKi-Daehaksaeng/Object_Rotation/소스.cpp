#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <numbers>

using namespace std;

int x, y, z;
double degreeX, degreeY, degreeZ;

#define target "basketball_player_fr0001_qp12_qt12.obj"
#define output "result_basketball_player_fr0001_qp12_qt12.obj"

vector<vector<double>> matrix_mul_3by3(vector<vector<double>> a, vector<vector<double>>b) {
	vector<vector<double>> result(3, vector<double>(3));
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				result[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return result;
}

vector<vector<int>> matrix_mul_3by1(vector<vector<double>> a, vector<vector<int>>b) {
	vector<vector<int>> result(3,vector<int>(1));
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			result[i][0] += a[i][k] * b[k][0];
		}
	}
	return result;
}

vector<vector<int>> rotate(vector<int> line) {
	vector<vector<double>> rx(3, vector<double>(3, 0)), ry(3, vector<double>(3, 0)), rz(3, vector<double>(3, 0)),
		temp1(3, vector<double>(3, 0)), temp2(3, vector<double>(3, 0));
	vector<vector<int>> temp3(3, vector<int>(1)), result(3, vector<int>(1));
	for (int i = 0; i < 3; i++) {
		temp3[i][0] = line[i];
	}
	/*rx = {
		{1,0,0},
		{0,cos(degreeX),-sin(degreeX)},
		{0,sin(degreeX),cos(degreeX)}
	};
	ry = {
		{cos(degreeY),0,sin(degreeY)},
		{0,1,0},
		{-sin(degreeY),0,cos(degreeY)}
	};
	rz = {
		{cos(degreeZ),-sin(degreeZ),0},
		{sin(degreeZ),cos(degreeZ),0},
		{0,0,1}
	};

	temp1 = matrix_mul_3by3(rz, ry);
	temp2 = matrix_mul_3by3(temp1, rx);*/
	temp2 = {
		{cos(degreeX) * cos(degreeY), cos(degreeX) * sin(degreeY) * sin(degreeZ) - sin(degreeX) * cos(degreeZ), cos(degreeX) * sin(degreeY) * cos(degreeZ) + sin(degreeX) * sin(degreeY)},
		{sin(degreeX) * cos(degreeY), sin(degreeX) * sin(degreeY) * sin(degreeZ) + cos(degreeX) * cos(degreeZ),sin(degreeX) * sin(degreeY) * cos(degreeZ) - cos(degreeX) * sin(degreeZ)},
		{-sin(degreeY),cos(degreeY) * sin(degreeZ),cos(degreeY) * cos(degreeZ)}
	};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][0] = temp3[j][0] * temp2[i][j];
		}
	}
	return result;
}

void transform(string str) {
	vector<string> line;
	vector<int> tline;
	char* c = strtok((char*)str.c_str(), " ");

	while (c) {
		line.push_back((c));
		c = strtok(NULL, " ");
	}
	if (!line.size())return;
	string trans = line[0];
	if (trans != "v") {
		ofstream ofile;
		ofile.open(output, ios::app);
		ofile << str << endl;
		ofile.close();
		return;
	}
	
	for (int i = 1; i < line.size(); i++) {
		tline.push_back(stoi(line[i]));
	}

	vector<vector<int>> v = rotate(tline);

	trans += " ";
	for (int i = 0; i < v.size(); i++) {
		trans += to_string(v[i][0]);
		if (i < v.size() - 1) trans += ' ';
	}

	ofstream ofile;
	ofile.open(output, ios::app);
	ofile << trans << endl;
	ofile.close();
	return;
}

void run() {
	ifstream ifile;
	ifile.open(target);

	ofstream ofile;
	ofile.open(output);

	if (!ifile.is_open()) {
		cout << "Target file can't open" << endl;
		return;
	}
	string str;
	getline(ifile, str);
	ofile << str << endl;
	ofile.close();

	while (!ifile.eof()) {
		getline(ifile, str);
		transform(str);
	}
	ifile.close();
}

int main() {
	cout << "Enter degrees of x, y, z(ex 0 0 90) :";
	cin >> degreeX >> degreeY >> degreeZ;
	degreeX *= (M_PI / 180);
	degreeY *= (M_PI / 180);
	degreeZ *= (M_PI / 180);
	run();
}