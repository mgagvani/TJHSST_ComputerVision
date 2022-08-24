#include <stdio.h>
#include <math.h>

#define FLTYPE float
#define SQ(x) (x)*(x)

typedef struct point {
	FLTYPE x;
	FLTYPE y;
} Point;

FLTYPE distance(Point a, Point b);


