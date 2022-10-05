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

/* UTILITY METHODS */
bool between(double a, double b, double p) {
    if (a < b) {
        return a <= p && p <= b;
    } else {
        return b <= p && p <= a;
    }
}

double polygon_area(vector<Point> points) { // note - this does not use polygon class
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

class Point {
    private: // nothing goes here?
    public:
        double x, y;
        Point(double x, double y): x(x), y(y) {}
        Point(const Point& point): x(point.x), y(point.y) {}
        Point() {}
        ~Point() {} // nothing happens
        static double distance(const Point& a, const Point& b) {
            return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }
        double distance(const Point& other) const {
            return Point::distance(*this, other);
        }
        double magnitude() const {
            return sqrt(x * x + y * y);
        }
        double getX() const { return x; }
        double getY() const { return y; }
};

class Line {
    private:
        double a, b, c; // ax + by = c

    public:
        Line() {}
        Line(double a, double b, double c):
            a(a), // set the private variables
            b(b),
            c(c) {} 

        double getA() const { return a; }
        double getB() const { return b; }
        double getC() const { return c; }

        double getSlope() const {
            return -a / b;
        }
        double getIntercept() const {
            return c / b;
        }
        bool isParallel(Line other) {
            if (b == 0) {
                return other.b == 0;
            } else {
                if (other.b == 0) {
                    return false;
                } else {
                    return other.getSlope() == getSlope();
                }
            }
        }

        bool isPerpendicular(const Line& other) const {
            if (b == 0) {
                return other.a == 0;
            } else {
                if (other.a == 0) {
                    return false;
                } else {
                    return (other.b / other.a) == (-a / b);
                }
            }
        }

        Point intersection(const Line& other) const {
            // matrix is:
            /*
            | a1 b1 | = | c1 |
            | a2 b2 |   | c2 |

            We will use Cramer's rule to find the (x, y) of the intersection.
            */

            double det = a * other.b - b * other.a;

            if (det == 0) {
                throw 0;
            }

            double detX = c * other.b - b * other.c;
            double detY = a * other.c - c * other.a;

            return Point(detX / det, detY / det);
        }

        Line through(const Point& p) const {
            double targetC = (p.getX() * a) + (p.getY() * b);
            return Line(a, b, targetC);
        }

        Line getPerpendicular() const {
            // ax + by = c
            // by = -ax + c
            // ay = bx + c
            // -bx + ay = c
            return Line(-b, a, c);
        }

        static Line fromPoints(Point a, Point b) {
            
            /*
            equations are ax + by = c
            how to find a, b, and c?
            y = mx + b
            m = slope_numer/slope_denom
            y = slope_numer/slope_denom * x + b
            slope_denom(y) = slope_numer(x) + b
            -slope_numer(x) + slope_denom(y) = b
            now, substitute a point for (x, y) to find b. we can just use (x1, y1)
            b = -slope_numer(x1) + slope_denom(y1)
            now, we know that in ax + by = c:
            a = -slope_numer
            b = slope_denom
            c = a(x1) + b(y1)
            */
            double slopeNumer, slopeDenom;
            slopeNumer = b.getY() - a.getY();
            slopeDenom = b.getX() - a.getX();

            double newA = -slopeNumer;
            double newB = slopeDenom;
            double newC = newA * (a.getX()) + newB * (a.getY());

            return Line(newA, newB, newC);
        }

        Line operator*(double scalar) const {
            return Line(a, b, c * scalar);
        }
};

class LineSegment {
    private:
        Point a, b;

    public:
        LineSegment(Point a, Point b): a(a), b(b) {}
        ~LineSegment() {} // nothing happens

        LineSegment(double x1, double y1, double x2, double y2):
            a(Point(x1, y2)),
            b(Point(x2, y2)) {}

        double length() const {
            return Point::distance(a, b);
        }
        double slope() const {
            return (b.getY() - a.getY()) / (b.getX() - a.getX());
        }
        bool pointAbove(Point p) const {
            // vertical line must intersect
            // first, finds the slope of the line
            if (!between(a.getX(), b.getX(), p.getX())) {
                return false;
            }

            // equation: y = mx + b
            // a[1] = ma[0] + b
            double intercept = a.getY() - slope() * a.getX();
            double segmentY = slope() * p.getX() + intercept;
            return p.getY() > segmentY;
        }
        Line getLine() const {
            return Line::fromPoints(a, b);
        }
        Point getA() const { return a; }
        Point getB() const { return b; }
};

class Polygon {
    public:
        std::vector<Point> points;
        std::vector<LineSegment> sides;

        Polygon(std::vector<Point> points) {
            for (int i = 0; i < points.size(); i++) {
                this -> points.push_back(points.at(i));
                this -> sides.push_back(LineSegment(points.at(i), points.at((i + 1) % points.size())));
            }
        }

        Point getPoint(int point) const {
            return points.at(point);
        }

        LineSegment getSide(int side) const {
            return sides.at(side);
        }

        std::vector<Point> getPoints() const {
            return points;
        }

        std::vector<LineSegment> getSides() const {
            return sides;
        }

        bool containsPoint(Point p) const {
            int count = 0;
            for (int i = 0; i < (sides.size() - 1); i++) {
                if (sides.at(i).pointAbove(p)) {
                    count += 1;
                }
            }

            return (count % 2) != 0;
        }

        bool isConvex() const {
            for (int checkPointIndex = 0; checkPointIndex < 4; checkPointIndex++) {
                std::vector<Point> otherPoints;
                Point checkPoint = points.at(checkPointIndex);
                for (int otherPointIndex = 0; otherPointIndex < 4; otherPointIndex++) {
                    if (otherPointIndex != checkPointIndex) {
                        otherPoints.push_back(points[otherPointIndex]);
                    }
                }

                Polygon otherPointsPolygon(otherPoints);

                if (otherPointsPolygon.containsPoint(checkPoint)) {
                    return false;
                }
            }

            return true;
        }

        static Polygon generateConvex(int nsides) {
            while (true) {
                std::cout << "Generating...\n";
                Polygon tmp (getRandomPolygon(nsides));

                if (tmp.isConvex()) {
                    return tmp;
                }
            }
        }

        static Polygon getRandomPolygon(int nsides) {
            std::vector<Point> points;
            for (int i = 0; i < nsides; i++) {
                points.push_back(Point(generate_pos(), generate_pos()));
            }
            return Polygon(points);
        }

        /**
         * Guaranteed to generate in counterclockwise order
         * 1) Generates angles in counterclockwise order
         * 2) Generates magnitudes randomly
         * 3) Generates points based on angles and magnitudes
         */
        static Polygon getRandomPolygonCounterclockwise(int nsides) {
            double* slices = new double[nsides];
            double sum = 0;
            double circleRads = 6.28318531;

            for (int i = 0; i < nsides; i++) {
                // generate proportions for 'slices'
                slices[i] = generate_pos();

                // keep track of the sum of the slices so we can scale them to 2pi
                sum += slices[i];
            }
            
            std::vector<Point> points;
            double angle = generate_pos();
            for (int i = 0; i < nsides; i++) {
                angle += circleRads * slices[i] / sum;

                // scale cos/sin to reach the square
                double maxMagnitude;
                double width = abs(cos(angle));
                double height = abs(sin(angle));
                if (width > height) {
                    maxMagnitude = 1 / width;
                } else {
                    maxMagnitude = 1 / height;
                }

                double magnitude = maxMagnitude * generate_pos();

                // Generate with magnitude/direction
                Point newPoint(magnitude * cos(angle), magnitude * sin(angle));
                // Fit to the unit square
                newPoint = (newPoint * 0.5) + Point(0.5, 0.5);

                points.push_back(newPoint);

                std::cout << "Generating point; m=" << magnitude;
                std::cout << "; theta=" << (angle * 180 / 3.14159265) << "\n";
            }

            return Polygon(points);
        }
        static Polygon fromFile(const char* filename, int nsides) {
            std::ifstream file(filename);
            std::vector<Point> points;
            for (int i = 0; i < nsides; i++) {
                double x, y;
                file.ignore(1); // '('
                file >> x;
                file.ignore(1); // ','
                file >> y;
                file.ignore(1); // ')'

                if (i < nsides - 1) {
                    file.ignore(3); // ' , '
                }

                points.push_back(Point(x, y));
            }

            file.close();

            return Polygon(points);
        }
        int length() const {
            return points.size();
        }
        double perimeter() const {
            double acc = 0;
            for (auto it : sides) { // google "auto class c++ looping"
                acc += it.length();
            }
            return acc;
        }
};



void draw_point(Point p, int size, int mat[][WIDTH]) {
    for(int i = 2; i < size; i++) {
        draw_circle(i, scale_double(p.y, HEIGHT), scale_double(p.x, WIDTH), mat);
    }
}
bool check_coincidence(vector<Point> points) {
    for(int i = 0; i < points.size(); i++) {
        for(int j = 0; j < points.size(); j++) {
            if(points[i].distance(points[j]) < 1e-4 && i != j) {
                return true;
            }
        }
    }
    return false;
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
    // vector<Point> points = generate_convex_points();
    // cout << "Generated Points" << endl;
    Point p1(0.43, 0.39); Point p2(0.67, 0.61); Point p3(0.63,0.38); Point p4(0.4,0.6);
    vector<Point> points;
    points.push_back(p4); points.push_back(p2); points.push_back(p3); points.push_back(p1);
    for(int i = 0; i < points.size(); i++) {
        draw_point(points.at(i), 5, mat);
    }
    cout << "Finished drawing points" << endl;
    draw_polygon(points, mat);
    cout << "Finished drawing polygon" << endl;

    writePPM(OUTFILE, HEIGHT, WIDTH, mat);

}