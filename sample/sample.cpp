#include <l01.h>
#include <l02lib.h>

int main() {
    int(*matrix)[WIDTH] = new int[HEIGHT][WIDTH];
    Point a = Point(0.1, 0.1);
    Point b = Point(0.2, 0.2);
    Point c = Point(0.2, 0.7);
    LineSegment seg = LineSegment(a,b);
    LineSegment seg2 = LineSegment(b,c);
    seg.draw(matrix, 1);
    seg2.draw(matrix, 2);

    writePPM("sample.ppm", HEIGHT, WIDTH, matrix);

    return 0;
}

