#include "utils.h"

FLTYPE distance (Point a, Point b) {
	FLTYPE dist_squared = SQ(a.x - b.x) + SQ(a.y - b.y);

	FLTYPE dist = (FLTYPE) sqrt((double) dist_squared);

	return dist;
}


	



