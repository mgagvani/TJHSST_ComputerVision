#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cassert>
#include <vector>

#define HEIGHT 800
#define WIDTH 800 

using namespace std;

double generate_pos() {
    double a = (double)rand()/RAND_MAX;
    while(a < 0.0 || a > 1.0) {
        a = (double)rand()/RAND_MAX;
    }
    assert(abs(a) < 1);
    return a;
}

int generate_position(int min, int max) {
    double d = generate_pos();
    int a = min + (int)max*d;
    while(a >= max || a <= min) {
        d = generate_pos();
        a = min + (int)(max*d);
    }
    return a;
}

int scale_double(double val, int max) {
    return (int)(val*max);
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

std::vector<double> get_incenter(double x1, double x2, double x3, double y1, double y2, double y3) {
    // find a, b, c
    double a, b, c;
    a = dist(x1, y1, x2, y2);
    b = dist(x2, y2, x3, y3);
    c = dist(x3, y3, x1, y1);

    double sumabc = a + b + c;
    double t1 = b * x1 + c * x2 + a * x3;
    double t2 = b * y1 + c * y2 + a * y3;

    double xc = t1/sumabc;
    double yc = t2/sumabc;
    // find radius
    double s = sumabc / 2.0;
    double r = sqrt(((s - a) * (s - b) * (s - c))/s);

    // pack into vector and return
    std::vector<double> toRet;
    toRet.push_back(xc); toRet.push_back(yc); toRet.push_back(r);

    toRet.push_back(a); toRet.push_back(b); toRet.push_back(c); toRet.push_back(s);

    cout << "(" << xc << ", " << yc << ") r = " << r << endl;

    return toRet;
}

std::vector<double> get_circumcenter(double x1, double x2, double x3, double y1, double y2, double y3) {
    // find midpoints for two of them
    double midAx = (x2+x1)/2; double midAy = (y2+y1)/2;
    double midBx = (x2+x3)/2; double midBy = (y2+y3)/2;

    // find slopes
    double slopeA = (y2 - y1)/(x2 - x1); slopeA = -1.0/slopeA;
    double slopeB = (y3 - y2)/(x3 - x2); slopeB = -1.0/slopeB;

    double ya, yb;

    // equation is of form slope * x - slope * midx + midy
    bool found = false; double thresh = 0.001;
    double x = 0;
    for(x = 0; x <= 1.0; x += 1.0/(WIDTH*5)) {
        ya = slopeA * x - slopeA * midAx + midAy;
        yb = slopeB * x - slopeB * midBx + midBy;
        if(abs(ya - yb) < thresh) { 
            found = true;
            break;
        }
    }
    while(found == false) {
        thresh += 0.001;
        x = 0;
        for(x = 0; x <= 1.0; x += 1.0/(WIDTH*5)) {
            ya = slopeA * x - slopeA * midAx + midAy;
            yb = slopeB * x - slopeB * midBx + midBy;
            if(abs(ya - yb) < thresh) { 
                found = true;
                break;
            }
        }
    }

    double y = (ya + yb)/2.0;

    cout << "x = " << x << ", ya = " << ya << ", yb = " << yb << endl;

    std::vector<double> toRet;
    toRet.push_back(x); toRet.push_back(y); 
    toRet.push_back(midAx); toRet.push_back(midAy); 
    toRet.push_back(midBx); toRet.push_back(midBy);
    toRet.push_back(slopeA); toRet.push_back(slopeB);
    return toRet;

}

std::vector<double> get_nineptcircle(double x1, double x2, double x3, double y1, double y2, double y3) {
    // find midpoints
    double xa = (x1+x2)/2.0; double ya = (y1+y2)/2.0;
    double xb = (x2+x3)/2.0; double yb = (y2+y3)/2.0;
    double xc = (x1+x3)/2.0; double yc = (y1+y3)/2.0;

    return get_circumcenter(xa, xb, xc, ya, yb, yc);
}

std::vector<double> get_centroid(double x1, double x2, double x3, double y1, double y2, double y3) {
    double x = (x1 + x2 + x3)/3.0;
    double y = (y1 + y2 + y3)/3.0;
    std::vector<double> toRet;
    toRet.push_back(x);
    toRet.push_back(y);
    return toRet;
}

std::vector<double> get_eulerline(std::vector<double> incenter, std::vector<double> centroid) {
    double xi = incenter.at(0); double yi = incenter.at(1);
    double xc = centroid.at(0); double yc = centroid.at(1);

    double slope = (yc - yi)/(xc - xi);

    // calculate start/end values (given start x is 0 and start y is 1)
    double ystart, yend;
    ystart = -1.0 * slope * xi + yi;
    yend = slope - slope * xi + yi;
    cout << ystart << " --> " << yend << endl;

    std::vector<double> toRet;
    toRet.push_back(ystart); toRet.push_back(yend);
    return toRet;
}

void writePPM(const char* filename, int iheight, int jwidth, int mat[][WIDTH]) {
    // this is a 2D matrix, thus grayscale and with max value 1
    // use ofstream 
    ofstream MyFile;
    MyFile.open(filename, ios::out);

    MyFile << "P3 800 800 1" << endl ; // TODO figure out how to put in HEIGHT and WIDTH in this

    for(int i = 0; i < HEIGHT; i++)
		for(int j = 0; j < WIDTH; j++)
            if(mat[i][j] == 0)
                MyFile << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else if (mat[i][j] == 1)
                MyFile << "1 1 1" << endl ; // if 1 is found, then print 1 1 1 
            else if (mat[i][j] == 2) 
                MyFile << "1 0 0" << endl ;
            else if (mat[i][j] == 3) 
                MyFile << "0 1 0" << endl ;
            else if (mat[i][j] == 4) 
                MyFile << "0 0 1" << endl ;
            else 
                MyFile << "1 0 1" << endl ; // error color (for now)
    
    MyFile.close();
}

void set_pixel(int x, int y, int color, int mat[][WIDTH]) {
    if(x >= WIDTH) return;  // Out of bounds, don't do anything
    if(y >= HEIGHT) return; // Out of bounds, don't do anything
    if(x < 0) return;
    if(y < 0) return;

    /** COLOR GUIDE
     * 0 -> BLACK
     * 1 -> WHITE
     * 2 -> RED
     * 3 -> GREEN
     * 4 -> BLUE
    */

    mat[x][y] = color;
    assert (mat[x][y] == color);
}

void draw_circle(int radius, int i, int j, int mat[][WIDTH]) {
    int x, y, xmax, y2, y2_new, ty;
    // assignments for method
    int r = radius;

    // double dtheta = asin(1/(radius*1.414213562))/2.0;
    // double theta = 45.0 + dtheta;

    // double xmult = sin(theta);

    xmax = (int) (radius * 0.75); 
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
        set_pixel( y  + i, x + j  , 3, mat); // only add offsets here
        set_pixel( -y + i, x + j  , 3, mat);
        set_pixel( y +  i, -x + j , 3, mat);
        set_pixel( -y+  i, -x + j , 3, mat);
        set_pixel( x  + i, y + j  , 3, mat);
        set_pixel( -x + i, y + j  , 3, mat);
        set_pixel( x +  i, -y + j , 3, mat);
        set_pixel( -x+  i, -y + j , 3, mat);
        y2_new -= (2 * x) - 3;
    }
}

