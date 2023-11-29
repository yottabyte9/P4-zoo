// Project UID 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include <iostream>
#include <string>
#include <cmath>
#include "zoo.h"
#include <algorithm>
#include <getopt.h>
#include <cstring>
#include <iomanip>
#include <random>

using namespace std;

vector<Coordinate> coords;
int num_vertices;

void fill(){
    cin >> num_vertices;
    coords.resize(num_vertices);
    for(int i=0; i<num_vertices; i++){
        int tempx;
        int tempy;
        cin >> tempx;
        cin >> tempy;
        coords[i].x = tempx;
        coords[i].y = tempy;
        char type;
        if(tempx < 0 && tempy < 0){
            type = 'd';
        }
        else if((tempx <= 0 && tempy == 0) || (tempx == 0 && tempy <= 0)){
            type = 'w';
        }
        else{
            type = 's';
        }
        coords[i].type = type;
    }
}

double calculatedist(int index1, int index2){
    if((coords[index1].type == 's' && coords[index2].type == 's') || (coords[index1].type == 'w' || coords[index2].type == 'w') || (coords[index1].type == 'd' && coords[index2].type == 'd')){
        int x2 = (coords[index1].x-coords[index2].x) * (coords[index1].x-coords[index2].x);
        int y2 = (coords[index1].y-coords[index2].y) * (coords[index1].y-coords[index2].y);
        int fin = x2 + y2;
        return sqrt(fin);
    }
    return numeric_limits<double>::infinity();
}

double calculatedistfast(int index1, int index2){
    int temp1 = coords[index1].x;
    int temp2 = coords[index1].y;
    int temp3 = coords[index2].x;
    int temp4 = coords[index2].y;
    int x2 = (temp1-temp3) * (temp1-temp3);
    int y2 = (temp2-temp4) * (temp2-temp4);
    int fin = x2 + y2;
    return sqrt(fin);
    
}

void mst(){
    coords[0].distance = 0;
    double running_total = 0;
    for (int i = 0; i < (int)coords.size(); ++i) {
        int minDistIndex = -1;
        double minDist = numeric_limits<double>::infinity();
        for (int j = 0; j < (int)coords.size(); ++j) {
            if (!coords[j].visited && coords[j].distance < minDist) {
                minDist = coords[j].distance;
                minDistIndex = j;
            }
        }
        if(minDistIndex != -1){
            coords[minDistIndex].visited = true;
            running_total += coords[minDistIndex].distance;
            for (int j = 0; j < (int)coords.size(); ++j) {
                if (!coords[j].visited) {
                    double dist = calculatedist(minDistIndex, j);
                    if (dist < coords[j].distance && dist != -1) {
                        coords[j].distance = dist;
                        coords[j].prevcoord = minDistIndex;
                    }
                }
            }
        }
    }
    cout << running_total << "\n";
    for (int i=0; i<(int)coords.size(); i++) {
        if (coords[i].prevcoord != -1) { // If the vertex is connected to the MST
            if(i < coords[i].prevcoord) cout << i << " " << coords[i].prevcoord << endl;
            else cout << coords[i].prevcoord << " " << i << endl;
        }
    }
}

void fasttsp() {
    vector<int> tour = {0, 1};
    tour.reserve(coords.size());
    double total_distance = 2 * calculatedistfast(0, 1);
    int temp2 = (int)coords.size();
    for (int city = 2; city < temp2; ++city) {
        double min_increase = numeric_limits<double>::max();
        int position = -1;
        int temp = (int)tour.size();
        for (int i = 0; i < temp; ++i) {
            int other = (i + 1);
            if (other == temp) {
                other = 0;
            }
            double increase = calculatedistfast(tour[i], city) + calculatedistfast(city, tour[other]) - calculatedistfast(tour[i], tour[other]);
            if (increase < min_increase) {
                min_increase = increase;
                position = i;
            }
        }

        if (position != -1) {
            tour.insert(tour.begin() + position + 1, city);
            total_distance += min_increase;
        }
    }

    cout << total_distance << "\n";

    for(int i=0; i< (int)tour.size(); i++){
        cout << tour[i] << " ";
    }
    cout << "\n";
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
                if (optarg[0] == 'M') {
                    MST = true;
                }
                else if((optarg[0]=='F')) {
                    fast = true;
                }
                else{
                    optimal = true;
                }
				break;
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
