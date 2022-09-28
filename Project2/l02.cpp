#include "../Project1/l01.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <vector>
#include <algorithm>

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
        // cout << "default constructor" << endl;
    }
    Point(const Point &p) {
        x = p.x;
        y = p.y;
        //
        // cout << "copy constructor" << endl;
    }
    ~Point() {
        // cout << "destructor" << endl;
    }
    double dist(const Point &p) {
        double dx = x - p.x;
        double dy = y - p.y;
        return sqrt(dx * dx + dy * dy);
    }
};

class Line {
    private:
    public:
        Point start;
        Point end; 
        Line(Point a, Point b) {
            start = a;
            end = b;
        }
        Line(const Line &l) {
            start = l.start;
            end = l.end;
        }
        ~Line() {
            // destructo
        }
        double length() {
            return start.dist(end);
        }
};

vector<double> polygon_centroid(vector<Point> polygon) {
    int num_points = polygon.size();
    double sumx = 0; double sumy = 0;
    for(int i = 0; i < num_points; i++) {
        sumx += polygon.at(i).x;
        sumy += polygon.at(i).y;
    }
    double avgx = sumx/num_points; double avgy = sumy/num_points;
    vector<double> toRet;
    toRet.push_back(avgx); toRet.push_back(avgy);
    return toRet;
}

void draw_point(Point p, int size, int mat[][WIDTH]) {
    for(int i = 2; i < size; i++) {
        draw_circle(i, scale_double(p.y, HEIGHT), scale_double(p.x, WIDTH), mat);
    }
}
bool check_coincidence(vector<Point> points) {
    for(int i = 0; i < points.size(); i++) {
        for(int j = 0; j < points.size(); j++) {
            if(points[i].dist(points[j]) < 1e-4 && i != j) {
                return true;
            }
        }
    }
    return false;
}

double polygon_area(vector<Point> points) {
    double area = 0.0;
    int j = points.size() - 1;

    for(int i = 0; i < points.size(); i++) {
        area += (points[i].x + points[j].x) * (points[i].y + points[j].y);
        j = i;
    }

    return abs(area/2.0);
}

bool check_collinear(vector<Point> points) {
    return (polygon_area(points) < 1e-4);
}

vector<Point> generate_convex_points() {
    vector<Point> toRet;
    for(int i = 0; i < 4; i++) {
        Point a; toRet.push_back(a);
    }
    return toRet;
}

void draw_polygon(vector<Point> points, int mat[][WIDTH]) {
    // check for coincident points
    bool coincident = check_coincidence(points); bool collinear = check_collinear(points);
    while(coincident || collinear) {
        // regenerate points
        points = generate_convex_points();
        coincident = check_coincidence(points); collinear = check_collinear(points);
    }
    
    // order points
    vector<double> centroid = polygon_centroid(points); double cx = centroid.at(0); double cy = centroid.at(1);
    cout << "Calculated centroid" << endl;
    vector<pair<double, Point>> angles_to_centroid; double currangle; pair<double, Point> entry;
    for(int a = 0; a < points.size(); a++) {
        currangle = atan2(points.at(a).y - cy, points.at(a).x - cx); 
        entry.first = currangle; entry.second = points.at(a);
        angles_to_centroid.push_back(entry);
    } int size = points.size()-1;
    cout << "Calculated angles to centroid" << endl;
    // sorting (selection sort)
    for(int step = 0; step < size+1; step++) { // iterate from start to end (minus one)
        int min_idx = step;                     // the minimum for the slice we are considering is at the lowest index
        for(int i = step + 1; i < size+1; i++) {  // iterate again over the slice
            if(angles_to_centroid[i].first > angles_to_centroid[min_idx].first) { // if the one I am looking at in my slice is smaller
                min_idx = i;                                                      // make it the "min idx"
            }
        }
        

        pair<double, Point> temp = angles_to_centroid[step];
        angles_to_centroid[step] = angles_to_centroid[min_idx];
        angles_to_centroid[min_idx] = temp;
    }
    cout << "Printing out angles_to_centroid" << endl;
    for(int a = 0; a < points.size(); a++) {
        cout << angles_to_centroid[a].first << " - " << angles_to_centroid[a].second.x << ", " << angles_to_centroid[a].second.y << endl;
    }
    


    // draw points
    for(int i = 0; i < points.size()-1; i++) {
        bresenham(scale_double(points.at(i).x, WIDTH), scale_double(points.at(i).y, HEIGHT), scale_double(points.at(i+1).x, WIDTH), scale_double(points.at(i+1).y, HEIGHT), mat);
    }
    bresenham(scale_double(points.at(0).x, WIDTH), scale_double(points.at(0).y, HEIGHT), scale_double(points.at(points.size()-1).x, WIDTH), scale_double(points.at(points.size() - 1).y, HEIGHT), mat);
}


int main() {
    srand(time(nullptr));
    int(*mat)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix

    // TESTS - L01
    // cout << dist(0.0, 6.0, 8.0, 0.0) << endl;
    // bresenham(100, 100, 400, 100, mat); // HORIZONTAL
    // bresenham(100, 100, 100, 400, mat); // VERTICAL
    // bresenham(100, 100, 400, 400, mat); // POSITIVE SLOPE
    // bresenham(400, 100, 100, 400, mat); // NEGATIVE SLOPE
    // for (int r = 2; r < 399; r += 140) { draw_circle(r, 400, 400, mat); }

    // TESTS - Point class
    Point a;
    Point b(a);
    Point* p;
    p = new Point;
    // cout << ( p->d(a) ) << endl ;
    // cout << ( a.d(*p) ) << endl ;
    delete p;

    // L02
    vector<Point> points = generate_convex_points();
    cout << "Generated Points" << endl;
    for(int i = 0; i < points.size(); i++) {
        draw_point(points.at(i), 5, mat);
    }
    cout << "Finished drawing points" << endl;
    draw_polygon(points, mat);
    cout << "Finished drawing polygon" << endl;

    writePPM(OUTFILE, HEIGHT, WIDTH, mat);

}