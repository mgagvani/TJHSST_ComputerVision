/**
 * The goal of this program is to test Bresenham's algorithm and PPM file writing.
 * It is one-half of the first lab - this code assumes integer points are given and puts them in an image.
 * The other half of this is the incircle and Euler line, etc. which is not in this code.
**/
#include <iostream>

#define HEIGHT 48
#define WIDTH 64 

using namespace std;

void writePPM(int iheight, int jwidth, int mat[][WIDTH]) {
    // this is a 2D matrix, thus grayscale and with max value 1
    // it has to be piped > to something.ppm

    cout << "P3 64 48 1" << endl ; // TODO figure out how to put in HEIGHT and WIDTH in this

    for(int i = 0; i < HEIGHT; i++)
		for(int j = 0; j < WIDTH; j++)
            if(mat[i][j] == 0)
                cout << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else
                cout << "1 1 1" << endl ; // if 1 is found, then print 0 1 0
}

void bresenham(int x1, int y1, int x2, int y2, int mat[][WIDTH]) {
    int dx, dy, j, e, i; // delta x, delta y, starting y, error, starting x
    dx = x2 - x1;
    dy = y2 - y1;
    j = y1;
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
    
    // CASE 1: dx > 0, dy > 0, and dx > dy
    for(i = x1; i < x2; i++) {
        mat[i][j] = 1;
        if(e >= 0) {
            j++;
            e -= dx;
        }
        e += dy;
    }
}

int main() {
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    bresenham(16, 1, 10, 1, result);
    writePPM(HEIGHT, WIDTH, result);

    return 0;
}