#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>

#define HEIGHT 480
#define WIDTH 640 

using namespace std;

double generate_pos() {
    return (double)rand()/RAND_MAX;
}

int generate_position(int min, int max) {
    double d = generate_pos();
    int a = min + (int)max*d;
    return a;
}

void writePPM(int iheight, int jwidth, int mat[][WIDTH]) {
    // this is a 2D matrix, thus grayscale and with max value 1
    // it has to be piped > to something.ppm

    cout << "P3 640 480 1" << endl ; // TODO figure out how to put in HEIGHT and WIDTH in this

    for(int i = 0; i < HEIGHT; i++)
		for(int j = 0; j < WIDTH; j++)
            if(mat[i][j] == 0)
                cout << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else if (mat[i][j] == 1)
                cout << "1 1 1" << endl ; // if 1 is found, then print 0 1 0
            else if (mat[i][j] == 2) 
                cout << "1 0 0" << endl ;
            else if (mat[i][j] == 3) 
                cout << "0 1 0" << endl ;
            else if (mat[i][j] == 4) 
                cout << "0 0 1" << endl ;
            else 
                cout << "1 0 1" << endl; // error color (for now)
}

void set_pixel(int x, int y, int color, int mat[][WIDTH]) {
    if(x >= WIDTH) return;  // Out of bounds, don't do anything
    if(y >= HEIGHT) return; // Out of bounds, don't do anything

    /** COLOR GUIDE
     * 0 -> BLACK
     * 1 -> WHITE
     * 2 -> RED
     * 3 -> GREEN
     * 4 -> BLUE
    */

    mat[y][x] = color;
    assert (mat[y][x] == color);
}

void draw_circle(int radius, int i, int j, int mat[][WIDTH]) {
    int x, y, xmax, y2, y2_new, ty;
    // assignments for method
    int r = radius;

    xmax = (int) (radius * 0.70710678); 
    y = r; 
    y2 = y * y;
    ty = (2 * y) - 1;
    y2_new = y2;
    for (x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
        y2 -= ty;
        y -= 1;
        ty -= 2;
        }
        set_pixel(x + j, y  + i, 3, mat); // only add offsets here
        set_pixel(x + j, -y + i, 3, mat);
        set_pixel(-x + j, y + i, 3, mat);
        set_pixel(-x + j, -y+ i, 3, mat);
        set_pixel(y + j, x  + i, 3, mat);
        set_pixel(y + j, -x + i, 3, mat);
        set_pixel(-y + j, x + i, 3, mat);
        set_pixel(-y + j, -x+ i, 3, mat);
        y2_new -= (2 * x) - 3;
    }
}

void bresenham(int x1, int y1, int x2, int y2, int mat[][WIDTH]) {

    int dx = x2 - x1;
    int dy = y2 - y1;

    bool driving_x = abs(dx) > abs(dy);

    if (driving_x) {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
            dx = -dx; // we swapped points, so these must be updated
            dy = -dy;
        }
        int j = y1;
        int error = dy - dx;
        for (int i = x1; i <= x2; i++) {
            set_pixel(j, i, 2, mat);
            // Error is greater than 0, so we know the line has moved up one pixel
            if (error > 0 && y2 > y1) {
                j++;
                error -= dx;
            } 
            else if (error < 0 && y2 < y1) {
                j--;
                error += dx;
            }
            error += dy;
        }
    } 
    else {
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
            dx = -dx; // we swapped points, so these must be updated
            dy = -dy;
        }
        int j = x1;
        int error = dx - dy;
        for (int i = y1; i <= y2; i++) {
            set_pixel(j, i, 2, mat);
            if (error > 0 && x2 > x1) {
                j++;
                error -= dy;
            } 
            else if (error < 0 && x2 < x1) {
                j--;
                error += dy;
            }
            error += dx;
        }
    }
    
}

void draw_triangle(int mat[][WIDTH]) {
    int x1, x2, x3, y1, y2, y3;

    x1 = generate_position(0,WIDTH);  y1 = generate_position(0, HEIGHT);
    x2 = generate_position(0,WIDTH);  y2 = generate_position(0, HEIGHT);
    x3 = generate_position(0,WIDTH);  y3 = generate_position(0, HEIGHT);

    // Point 1 to Point 2
    bresenham(x1, y1, x2, y2, mat);
    // Point 2 to Point 3
    bresenham(x2, y2, x3, y3, mat);
    // Point 3 to Point 1
    bresenham(x3, y3, x1, y1, mat);
}

int main() {
    // Set random time
    srand(time(nullptr));
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    // Tests
    // X1 Y1 X2 Y2
    // bresenham(1, 1, 1, 1, result);         // CASE 0a
    // bresenham(50, 90, 100, 90, result);    // CASE 0e
    // bresenham(100, 300, 50, 300, result);  // CASE 0d
    // bresenham(120, 100, 120, 400, result); // CASE 0b
    // bresenham(300, 80, 200, 80, result);   // CASE 0c
    // bresenham(300, 300, 450, 400, result); // CASE 1
    // bresenham(50, 50, 1, 1, result);       // CASE 1 
    // bresenham(10, 100, 50, 10, result);   // CASE 2
    draw_triangle(result);
    draw_circle(100, 100, 300, result);
    writePPM(HEIGHT, WIDTH, result);

    return 0;
}