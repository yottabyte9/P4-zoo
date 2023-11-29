// Project UID 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include <cmath>
#include <limits>
#include <queue>

using namespace std;

struct MinHeapNode {
    long vertex;
    double distance;
};

struct Compare {
    bool operator()(const MinHeapNode& lhs, const MinHeapNode& rhs) {
        return lhs.distance > rhs.distance;
    }
};

struct Coordinate {
	bool visited = false;
	double distance = numeric_limits<double>::infinity();
	
    int x;
    int y;
    int prevcoord = -1;
    char type;
    
};
