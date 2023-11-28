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
	bool visited;
	double distance;
	
    long x;
    long y;
    long index;
    long prevcoord;
    char type;
    
    Coordinate(long index_in, long x_in, long y_in, char type_in)
        : x(x_in), y(y_in), index(index_in), type(type_in){
        visited = false;
        distance = numeric_limits<double>::infinity();
        prevcoord = -1;
    };
    
    ~Coordinate(){};
};
