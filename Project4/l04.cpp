#include "l04lib.cpp"

#include <fstream>
#include <iostream>
#include <numeric>
#include <cmath>

using namespace std;

int main() {
    std::string filepath = "image02.ppm";

    static RGBImage img = RGBImage(filepath);
    static RGBImage img2 = RGBImage(filepath);
    img2.dim(0.4f);
    img.toGrayscale();
    // img.write("l04gray.ppm");
    img.reduceColors(2);
    // img.write("l04reducecolor.ppm");
    for(int i = 0; i < RGB_H; i++) {
        for(int j = 0; j < RGB_W; j++) {
            if(img.getGrayscale(i, j) > MAX_COLOR) {
                cout << i << ", " << j << " = " << img.getGrayscale(i, j) << endl;
            }
        }
    }
    cout << "created the image from file and converted to grayscale" << endl;

    std::vector<vector<int>> gaussian = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    std::vector<vector<int>> sobely = {{1, 2, 1}, {0, 0, 0,}, {-1, -2, -1}};
    std::vector<vector<int>> sobelx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};


    static RGBImage _img = img.conv2d(gaussian);
    static RGBImage _img2 = _img.conv2d(gaussian);
    static RGBImage _img3 = _img2.conv2d(gaussian);
    static RGBImage _img4 = _img3.conv2d(gaussian);
    static RGBImage _img5 = _img4.conv2d(gaussian);
    static RGBImage _img6 = _img5.conv2d(gaussian);

    static RGBImage out = img.conv2d(sobely);
    static RGBImage out2 = img.conv2d(sobelx);
    static RGBImage asdf = RGBImage(); asdf.combineImgs(out, out2); 
    asdf.binaryThresh(100);
    asdf.binaryThresh(250);
    static RGBImage foo = RGBImage(); foo.combineImgs(img2, asdf);
    static RGBImage canny = img.canny(sobelx, sobely, _img6, 10, 80);
    canny.thinEdges(canny);
    canny.write("l04.ppm");
    cout << "performed convolution" << endl;
    
    cout << "wrote the image to file" << endl;

    return 0;
}