// Project UID 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include <iostream>
#include <string>
#include <cmath>
#include "zoo.h"
#include <math.h>
#include <algorithm>
#include <getopt.h>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_set>
#include <cstring>
#include <limits>
#include <numbers>
#include <iomanip>




using namespace std;

vector<Coordinate*> coords;
int num_vertices;

void fill(){
    cin >> num_vertices;
    for(int i=0; i<num_vertices; i++){
        long tempx;
        long tempy;
        cin >> tempx;
        cin >> tempy;
        Coordinate* temp = new Coordinate(i, tempx, tempy);
        coords.push_back(temp);
    }
}

double calculatedist(long index1, long index2){
    bool safe1 = false;
    bool safe2 = false;
    if(coords[index1]->x > 0 || coords[index1]->y > 0){
        safe1 = true;
    }
    if(coords[index2]->x > 0 || coords[index2]->y > 0){
        safe2 = true;
    }
    if(coords[index1]->x == 0 || coords[index1]->y == 0 || coords[index2]->x == 0 || coords[index2]->y == 0){
        safe1 = true;
        safe2 = true;
    }
    if((safe1 && !safe2) || (!safe1 && safe2)){
        return numeric_limits<double>::infinity();
    }
    long x2 = (coords[index1]->x-coords[index2]->x) * (coords[index1]->x-coords[index2]->x);
    long y2 = (coords[index1]->y-coords[index2]->y) * (coords[index1]->y-coords[index2]->y);
    long fin = x2 + y2;
    return sqrt(fin); //do the sqrt later when it is returned
}

void mst(){
    coords[0]->distance = 0;
    double running_total = 0;
    for (size_t i = 0; i < coords.size(); ++i) {
        long minDistIndex = -1;
        double minDist = numeric_limits<double>::infinity();
        for (size_t j = 0; j < coords.size(); ++j) {
            if (!coords[j]->visited && coords[j]->distance < minDist) {
                minDist = coords[j]->distance;
                minDistIndex = j;
            }
        }
        if(minDistIndex != -1){
            coords[minDistIndex]->visited = true; //we have a -1 visited
            running_total += coords[minDistIndex]->distance;
            for (size_t j = 0; j < coords.size(); ++j) {
                if (!coords[j]->visited) {
                    double dist = calculatedist(minDistIndex, j);
                    if (dist < coords[j]->distance && dist != -1) {
                        coords[j]->distance = dist;
                        coords[j]->prevcoord = minDistIndex;
                    }
                }
            }
        }
    }
    cout << running_total << "\n";
    for (int i=0; i<(int)coords.size(); i++) {
        if (coords[i]->prevcoord != -1) { // If the vertex is connected to the MST
            if(coords[i]->index < coords[i]->prevcoord) cout << coords[i]->index << " " << coords[i]->prevcoord << endl;
            else cout << coords[i]->prevcoord << " " << coords[i]->index << endl;
        }
    }
}

void fasttsp(){

}

void opttsp(){

}

int main(int argc, char * argv[]) {
    cout << std::setprecision(2);
    cout << std::fixed;
	struct option longOpts[] = {
		{"help",no_argument,NULL,'h'},
		{"mode",required_argument,NULL,'m'},
		{NULL,0,NULL,0}
	};
    
	opterr = 1;
    bool MST = false;
    bool fast = false;
	int opt = 0, index = 0;
	bool optimal = false;
	while ((opt = getopt_long(argc,argv,"hm:",longOpts,&index)) != -1) {
		switch (opt) {
			case 'h':
				cout << "help flag\n";
				return 0;
			case 'm':
                if (strcmp(optarg, "MST") == 0) {
                    MST = true;
                }
                if(strcmp(optarg, "FASTTSP") == 0) {
                    fast = true;
                }
                if(strcmp(optarg, "OPTTSP") == 0) {
                    optimal = true;
                }
				break;
			case '?':
			default:
                cout << "default error\n";
				exit(1);
		}
	}
	fill();
    
    if(MST){
        mst();
    }
    else if(fast){
        fasttsp();
    }
    else if(optimal){
        opttsp();
    }

	return 0;
}
