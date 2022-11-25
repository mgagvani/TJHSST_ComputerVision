#include <l01.h>
#include <l02lib.h>

#include <fstream>
#include <iostream>

using namespace std;

#define RGB_H 3024
#define RGB_W 4032
#define RGB_D 3
#define MAX_COLOR 255

class RGBImage {
    private:
    public:
        int height, width;
        unsigned int img[RGB_H][RGB_W][RGB_D];

        RGBImage() {
            for(int i = 0; i < RGB_H; i++) {
                for(int j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        img[i][j][d] = 127;
                    }
                }
            }
        }
        RGBImage(string filepath) {
            cout << "got into the constructor" << endl;
            ifstream fin(filepath);
            cout << "created input file stream" << endl;
            string a; 
            int y, x, w;
            fin >> a >> y >> x >> w;
            cout << "got the metadata" << endl;
            cout << "a is: " << a << endl;
            cout << "y is: " << y << endl;
            cout << "x is: " << x << endl;
            cout << "w is: " << w << endl;

            for (int y = 0; y < RGB_H; y++) {
                for (int x = 0; x < RGB_W; x++)
                {
                    unsigned int red, green, blue;
                    fin >> red >> green >> blue;
                    if (!fin)
                    {
                        std::cerr << "Error reading from file around (" << y << "," << x << ")" << std::endl;
                        return;
                    }
                    // TODO: Check the values of red, green, blue against the interval [0,255]
                    img[y][x][0] = red;
                    img[y][x][1] = green;
                    img[y][x][2] = blue;
                }
            }
        }
        ~RGBImage() {}

        int write(string filepath) {
            ofstream MyFile;
            MyFile.open(filepath, ios::out);

            MyFile << "P3" << RGB_W << " " << RGB_H << " " << MAX_COLOR << endl ;

            for(int i = 0; i < RGB_H; i++) {
                if(i == 0) {
                    MyFile << endl;
                }
	        	for(int j = 0; j < RGB_W; j++) {
                    unsigned int r = img[i][j][0];
                    unsigned int g = img[i][j][1];
                    unsigned int b = img[i][j][2];
                    MyFile << r << " " << g << " " << b << " ";
                }
            }
            MyFile.close();
            return 0;
        }
};



int main() {
    cout << "at least the program started" << endl;
    string filepath = "image01.ppm";
    static RGBImage img = RGBImage(filepath);
    cout << "created the image from file" << endl;
    string filepathnew = "l04.ppm";
    int _retval = img.write(filepathnew);
    cout << "wrote the image to file" << endl;

    return 0;
}