void bresenham(int x1, int y1, int x2, int y2, int mat[][WIDTH]) {
    // swap x and y beforehand
    
    

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
            set_pixel(i, j, 2, mat);
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

void bresenham(double x1, double y1, double x2, double y2, int mat[][WIDTH]) {
    int nx1, ny1, nx2, ny2;

    nx1 = scale_double(x1, WIDTH); nx2 = scale_double(x2, WIDTH);
    ny2 = scale_double(y2, HEIGHT); ny1 = scale_double(y1, HEIGHT);

    cout << "(" << nx1 << ", " << ny1 << ") (" << nx2 << ", " << ny2 << ")" << endl;

    bresenham(nx1, ny1, nx2, ny2, mat);
}

std::vector<double> draw_triangle(int mat[][WIDTH]) {
    double x1, x2, x3, y1, y2, y3;

    x1 = generate_pos();  y1 = generate_pos();
    x2 = generate_pos();  y2 = generate_pos();
    x3 = generate_pos();  y3 = generate_pos();

    cout << x1 << x2 << x3 << y1 << y2 << y3 << endl;

    // Point 1 to Point 2
    bresenham(x1, y1, x2, y2, mat);
    // Point 2 to Point 3
    bresenham(x2, y2, x3, y3, mat);
    // Point 3 to Point 1
    bresenham(x3, y3, x1, y1, mat);

    // return for other methods to use
    std::vector<double> toRet;
    toRet.push_back(x1); toRet.push_back(x2); toRet.push_back(x3);
    toRet.push_back(y1); toRet.push_back(y2); toRet.push_back(y3);
    return toRet;
}

void draw_circles(std::vector<double> triangle, int mat[][WIDTH]) {
    double x1, x2, x3, y1, y2, y3;
    x1 = triangle[0]; x2 = triangle[1]; x3 = triangle[2];
    y1 = triangle[3]; y2 = triangle[4]; y3 = triangle[5];
    std::vector<double> incircle = get_incenter(x1, x2, x3, y1, y2, y3);
    
    int xc = scale_double(incircle.at(0), WIDTH);
    int yc = scale_double(incircle.at(1), HEIGHT);
    int _scalefactor = (WIDTH + HEIGHT)/2;
    int r = scale_double(incircle.at(2), _scalefactor); 

    /** debug
    for(int a = xc-5; a < xc+6; a++) {
        for(int b = yc-5; b < yc+6; b++) {
            set_pixel(b, a, 3, mat);
        }
    } */

    draw_circle(r, yc, xc, mat);
    

    // circumcircle
    std::vector<double> circumcircle = get_circumcenter(x1, x2, x3, y1, y2, y3);

    int xcc = scale_double(circumcircle.at(0), WIDTH);
    int ycc = scale_double(circumcircle.at(1), HEIGHT);

    double _R = (incircle.at(3) * incircle.at(4) * incircle.at(5))/(4.0 * r/800.0 * incircle.at(6));
    int R = scale_double(_R, _scalefactor);

    draw_circle(R, ycc, xcc, mat);

    // 9-point circle
    std::vector<double> nineptcircle = get_nineptcircle(x1, x2, x3, y1, y2, y3);
    int x9c = scale_double(nineptcircle.at(0), WIDTH);
    int y9c = scale_double(nineptcircle.at(1), HEIGHT);
    int rad = R/2;

    draw_circle(rad, y9c, x9c, mat);    

    // Euler line
    std::vector<double> centroid = get_centroid(x1, x2, x3, y1, y2, y3);
    std::vector<double> eulerline = get_eulerline(incircle, centroid);
    int ystart = scale_double(eulerline.at(0), HEIGHT);
    int yend = scale_double(eulerline.at(1), HEIGHT);

    bresenham(0, ystart, WIDTH, yend, mat);

    // debug circumcirlce - draw lines
    //int xa = scale_double(circumcircle.at(2), WIDTH);
    //int ya = scale_double(circumcircle.at(3), HEIGHT);
    //int xb = scale_double(circumcircle.at(4), WIDTH);
    //int yb = scale_double(circumcircle.at(5), HEIGHT);
    //int yafar = circumcircle.at(6) * 790 - circumcircle.at(6) * xa + ya;
    //int ybfar = circumcircle.at(7) * 790 - circumcircle.at(7) * xb + yb;
    //bresenham(xa, ya, 780, yafar, mat);
    //bresenham(xb, yb, 780, ybfar, mat);
}

/** comment out main to use in project 2
int main() {
    // Set random time
    srand(time(nullptr));
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    // Tests
    // X1 Y1 X2 Y2
    // bresenham(100, 100, 400, 100, result); // HORIZONTAL
    // bresenham(100, 100, 100, 400, result); // VERTICAL
    // bresenham(100, 100, 400, 400, result); // POSITIVE SLOPE
    // bresenham(400, 100, 100, 400, result); // NEGATIVE SLOPE
    vector<double> points = draw_triangle(result);
    cout << "Finished drawing triangle" << endl;
    draw_circles(points, result);
    // for(int r = 2; r < 399; r += 10) {
    //     draw_circle(r, 400, 400, result);
    // }
    // cout << "Finished drawing incircle" << endl;
    writePPM("l01.ppm", HEIGHT, WIDTH, result);
    // cout << "Finished writing to PPM" << endl;

    return 0;
}
*/