#include "utils.h"

void test_dist() {
	Point a, b;
	FLTYPE x, y;
	FLTYPE dist;
	

	printf("Enter x,y for first point: ");
	scanf("%f,%f", &x, &y);


	a.x = x;
	a.y = y;
	
	printf("Enter x,y for second point: ");
	scanf("%f,%f", &x, &y);

	b.x = x;
    b.y = y;

	dist = distance(a, b);

	printf("Distance is %f\n", dist);	
}

int main() {
	test_dist();
}
