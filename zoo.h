#include <cmath>
#include <limits>
using namespace std;

struct Coordinate {
	bool visited;
	double distance;
	
    long x;
    long y;
    long index;
    long prevcoord;
    
    Coordinate(long index_in, long x_in, long y_in)
        : x(x_in), y(y_in), index(index_in){
        visited = false;
        distance = numeric_limits<double>::infinity();
        prevcoord = -1;
    };
    
    ~Coordinate(){};
};