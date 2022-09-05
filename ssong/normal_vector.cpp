#include <iostream>
#include <cstring>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

vector<float> v1;
vector<tuple<float,float,float>> v2; // Vertex X,Y,Z

vector<int> v3; // Three Vertex of Face

ifstream input("input.obj");
ofstream output("output.txt");

void Calculate_normal_vector(float first, int second, int third)
{
    // get<0>(v2[first-1])  get<1>(v2[first-1])  get<2>(v2[first-1])   // First vertex (X Y Z) 
    // get<0>(v2[second-1]) get<1>(v2[second-1])  get<2>(v2[second-1]) // Second vertex (X Y Z)
    // get<0>(v2[third-1])  get<1>(v2[third-1])  get<2>(v2[third-1])   // Third vertex의 (X Y Z)

    float v1_x, v1_y, v1_z;
    float v2_x, v2_y, v2_z;
    float v3_x, v3_y, v3_z;
    
    //First Vector(v1) : Second - First
    v1_x = get<0>(v2[second-1]) - get<0>(v2[first-1]);
    v1_y = get<1>(v2[second-1]) - get<1>(v2[first-1]);
    v1_z = get<2>(v2[second-1]) - get<2>(v2[first-1]);

    //Second Vector(v2) : Third - Second
    v2_x = get<0>(v2[third-1]) - get<0>(v2[second-1]);
    v2_y = get<1>(v2[third-1]) - get<1>(v2[second-1]);
    v2_z = get<2>(v2[third-1]) - get<2>(v2[second-1]);

    float normal_vector_x = v1_y * v2_z - v1_z*v2_y;
    float normal_vector_y = v1_z * v2_x - v1_x*v2_z;
    float normal_vector_z = v1_x * v2_y - v1_y*v2_x;

    // Normalize
    float len;
    len = sqrt(normal_vector_x * normal_vector_x + normal_vector_y * normal_vector_y + normal_vector_z * normal_vector_z);
    output << normal_vector_x / len << " " <<normal_vector_y / len << " " << normal_vector_z / len << endl; 
}

int main()
{
    output << "---------------Normalized Normal Vector Calculate Result ---------------" << endl;

    char buf[100];
    while(!input.fail())
    {
        if(!input.getline(buf, 100)) break;
        char *ptr = strtok(buf, " ");

        // Vertex
        if(strcmp(ptr,"v") == 0)
        {
            //Save Vertex
            for(int i =0; i< 3; i++)
            {
                ptr = strtok(NULL, " ");
                float point = atof(ptr); // string to float
                v1.push_back(point);
            }
            v2.push_back(make_tuple(v1[0],v1[1],v1[2])); // make tuple(X, Y ,Z)
            v1.clear();
        }
        // Face
        else if(strcmp(ptr,"f") == 0)
        {
            char buf2[50];
            for(int i = 0; i < 3; i++)
            {
                ptr = strtok(NULL, " ");
                strcpy(buf2, ptr);

                int cnt = 0;
                string points;
                
                while(1)
                {
                    if(buf2[cnt] == '/') break; // extract only V  (V/F)   
                    points += buf2[cnt];
                    cnt++;
                }
                
                int f_point = stoi(points); // extract point
                v3.push_back(f_point); // save order of V
                memset(buf2, 0, sizeof(buf2));
            }

            // Calculate Normal Vector
            Calculate_normal_vector(v3[0], v3[1], v3[2]);
            v3.clear();
        }
        memset(buf, 0, sizeof(buf));
    }
    input.close();
    output.close();
    return 0;
}