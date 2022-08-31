/**
 * The goal of this program is to test Bresenham's algorithm and PPM file writing.
 * It is one-half of the first lab - this code assumes integer points are given and puts them in an image.
 * The other half of this is the incircle and Euler line, etc. which is not in this code.
**/
#include <iostream>
#include <cassert>  


#define HEIGHT 480
#define WIDTH 640 

using namespace std;

void writePPM(int iheight, int jwidth, int mat[][WIDTH]) {
    // this is a 2D matrix, thus grayscale and with max value 1
    // it has to be piped > to something.ppm

    cout << "P3 640 480 1" << endl ; // TODO figure out how to put in HEIGHT and WIDTH in this

    for(int i = 0; i < HEIGHT; i++)
		for(int j = 0; j < WIDTH; j++)
            if(mat[i][j] == 0)
                cout << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else
                cout << "1 1 1" << endl ; // if 1 is found, then print 0 1 0
}

void bresenham(int x1, int y1, int x2, int y2, int mat[][WIDTH]) {
    int dx, dy, j, e, i; // delta x, delta y, starting y, error, starting x

    // reorder the x's and y's so they are always increasing
    if(x1 > x2) {
        int _t1 = x1;
        x1 = x2;
        x2 = _t1;
    } assert (x2 >= x1);
    if(y1 > y2) {
        int _t2 = y1;
        y1 = y2;
        y2 = _t2;
    } assert (y2 >= y1);

    dx = x2 - x1; // these should always be positive
    dy = y2 - y1; // these should always be positive
    e = dy - dx;

    // CASE 0a: same point
    if(x2 == x1 && y2 == y1) {
        mat[x1][y1] = 1; 
        return;
    }
    // CASE 0b: same horizontal value but y2 > y1
    else if(x1 == x2 && y2 > y1) {
        for(int a = y1; a < y2; a++) {
            mat[x1][a] = 1;
        }
        return;
    }
    // CASE 0c: same horizontal value but y1 > y2
    else if(x1 == x2 && y1 > y2) {
        for(int a = y2; a < y1; a++) {
            mat[x1][a] = 1;
        }
        return;
    }
    // CASE 0d: x1 > x2, vertical values same
    else if(y1 == y2 && x1 > x2) {
        for(int a = x2; a < x1; a++) {
            mat[a][y1] = 1;
        }
        return;
    }
    // CASE 0e: x2 > x1, vertical values same
    else if(y1 == y2 && x2 > x1) {
        for(int a = x1; a < x2; a++) {
            mat[a][y1] = 1;
        }
        return;
    }
    // CASE 1: dx > 0, dy > 0, and dx > dy (Driving axis X)
    if(dx > 0 && dy > 0 && dx > dy) {
        j = y1;
        for(i = x1; i < x2; i++) {
            mat[i][j] = 1;
            if(e >= 0) {
                j++;
                e -= dx;
            }
            e += dy;
        }
    }
    // CASE 2: dx > 0, dy > 0, and dy > dx (Driving axis Y)
    else if(dx > 0 && dy > 0 && dy > dx) {
        i = x1;
        for(j = y1; i < y2; j++) {
            mat[i][j] = 1;
            if(e >= 0) {
                i++;
                e -= dy;
            }
            e += dx;
        }
    }
}

int main() {
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    // Tests
    // X1 Y1 X2 Y2
    bresenham(1, 1, 1, 1, result);         // CASE 0a
    bresenham(50, 90, 100, 90, result);    // CASE 0e
    bresenham(100, 300, 50, 300, result);  // CASE 0d
    bresenham(120, 100, 120, 400, result); // CASE 0b
    bresenham(300, 80, 200, 80, result);   // CASE 0c
    bresenham(300, 300, 450, 400, result); // CASE 1
    bresenham(222, 222, 300, 410, result); // CASE 2
    writePPM(HEIGHT, WIDTH, result);

    return 0;
}