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
#include <set>

using namespace std;

vector<Coordinate> coords;
int num_vertices;
bool MST = false;
bool fast = false;
bool optimal = false;

double fast_dist;
vector<int> fasttsp_tour;
vector<int> opt_tour;


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
        if(!fast){
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
}

double calculatedist(int index1, int index2){
    if((coords[index1].type == 's' && coords[index2].type == 's') || (coords[index1].type == 'w' || coords[index2].type == 'w') || (coords[index1].type == 'd' && coords[index2].type == 'd')){
        double temp1 = coords[index1].x;
        double temp2 = coords[index1].y;
        double temp3 = coords[index2].x;
        double temp4 = coords[index2].y;
        double x2 = (temp1-temp3) * (temp1-temp3);
        double y2 = (temp2-temp4) * (temp2-temp4);
        double fin = x2 + y2;
        return sqrt(fin);
    }
    return numeric_limits<double>::infinity();
}

double calculatedistfast(int index1, int index2){
    double temp1 = coords[index1].x;
    double temp2 = coords[index1].y;
    double temp3 = coords[index2].x;
    double temp4 = coords[index2].y;
    double x2 = (temp1-temp3) * (temp1-temp3);
    double y2 = (temp2-temp4) * (temp2-temp4);
    double fin = x2 + y2;
    return sqrt(fin);
}

void mst(){
    coords[0].distance = 0;
    double running_total = 0;
    int coords_size = (int)coords.size();

    // Initialize the vector of unvisited vertices
    vector<int> unvisited;
    for (int i = 0; i < coords_size; ++i) {
        unvisited.push_back(i);
    }

    while (!unvisited.empty()) {
        int minDistIndex = -1;
        double minDist = numeric_limits<double>::infinity();

        // Find the unvisited vertex with the minimum distance
        for (int idx : unvisited) {
            if (coords[idx].distance < minDist) {
                minDist = coords[idx].distance;
                minDistIndex = idx;
            }
        }

        if (minDistIndex != -1) {
            coords[minDistIndex].visited = true;
            running_total += coords[minDistIndex].distance;

            // Remove the visited vertex from the unvisited list
            unvisited.erase(remove(unvisited.begin(), unvisited.end(), minDistIndex), unvisited.end());

            // Update distances to the unvisited vertices
            for (int idx : unvisited) {
                double dist = calculatedist(minDistIndex, idx);
                if (dist < coords[idx].distance && dist != -1) {
                    coords[idx].distance = dist;
                    coords[idx].prevcoord = minDistIndex;
                }
            }
        } else {
            // Break the loop if no unvisited vertex is found
            break;
        }
    }

    cout << running_total << "\n";
    for (int i = 0; i < coords_size; i++) {
        if (coords[i].prevcoord != -1) {
            if(i < coords[i].prevcoord) cout << i << " " << coords[i].prevcoord << endl;
            else cout << coords[i].prevcoord << " " << i << endl;
        }
    }
}

