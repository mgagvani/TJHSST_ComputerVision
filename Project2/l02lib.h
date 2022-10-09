#include "../Project1/l01.h"

#include <stdlib.h>
#include <vector>

#define OUTFILE "l02.ppm"
#define TEXTOUTFILE "squares.txt"

class Point {
    public:
        double x, y;
        Point(double x, double y);
        Point(const Point & other);
        Point();
        ~Point();
        double distance(Point a, Point b);
        double distance(const Point other) const;
        double magnitude() const;
        Point scaled_point(Point to_scale, double scalefactor);
        Point operator+(const Point& b) const;
        Point operator-(const Point& b) const;
        Point operator*(double scalar) const;
        double getX() const;
        double getY() const;
        void draw(int mat[][WIDTH], int color);
};

bool between(double a, double b, double p);

double polygon_area(std::vector<Point> points);

bool check_collinear(std::vector<Point> points);

std::vector<double> polygon_centroid(std::vector<Point> polygon);

class LineSegment {
    private:
        Point a, b;
    public:
        LineSegment(Point a, Point b);
        ~LineSegment();
        LineSegment(double x1, double y1, double x2, double y2);
        double length() const;
        double slope() const;
        bool pointAbove(Point p) const;
        LineSegment rotateClockwiseAroundA();
        LineSegment rotateClockwiseAroundB();
        LineSegment rotateCounterClockwiseAroundA();
        LineSegment rotateCounterClockwiseAroundB();
        LineSegment operator+(Point p);
        LineSegment operator-(Point p);
        LineSegment operator*(double s);
        Point getA() const;
        Point getB() const;
        void draw(int mat[][WIDTH],int color);
};

class Line {
    private:
        double a, b, c;
    public:
        Line();
        ~Line();
        Line(double a, double b, double c);
        Line(Point p1, Point p2);
        double getA() const;
        double getB() const;
        double getC() const;
        double getSlope() const;
        double getIntercept() const;
        bool isParallel(Line other);
        bool isPerpendicular(const Line& other) const;
        Point intersection(const Line& other) const;
        Line through(Point p);
        Line getPerpendicular();
        Line operator*(double scalar) const;
        void draw(int mat[][WIDTH], int color);
};

class Polygon {
    private:
        std::vector<Point> points;
        std::vector<LineSegment> sides;
    public:
        Polygon(std::vector<Point> points);
        Point getPoint(int point);
        LineSegment getSide(int side);
        std::vector<Point> getPoints();
        std::vector<LineSegment> getSides();
        bool containsPoint(Point p);
        bool isConvex();
        void draw(int mat[][WIDTH], int linecolor);
};