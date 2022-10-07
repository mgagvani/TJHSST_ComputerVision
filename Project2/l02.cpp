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
#define TEXTOUTFILE "squares.txt"

using namespace std;


class Point {
    private: // nothing goes here?
    public:
        double x, y;
        Point(double x, double y): x(x), y(y) {}
        Point(const Point & other): x(other.x), y(other.y) {}
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
        Point scaled_point(Point to_scale, double scalefactor) {
            double newx = to_scale.getX() * scalefactor;
            double newy = to_scale.getY() * scalefactor;
            return Point(newx, newy);
        }
        Point operator+(const Point& b) const { // operator over load (add points x and y)
            return Point(x + b.x, y + b.y);
        }
        Point operator-(const Point& b) const {
            return Point(x - b.x, y - b.y);
        }
        Point operator*(double scalar) const {
            return Point(x * scalar, y * scalar);
        }
        double getX() const { return x; }
        double getY() const { return y; }

        // RENDERING METHODS
        static void draw(Point p, int mat[][WIDTH], int color) {
            draw_point(p.getY(), p.getX(), 5, mat, color);
        }
};

/* UTILITY METHODS - WITH POINT */
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
        LineSegment rotateClockwiseAroundA() { // -90
            Point offset = b - a;
            Point newOffset = Point(offset.getY(), -offset.getX());
            return LineSegment(a, a + newOffset);
        }
        LineSegment rotateClockwiseAroundB() { // -90
            Point offset = a - b;
            Point newOffset = Point(offset.getY(), -offset.getX());
            return LineSegment(b + newOffset, b);
        }
        LineSegment rotateCounterClockwiseAroundA() { // +90
            Point offset = b - a;
            Point newOffset = Point(-offset.getY(), offset.getX());
            return LineSegment(a, a + newOffset);
        }
        LineSegment rotateCounterClockwiseAroundB() { // +90
            Point offset = a - b; 
            Point newOffset = Point(-offset.getY(), offset.getX());
            return LineSegment(b + newOffset, b);
        }
        LineSegment operator+(Point p) { // overload to add points together
            return LineSegment(a + p, b + p);
        }
        LineSegment operator-(Point p) {
            return LineSegment(a - p, b - p);
        }
        LineSegment operator*(double s) {
            return LineSegment(a * s, b * s);
        }
        Point getA() const { return a; }
        Point getB() const { return b; }

        // RENDERING METHODS
        static void draw(LineSegment l, int mat[][WIDTH],int color) {
            bresenham(l.a.x, l.a.y, l.b.x, l.b.y, mat, color);
        }
};

class Line {
    private:
        double a, b, c; // ax + by = c

    public:
        Line() {}
        ~Line() {}
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
            double det = a * other.b - b * other.a;

            if (det == 0) {
                throw 0;
            }

            double detX = c * other.b - b * other.c;
            double detY = a * other.c - c * other.a;

            return Point(detX / det, detY / det);
        }
        Line through(Point p) {
            double targetC = (p.getX() * a) + (p.getY() * b);
            return Line(a, b, targetC);
        }
        Line getPerpendicular() {
            return Line(-b, a, c);
        }
        static Line fromPoints(Point a, Point b) {
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

        // RENDERING METHODS
        static void draw(Line l, int mat[][WIDTH], int color) {
            Point left = Point(0, l.c/l.b); 
            Point right = Point(1, (l.c-l.a)/l.b);
            LineSegment seg = LineSegment(left, right);
            LineSegment::draw(seg, mat, color);
        }
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
        Point getPoint(int point) {
            return points.at(point);
        }
        LineSegment getSide(int side) {
            return sides.at(side);
        }
        std::vector<Point> getPoints() {
            return points;
        }
        std::vector<LineSegment> getSides() {
            return sides;
        }
        bool containsPoint(Point p) {
            int count = 0;
            for (int i = 0; i < (sides.size() - 1); i++) {
                if (sides.at(i).pointAbove(p)) {
                    count += 1;
                }
            }

            return (count % 2) != 0;
        }
        bool isConvex() {
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
                Polygon tmp (getRandomPolygonCounterclockwise(nsides));
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
                newPoint = newPoint.scaled_point(newPoint,0.5) + Point(0.5, 0.5);

                points.push_back(newPoint);
            }

            return Polygon(points);
        }

        // RENDERING METHODS
        static void draw(Polygon p, int mat[][WIDTH], int linecolor) {
            for(LineSegment seg: p.sides) {
                LineSegment::draw(seg, mat, linecolor);
            }
            for(Point p: p.points) {
                Point::draw(p, mat, 3);
            }
        }
};

