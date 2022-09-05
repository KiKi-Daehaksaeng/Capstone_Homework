#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

#define target "basketball_player_fr0001_qp12_qt12.obj"
#define output "normal_vector.txt"

bool order = false;
int x, y, z;

vector<vector<int>> v;

vector<int> line1, line2, norm;

void normal() {
	norm = vector<int>(3);
	
	int ax = v[line1[1] - 1][0] - v[line1[0] - 1][0];
	int ay = v[line1[1] - 1][1] - v[line1[0] - 1][1];
	int az = v[line1[1] - 1][2] - v[line1[0] - 1][2];
	int bx=v[line1[2]-1][0] -v[line1[1]-1][0], by=v[line1[2]-1][1]-v[line1[1]-1][1], bz=v[line1[2]-1][2]-v[line1[1]-1][2];


	norm[0] = ay * bz-az*by;
	norm[1] = az*bx-ax*bz;
	norm[2] = ax*by-ay*bx;
}

void transform(string str) {
	vector<string> line;
	
	char* c = strtok((char*)str.c_str(), " ");

	while (c) {
		line.push_back((c));
		c = strtok(NULL, " ");
	}
	if (!line.size())return;
	string trans = line[0];

	if (trans == "v") {
		for (int i = 1; i < line.size(); i++) {
			line1.push_back(stoi(line[i]));
		}

		v.push_back(line1);
		line1.clear();
		return;
	}

	if (trans != "f") {
		return;
	}
	
		for (int i = 1; i < line.size(); i++) {
			c = strtok((char*)line[i].c_str(), "/");
			line1.push_back(stoi(line[i]));
		}

		normal();
		trans += " ";
		for (int i = 0; i < norm.size(); i++) {
			trans += to_string(norm[i]);
			if (i < norm.size() - 1) trans += ' ';
		}
		line1.clear();
		norm.clear();
		order = !order;

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
	ofile.close();

	if (!ifile.is_open()) {
		ofstream ofile;
		ofile.open(output);
		cout << "Target file can't open" << endl;
		ofile.close();
		return;
	}
	string str;

	while (!ifile.eof()) {
		getline(ifile, str);
		transform(str);
	}
	ifile.close();
}

int main() {
	run();
}