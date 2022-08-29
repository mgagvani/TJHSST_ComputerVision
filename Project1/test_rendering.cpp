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

    for(i = x1; i < x2; i++) {
        mat[i][j] = 1;
        if(e >= 0) {
            j++;
            e -= dx;
        }
        i++; // Will this work with the FOR LOOP or not
        e += dy;
    }
}

int main() {
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    bresenham(5, 5, 20, 20, result);
    writePPM(HEIGHT, WIDTH, result);

    return 0;
}