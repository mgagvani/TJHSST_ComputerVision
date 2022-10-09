#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <vector>

#define HEIGHT 800
#define WIDTH 800

double generate_pos();

int generate_position(int min, int max);

int scale_double(double val, int max);

double dist(double x1, double y1, double x2, double y2);

std::vector<double> get_incenter(double x1, double x2, double x3, double y1, double y2, double y3);

std::vector<double> get_circumcenter(double x1, double x2, double x3, double y1, double y2, double y3);

std::vector<double> get_nineptcircle(double x1, double x2, double x3, double y1, double y2, double y3);

std::vector<double> get_centroid(double x1, double x2, double x3, double y1, double y2, double y3);

std::vector<double> get_eulerline(std::vector<double> incenter, std::vector<double> centroid);

void writePPM(const char* filename, int iheight, int jwidth, int mat[][WIDTH]);

void set_pixel(int x, int y, int color, int mat[][WIDTH]);

void draw_circle(int radius, int i, int j, int mat[][WIDTH], int color);

void draw_point(double y, double x, int size, int mat[][WIDTH], int color);

void bresenham(int x1, int y1, int x2, int y2, int mat[][WIDTH], int color);

void bresenham(double x1, double y1, double x2, double y2, int mat[][WIDTH], int color);

std::vector<double> draw_triangle(int mat[][WIDTH]);

void draw_circles(std::vector<double> triangle, int mat[][WIDTH]);

