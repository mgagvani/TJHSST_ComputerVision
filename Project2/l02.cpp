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

#define OUTFILE "l02.ppm"
#define TEXTOUTFILE "squares.txt"

using namespace std;

Polygon getRandomPolygonCounterclockwise(int nsides) {
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
Polygon generateConvex(int nsides) {
    while (true) {
        Polygon tmp (getRandomPolygonCounterclockwise(nsides));
        if (tmp.isConvex()) {
            return tmp;
        }
    }
}
Polygon getRandomPolygon(int nsides) {
    std::vector<Point> points;
    for (int i = 0; i < nsides; i++) {
        points.push_back(Point(generate_pos(), generate_pos()));
    }
    return Polygon(points);
}

// L02 METHODS
void perp_diagonal(int mat[][WIDTH]) {
    Polygon poly = generateConvex(4);
    Line diagonal = Line(poly.getPoints()[0], poly.getPoints()[2]);
    Line perp = diagonal.getPerpendicular().through(poly.getPoints()[1]);
    Point intersect = diagonal.intersection(perp);
    LineSegment dropped_perp = LineSegment(intersect, poly.getPoints()[1]);
    
    // draw
    poly.draw(mat, 2);
    diagonal.draw(mat, 3);
    intersect.draw(mat, 8);
    dropped_perp.draw(mat, 8);

    // output
    for(Point p: poly.getPoints()) {
        cout << p.x << " " << p.y << endl;
    }
}

Polygon four_point_square(int mat[][WIDTH], int i, Polygon poly) {
    Point A = poly.getPoints()[0];
    std::vector<int> available = {1,2,3};
    Point opp = poly.getPoints()[available.at(i%3)];

    available.erase(available.begin() + (i%3));

    Point o = poly.getPoints()[available.at(0)];
    available.erase(available.begin());

    Point last = poly.getPoints()[available.at(0)];
    available.erase(available.begin());

    LineSegment AC = LineSegment(A, opp);
    LineSegment BE = ((i % 2 == 0) ? AC.rotateClockwiseAroundA() : AC.rotateCounterClockwiseAroundA()) + (o - A);
    Point E = BE.getB();

    Line sideDE = Line(last, E);
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
    sideDE.draw(mat, 99);
    sideA.draw(mat, 99);
    sideB.draw(mat, 99);
    sideC.draw(mat, 99);

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
    Polygon poly = generateConvex(4);
    std::vector<Polygon> squares;
    std::vector<string> outfiles = {"Images/out1.ppm", "Images/out2.ppm", "Images/out3.ppm", "Images/out4.ppm", "Images/out5.ppm", "Images/out6.ppm"};
    for(int i = 0; i < 6; i++) {
        int(*matrix)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix
        Polygon _s = four_point_square(matrix, i, poly);
        squares.push_back(_s);
        _s.draw(matrix, 4);
        poly.draw(matrix, 3);
        writePPM(outfiles[i].c_str(), HEIGHT, WIDTH, matrix);
    }
    write_square_info(squares, TEXTOUTFILE);
    int(*mat)[WIDTH] = new int[HEIGHT][WIDTH]; // matrix
    perp_diagonal(mat);
    writePPM(OUTFILE, HEIGHT, WIDTH, mat);
}