// L02 METHODS
void perp_diagonal(int mat[][WIDTH]) {
    Polygon poly = Polygon::generateConvex(4);
    Line diagonal = Line::fromPoints(poly.points[0], poly.points[2]);
    Line perp = diagonal.getPerpendicular().through(poly.points[1]);
    Point intersect = diagonal.intersection(perp);
    LineSegment dropped_perp = LineSegment(intersect, poly.points[1]);
    
    // draw
    Polygon::draw(poly, mat, 2);
    Line::draw(diagonal, mat, 3);
    Point::draw(intersect, mat, 8);
    LineSegment::draw(dropped_perp, mat, 8);

    // output
    for(Point p: poly.points) {
        cout << p.x << " " << p.y << endl;
    }
}

Polygon four_point_square(int mat[][WIDTH], int i, Polygon poly) {
    Point A = poly.points[0];
    std::vector<int> available = {1,2,3};
    Point opp = poly.points[available.at(i%3)];

    available.erase(available.begin() + (i%3));

    Point o = poly.points[available.at(0)];
    available.erase(available.begin());

    Point last = poly.points[available.at(0)];
    available.erase(available.begin());

    LineSegment AC = LineSegment(A, opp);
    LineSegment BE = ((i % 2 == 0) ? AC.rotateClockwiseAroundA() : AC.rotateCounterClockwiseAroundA()) + (o - A);
    Point E = BE.getB();

    Line sideDE = Line::fromPoints(last, E);
    Line sideA = sideDE.getPerpendicular().through(A);
    Line sideC = sideDE.getPerpendicular().through(opp);
    Line sideB = sideDE.through(o);

    std::vector<Point> squarePoints;
    squarePoints.push_back(sideA.intersection(sideB));
    squarePoints.push_back(sideB.intersection(sideC));
    squarePoints.push_back(sideC.intersection(sideDE));
    squarePoints.push_back(sideDE.intersection(sideA));

    Polygon square = Polygon(squarePoints);

    // DEBUG
    Line::draw(sideDE, mat, 99);
    Line::draw(sideA, mat, 99);
    Line::draw(sideB, mat, 99);
    Line::draw(sideC, mat, 99);

    return square;
}

void write_square_info(std::vector<Polygon> squares, const char* filename) {
    // preprocess, find areas
    std::vector<double> areas;
    for(Polygon p: squares) {
        areas.push_back(p.getSide(0).length()*p.getSide(0).length());
    }
    double minArea = 2.00; /// the max actually is 1x1 which is 1
    for(double area: areas) {
        if(area < minArea) {
            minArea = area;
        }
    }
    
    // output file
    ofstream MyFile;
    MyFile.open(filename, ios::out);
    int i = 0;
    for(Polygon p: squares) {
        for(Point point: p.getPoints()) {
            MyFile << "(" << point.x << ", " << point.y << ")" << endl;
        }
        MyFile << "Area :" << areas[i] << endl;
        MyFile << endl;
        i++;
    }
    MyFile.close();

}


int main() {
    srand(time(nullptr));

    /* TESTS - L01 */
    // cout << dist(0.0, 6.0, 8.0, 0.0) << endl;
    // bresenham(100, 100, 400, 100, mat, 2); // HORIZONTAL
    // bresenham(100, 100, 100, 400, mat, 2); // VERTICAL
    // bresenham(100, 100, 400, 400, mat, 2); // POSITIVE SLOPE
    // bresenham(400, 100, 100, 400, mat, 2); // NEGATIVE SLOPE
    // for (int r = 2; r < 399; r += 140) { draw_circle(r, 400, 400, mat); }

    /* TESTS */
    // Point a = Point(0.1, 0.1); Point c = Point(0.9, 0.1);
    // Point b = Point(0.9, 0.9); Point d = Point(0.1, 0.9);
    // std::vector<Point> points = {a, b, c, d};
    // Point::draw(a, mat); Point::draw(b, mat);
    // LineSegment seg = LineSegment(a, b);
    // LineSegment::draw(seg, mat);
    // Polygon poly = Polygon::generateConvex(4);
    // Polygon::draw(poly, mat);
    Polygon poly = Polygon::generateConvex(4);
    std::vector<Polygon> squares;
    std::vector<string> outfiles = {"Images/out1.ppm", "Images/out2.ppm", "Images/out3.ppm", "Images/out4.ppm", "Images/out5.ppm", "Images/out6.ppm"};
    for(int i = 0; i < 6; i++) {
        int(*matrix)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix
        Polygon _s = four_point_square(matrix, i, poly);
        squares.push_back(_s);
        Polygon::draw(_s, matrix, 4);
        Polygon::draw(poly, matrix, 3);
        writePPM(outfiles[i].c_str(), HEIGHT, WIDTH, matrix);
    }
    write_square_info(squares, TEXTOUTFILE);
    int(*mat)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix
    perp_diagonal(mat);
    writePPM(OUTFILE, HEIGHT, WIDTH, mat);
}