void fasttsp() {
    vector<int> tour = {0, 1};
    tour.reserve(coords.size());
    double total_distance = 2 * calculatedistfast(0, 1);
    int coords_size = (int)coords.size();
    for (int city = 2; city < coords_size; ++city) {
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
    if(!optimal){
        cout << total_distance << "\n";

        for(int i=0; i< (int)tour.size(); i++){
            cout << tour[i] << " ";
        }
        cout << "\n";
    }
    if(optimal){
        fast_dist = total_distance;
        fasttsp_tour = tour;
    }
}

double optmst(const vector<int>& subset) {
    if(size(subset) == 1){
        return 0;
    }
    int coords_size = (int)coords.size();
    
    // Resetting the visited and distance for all vertices
    for (int i = 0; i < coords_size; ++i) {
        coords[i].visited = false;
        coords[i].distance = numeric_limits<double>::infinity();
    }

    // Start from the first vertex in the subset
    if (!subset.empty()) {
        coords[subset[0]].distance = 0;
    }

    double running_total = 0;
    vector<int> unvisited(subset);  // Use only the vertices in the subset

    while (!unvisited.empty()) {
        int minDistIndex = -1;
        double minDist = numeric_limits<double>::infinity();

        // Find the unvisited vertex with the minimum distance
        for (int idx : unvisited) {
            if (coords[idx].distance < minDist) {
                minDist = coords[idx].distance;
                minDistIndex = idx;
            }
        }

        if (minDistIndex != -1) {
            coords[minDistIndex].visited = true;
            running_total += coords[minDistIndex].distance;

            // Remove the visited vertex from the unvisited list
            unvisited.erase(remove(unvisited.begin(), unvisited.end(), minDistIndex), unvisited.end());

            // Update distances to the unvisited vertices in the subset
            for (int idx : unvisited) {
                double dist = calculatedistfast(minDistIndex, idx);
                if (dist < coords[idx].distance && dist != -1) {
                    coords[idx].distance = dist;
                    coords[idx].prevcoord = minDistIndex;
                }
            }
        } else {
            // Break the loop if no unvisited vertex is found
            break;
        }
    }
    //cout << "mst of " << subset.size() << ": " << running_total << "\n";
    return running_total;
}

bool promising(vector<int> path, int permLength){
    double currCost = 0;
    for (int i = 0; i < permLength-1; ++i) {
        currCost += calculatedistfast(path[i], path[i + 1]);
    }
    std::vector<int> rest_of_vertices(path.begin() + permLength, path.end());
    double mstCost = optmst(rest_of_vertices);
    double currDist = currCost;
    double arm1Len = numeric_limits<double>::infinity();
    double arm2Len = numeric_limits<double>::infinity();
    for(int i=permLength; i<(int)path.size(); i++){
        double temp = calculatedistfast(path[0], path[i]);
        if(temp < arm1Len){
            arm1Len = temp;
        }
    }
    for(int i=permLength; i<(int)path.size(); i++){
        double temp = calculatedistfast(path[permLength-1], path[i]);
        if(temp < arm2Len){
            arm2Len = temp;
        }
    }
    currDist += mstCost + arm1Len + arm2Len;
    bool promise = currDist < fast_dist;
    for (size_t i = 0; i < path.size(); ++i){
        cout << setw(2) << path[i] << ' ';
    }
    //cout << setw(4) << permLength << setw(10) << currCost;
    //cout << setw(10) << arm1Len << setw(10) << arm2Len;
    if(promise){
        //cout << setw(10) << mstCost << setw(10) << currDist << "  " << "true" << '\n';
        if(mstCost==0){
            //cout << "New best cost achieved: " << currDist << "\n";
            opt_tour = path;
            fast_dist = currDist;
        }
    }
    //else cout << setw(10) << mstCost << setw(10) << currDist << "  " << "false" << '\n';
    return promise;
}

void genPerms(vector<int> path, int permLength) { //permlength = how many indices are fixed
    if (permLength == (int)path.size()) {
        // Do something with the path
        //currCost += (closing edge);
        //check if it's better
            //update things
        //currCost -= (closing edge);
        double currDist = 0;
        for (int i = 0; i < permLength - 1; ++i) {
            currDist += calculatedist(path[i], path[i + 1]);
        }
        currDist += calculatedist(path[permLength - 1], path[0]);  // Return to the starting city
        if (currDist < fast_dist) {
            fast_dist = currDist;
            fasttsp_tour = path;
        }
        return;
    }  // if ..complete path

    /* cout << "looking at path with permlength " << permLength << ": ";
    for(int i=0; i< (int)path.size(); i++){
        cout << path[i] << " ";
    } */
    

    if (!promising(path, permLength)) {
        return;
    }  // if ..not promising

    /* if(permLength == num_vertices-1){
        double tempcost = 0;
        for(int i=0; i< (int)path.size(); i++){
            tempcost += calculatedistfast(path[i], path[i+1]);
        }
        cout << "path cost: " << tempcost << "\n";
        if( tempcost < fast_dist){
            fast_dist = tempcost;
        }
    } */

    for (size_t i = permLength; i < path.size(); ++i) {
        swap(path[permLength], path[i]);
        //currCost += (cost of new edge);
        genPerms(path, permLength + 1);
        //currCost += (cost of that same edge);
        swap(path[permLength], path[i]);
    }  // for ..unpermuted elements
}  // genPerms()

void opttsp(){
    std::vector<int> indices(num_vertices);  // Create a vector of the required size
    for(int i = 0; i < num_vertices; ++i) {
        indices[i] = i;  // Fill the vector with increasing indices
    }
    fasttsp();
    genPerms(indices, 1);
    cout << fast_dist << "\n";
    
    for(int i=0; i< (int)opt_tour.size(); i++){
        cout << opt_tour[i] << " ";
    }
}

int main(int argc, char * argv[]) {
    cout << std::setprecision(2);
    cout << std::fixed;
    //cerr << fixed << showpoint << setprecision(2) << boolalpha;
	struct option longOpts[] = {
		{"help",no_argument,NULL,'h'},
		{"mode",required_argument,NULL,'m'},
		{NULL,0,NULL,0}
	};
    
	opterr = 1;
	int opt = 0, index = 0;
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
