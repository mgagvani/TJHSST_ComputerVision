#include "../Project1/l01.h"
#include "l02lib.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;



Point::Point(double x, double y): x(x), y(y) {}
Point::Point(const Point & other): x(other.x), y(other.y) {}
Point::Point() {}
Point::~Point() {} // nothing happens
double Point::distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
double Point::distance(const Point other) const {
    Point other_new = Point(other);
    return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}
double Point::magnitude() const {
    return sqrt(x * x + y * y);
}
Point Point::scaled_point(Point to_scale, double scalefactor) {
    double newx = to_scale.getX() * scalefactor;
    double newy = to_scale.getY() * scalefactor;
    return Point(newx, newy);
}
Point Point::operator+(const Point& b) const { // operator over load (add points x and y)
    return Point(x + b.x, y + b.y);
}
Point Point::operator-(const Point& b) const {
    return Point(x - b.x, y - b.y);
}
Point Point::operator*(double scalar) const {
    return Point(x * scalar, y * scalar);
}
double Point::getX() const { return x; }
double Point::getY() const { return y; }
// RENDERING METHODS
void Point::draw(int mat[][WIDTH], int color) {
    draw_point(y, x, 5, mat, color);
}


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

    for(unsigned long int i = 0; i < points.size(); i++) {
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


LineSegment::LineSegment(Point a, Point b): a(a), b(b) {}
LineSegment::~LineSegment() {} // nothing happen
LineSegment::LineSegment(double x1, double y1, double x2, double y2):
    a(Point(x1, y1)),
    b(Point(x2, y2)) {}
double LineSegment::length() const {
    return a.distance(b);
}
double LineSegment::slope() const {
    return (b.getY() - a.getY()) / (b.getX() - a.getX());
}
bool LineSegment::pointAbove(Point p) const {
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
LineSegment LineSegment::rotateClockwiseAroundA() { // -90
    Point offset = b - a;
    Point newOffset = Point(offset.getY(), -offset.getX());
    return LineSegment(a, a + newOffset);
}
LineSegment LineSegment::rotateClockwiseAroundB() { // -90
    Point offset = a - b;
    Point newOffset = Point(offset.getY(), -offset.getX());
    return LineSegment(b + newOffset, b);
}
LineSegment LineSegment::rotateCounterClockwiseAroundA() { // +90
    Point offset = b - a;
    Point newOffset = Point(-offset.getY(), offset.getX());
    return LineSegment(a, a + newOffset);
}
LineSegment LineSegment::rotateCounterClockwiseAroundB() { // +90
    Point offset = a - b; 
    Point newOffset = Point(-offset.getY(), offset.getX());
    return LineSegment(b + newOffset, b);
}
LineSegment LineSegment::operator+(Point p) { // overload to add points together
    return LineSegment(a + p, b + p);
}
LineSegment LineSegment::operator-(Point p) {
    return LineSegment(a - p, b - p);
}
LineSegment LineSegment::operator*(double s) {
    return LineSegment(a * s, b * s);
}
Point LineSegment::getA() const { return a; }
Point LineSegment::getB() const { return b; }
// RENDERING METHODS
void LineSegment::draw(int mat[][WIDTH],int color) {
    bresenham(a.x, a.y, b.x, b.y, mat, color);
}



Line::Line() {}
Line::~Line() {}
Line::Line(double a, double b, double c):
    a(a), // set the private variables
    b(b),
    c(c) {}
Line::Line(Point p1, Point p2) {
    double slopeNumer, slopeDenom;
    slopeNumer = p2.getY() - p1.getY();
    slopeDenom = p2.getX() - p1.getX();
    double newA = -slopeNumer;
    double newB = slopeDenom;
    double newC = newA * (p1.getX()) + newB * (p1.getY());
    a = newA; b = newB; c = newC;
}
double Line::getA() const { return a; }
double Line::getB() const { return b; }
double Line::getC() const { return c; }
double Line::getSlope() const {
    return -a / b;
}
double Line::getIntercept() const {
    return c / b;
}
bool Line::isParallel(Line other) {
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
bool Line::isPerpendicular(const Line& other) const {
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
Point Line::intersection(const Line& other) const {
    double det = a * other.b - b * other.a;
    if (det == 0) {
        throw 0;
    }
    double detX = c * other.b - b * other.c;
    double detY = a * other.c - c * other.a;
    return Point(detX / det, detY / det);
}
Line Line::through(Point p) {
    double targetC = (p.getX() * a) + (p.getY() * b);
    return Line(a, b, targetC);
}
Line Line::getPerpendicular() {
    return Line(-b, a, c);
}
Line Line::operator*(double scalar) const {
    return Line(a, b, c * scalar);
}
// RENDERING METHODS
void Line::draw(int mat[][WIDTH], int color) {
    Point left = Point(0, c/b); 
    Point right = Point(1, (c-a)/b);
    LineSegment seg = LineSegment(left, right);
    seg.draw(mat,color);
}


Polygon::Polygon(std::vector<Point> points) {
    for (int i = 0; i < points.size(); i++) {
        this -> points.push_back(points.at(i));
        this -> sides.push_back(LineSegment(points.at(i), points.at((i + 1) % points.size())));
    }
}
Point Polygon::getPoint(int point) {
    return points.at(point);
}
LineSegment Polygon::getSide(int side) {
    return sides.at(side);
}
std::vector<Point> Polygon::getPoints() {
    return points;
}
std::vector<LineSegment> Polygon::getSides() {
    return sides;
}
bool Polygon::containsPoint(Point p) {
    int count = 0;
    for (int i = 0; i < (sides.size() - 1); i++) {
        if (sides.at(i).pointAbove(p)) {
            count += 1;
        }
    }
    return (count % 2) != 0;
}
bool Polygon::isConvex() {
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
// RENDERING METHODS
void Polygon::draw(int mat[][WIDTH], int linecolor) {
    for(LineSegment seg: sides) {
        seg.draw(mat, linecolor);
    }
    for(Point p: points) {
        p.draw(mat, 3);
    }
}
