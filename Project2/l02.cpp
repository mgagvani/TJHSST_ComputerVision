#include "../Project1/l01.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <vector>

#define OUTFILE "l02.ppm"

using namespace std;

class Point
{
private:
public:
    double x;
    double y;
    Point() {
        x = 1.0 * rand() / RAND_MAX;
        y = 1.0 * rand() / RAND_MAX;
        cout << "default constructor" << endl;
    }
    Point(const Point &p) {
        x = p.x;
        y = p.y;
        //
        cout << "copy constructor" << endl;
    }
    ~Point() {
        cout << "destructor" << endl;
    }
    double d(const Point &p) {
        double dx = x - p.x;
        double dy = y - p.y;
        return sqrt(dx * dx + dy * dy);
    }
};

void draw_point(Point p, int size, int mat[][WIDTH]) {
    for(int i = 2; i < size; i++) {
        draw_circle(i, scale_double(p.y, HEIGHT), scale_double(p.x, WIDTH), mat);
    }
}

void draw_polygon(vector<Point> points, int mat[][WIDTH]) {
    // have to order first - TODO
    for(int i = 0; i < points.size() - 1; i++) {
        bresenham(scale_double(points.at(i).x, WIDTH), scale_double(points.at(i).y, HEIGHT), scale_double(points.at(i+1).x, WIDTH), scale_double(points.at(i+1).y, HEIGHT), mat);
    }
}

vector<Point> generate_convex_points() {
    vector<Point> toRet;
    for(int i = 0; i < 4; i++) {
        Point a; toRet.push_back(a);
    }
    return toRet;
}



int main() {
    srand(time(nullptr));
    int(*mat)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix

    // TESTS - L01
    cout << dist(0.0, 6.0, 8.0, 0.0) << endl;
    bresenham(100, 100, 400, 100, mat); // HORIZONTAL
    bresenham(100, 100, 100, 400, mat); // VERTICAL
    bresenham(100, 100, 400, 400, mat); // POSITIVE SLOPE
    bresenham(400, 100, 100, 400, mat); // NEGATIVE SLOPE
    for (int r = 2; r < 399; r += 100) { draw_circle(r, 400, 400, mat); }

    // TESTS - Point class
    Point a;
    Point b(a);
    Point* p;
    p = new Point;
    cout << ( p->d(a) ) << endl ;
    cout << ( a.d(*p) ) << endl ;
    delete p;

    // L02
    vector<Point> points = generate_convex_points();
    for(int i = 0; i < points.size(); i++) {
        draw_point(points.at(i), 5, mat);
    }
    draw_polygon(points, mat);

    writePPM(OUTFILE, HEIGHT, WIDTH, mat);

}