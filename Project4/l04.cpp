#include <l01.h>
#include <l02lib.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <cmath>


using namespace std;

#define RGB_H 1512
#define RGB_W 2016
#define RGB_D 3
#define MAX_COLOR 255

#define R_COEFF 0.2989
#define G_COEFF 0.5870
#define B_COEFF 0.1140

#define PI 3.141592653

class RGBImage { // size_t is cool
    private:
    public:
        int height, width;
        unsigned int img[RGB_H][RGB_W][RGB_D];

        RGBImage() {
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        img[i][j][d] = 127;
                    }
                }
            }
        }
        RGBImage(string filepath) {
            // cout << "got into the constructor" << endl;
            ifstream fin(filepath);
            // cout << "created input file stream" << endl;
            string a; 
            int y, x, w;
            fin >> a >> y >> x >> w;
            cout << "got the metadata" << endl;
            cout << "a is: " << a << endl;
            cout << "y is: " << y << endl;
            cout << "x is: " << x << endl;
            cout << "w is: " << w << endl;

            for (size_t y = 0; y < RGB_H; y++) {
                for (size_t x = 0; x < RGB_W; x++)
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
        // RGBImage(const RGBImage &other[RGB_H]) {
        //     img = *other.img;
        // }

        void write(string filepath) {
            ofstream MyFile;
            MyFile.open(filepath, ios::out);

            MyFile << "P3" << RGB_W << " " << RGB_H << " " << MAX_COLOR << endl ;

            for(size_t i = 0; i < RGB_H; i++) {
                if(i == 0) {
                    MyFile << endl;
                }
	        	for(size_t j = 0; j < RGB_W; j++) {
                    unsigned int r = img[i][j][0];
                    unsigned int g = img[i][j][1];
                    unsigned int b = img[i][j][2];
                    MyFile << r << " " << g << " " << b << " ";
                }
            }
            MyFile.close();
        }

        // reduces the number of colors in each channel by quantizing
        void reduceColors(short colFactor) {
            int numColors = MAX_COLOR / colFactor;
            if(numColors > MAX_COLOR) {
                return;
            }
            int temp;
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        temp = img[i][j][d] / numColors;
                        temp *= numColors;
                        img[i][j][d] = temp;
                    }
                }
            }
        }

        // makes the image dimmer
        void dim(float dimFactor) {
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        img[i][j][d] *= dimFactor;
                    }
                }
            }
        }

        // converts self to a grayscale image
        void toGrayscale() {
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    unsigned int r = img[i][j][0];
                    unsigned int g = img[i][j][1];
                    unsigned int b = img[i][j][2];
                    
                    // unsigned int result = (r + g + b)/3;
                    unsigned int result = r * R_COEFF + g * G_COEFF + b * B_COEFF;
                    img[i][j][0] = result;
                    img[i][j][1] = result;
                    img[i][j][2] = result;
                }
            }
        }

        void binaryThresh(int threshValue) {
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        if(img[i][j][d] > threshValue) {
                            img[i][j][d] = MAX_COLOR;
                        }
                        else {
                            img[i][j][d] = 0;
                        }
                    }
                }
            }
        }

        // returns index 0 and also zero pads the result
        int getGrayscale(int i, int j) {
            if(i < 0 || j < 0 || i > RGB_H || i > RGB_W) {
                return 0; // zero padding
            }
            return img[i][j][0];
        }
        void setGrayscale(int i, int j, unsigned int val) {
            img[i][j][0] = val;
            img[i][j][1] = val;
            img[i][j][2] = val;
        }
        RGBImage conv2d(std::vector<std::vector<int>> kernel) {
            RGBImage out = RGBImage();
            int kernelSum = 0;
            for(size_t a = 0; a < kernel.size(); a++) {
                kernelSum += accumulate(kernel[a].begin(), kernel[a].end(), 0);
            }
            if(kernelSum == 0) {
                kernelSum = 1;
            }
            int total;
            for (size_t y = 1; y < RGB_H - 1; y++) {
			    for (size_t x = 1; x < RGB_W - 1; x++) {
			    	total = 0;
			    	for (short relativeX = -1; relativeX <= 1; relativeX++) { // NOTE hardcoded 3x3 convolution kernel size
			    		for (short relativeY = -1; relativeY <= 1; relativeY++) {
			    			total += getGrayscale(y + relativeY, x + relativeX) * kernel[relativeY + 1][relativeX + 1];
                            // if(y + relativeY > 1500 || y + relativeY < 10 && x + relativeX < 10 || x + relativeX > 2000 && img[y + relativeY][x + relativeX][0] - getGrayscale(y + relativeY, x + relativeX) == 0) {
                            //     cout << "@ coords " << y << ", " << x << " accessing " << y + relativeY << ", " << x + relativeX << " which is " << img[y + relativeY][x + relativeX][0] << " OR " << getGrayscale(y + relativeY, x + relativeX) << endl;
                            // }
                            // cout << "@ coords " << x << ", " << y << "val is " << img[y + relativeY][x + relativeX][0] << endl;
                            /*cout << "@ coords " << y << ", " << x << "accessing "*/ // cout << y + relativeY << ", " << x + relativeX << endl;
			    		}
			    	}
                // cout << total << ", " << kernelSum << endl;
                out.img[y][x][0] = total/kernelSum;
                out.img[y][x][1] = total/kernelSum;
                out.img[y][x][2] = total/kernelSum;
				// setGrayscale(y, x, total/kernelSum);
			    }
		    }
            return out;
        }

        RGBImage canny(std::vector<std::vector<int>> sobelx, std::vector<std::vector<int>> sobely, RGBImage &other, int low, int high) {
            // currently only uses theta to calculate gradient
            
            // first, convolve with sobelx and sobely
            static RGBImage sobelxImg = other.conv2d(sobelx);
            static RGBImage sobelyImg = other.conv2d(sobely);
            static RGBImage out = RGBImage();
            float(*thetas)[RGB_W] = new float[RGB_H][RGB_W]; // matrix
            // put together, calculate theta
            for(size_t i = 0; i < RGB_H; i++) {
                for(size_t j = 0; j < RGB_W; j++) {
                    int gx = sobelxImg.getGrayscale(i, j);
                    int gy = sobelyImg.getGrayscale(i, j);
                    int mag = sqrt(gx*gx + gy*gy);
                    float theta = atan2f(gy, gx);
                    out.setGrayscale(i, j, mag);
                    thetas[i][j] = theta;
                    // if(theta != 0.0f) {
                    //     cout << "theta is " << theta << " at " << i << ", " << j << endl;
                    // }
                }
            }
            int G;
            int otherAvgG;
            // loop over again (yeah, inefficient) to delete pixels not on the edge (gradient thresholding)
            for(size_t i = 1; i < RGB_H - 1; i++) {
                for(size_t j = 1; j < RGB_W - 1; j++) {
                    float theta = thetas[i][j];
                    // if(theta + PI/2 > PI) {
                    //     theta -= PI/2;
                    // }
                    // else {
                    //     theta += PI/2;
                    // }
                    float theta2 = theta;
                    if(theta < 0) {
                        theta2 = theta + 2 * PI;
                    }
                    // cout << "theta is " << theta << " at " << i << ", " << j << endl;
                    G = out.getGrayscale(i, j);
                    if((theta > -PI/8 && theta < PI/8) || (theta > 7*PI/8 && theta2 < 9*PI/8)) { // right/left
                        otherAvgG = (out.getGrayscale(i, j-1) + out.getGrayscale(i, j+1))/2;
                        if(G < otherAvgG) {
                            out.setGrayscale(i, j, 0);
                        }
                    }
                    else if((PI/8 < theta && theta < 3*PI/8) || (-7*PI/8 < theta && theta < 5*PI/8)) { // diagonals, upright, downleft
                        otherAvgG = (out.getGrayscale(i-1, j+1) + out.getGrayscale(i+1, j-1))/2;
                        if(G < otherAvgG) {
                            out.setGrayscale(i, j, 0);
                        }
                    }
                    else if((5*PI/8 < theta && theta < 7*PI/8) || (-3*PI/8 < theta && theta < -PI/8)) { // diagonals, upleft, downright
                        otherAvgG = (out.getGrayscale(i-1, j-1) + out.getGrayscale(i+1, j+1))/2;
                        if(G < otherAvgG) {
                            out.setGrayscale(i, j, 0);
                        }
                    }
                    else if((3*PI/8 < theta && theta < 5*PI/8) || (-5*PI/8 < theta && theta < -3*PI/8)) { // up/down
                        otherAvgG = (out.getGrayscale(i-1, j) + out.getGrayscale(i+1, j))/2;
                        if(G < otherAvgG) {
                            out.setGrayscale(i, j, 0);
                        }
                    }
                    // else {
                    //     cout << "error at: " << i << ", " << j << ", theta is: " << theta << endl;
                    // }
                }
            }
            // loop over again do a simple double threshold with high/low
            for(size_t i = 1; i < RGB_H - 1; i++) {
                for(size_t j = 1; j < RGB_W - 1; j++) {
                    if(getGrayscale(i, j) < low) { // not an edge pixel
                        out.setGrayscale(i, j, 0);
                        // out.img[i][j][2] = 255;
                    }
                    else if(getGrayscale(i, j) > high) { // strong edge pixel
                        // out.setGrayscale(i, j, 255);
                        // out.img[i][j][1] = 255;
                    }     
                    else { // weak edge pixel
                        // out.img[i][j][0] = 255;
                        out.setGrayscale(i, j, 0);
                    }          
                }
            }
            return out;
        }

        void combineImgs(const RGBImage &img1, const RGBImage &img2) {
            for(int i = 0; i < RGB_H; i++) {
                for(int j = 0; j < RGB_W; j++) {
                    for(short d = 0; d < RGB_D; d++) {
                        // img[i][j][d] = (unsigned int)sqrtf(pow(img1.img[i][j][d], 2) + pow(img2.img[i][j][d], 2));
                        img[i][j][d] = (unsigned int)(img1.img[i][j][d] + img2.img[i][j][d]);
                        // cout << img[i][j][d];
                    }
                }
            }
        }
};



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
    // static RGBImage foo = RGBImage(); foo.combineImgs(img2, asdf);
    static RGBImage canny = img.canny(sobelx, sobely, _img6, 105, 120);
    canny.write("l04.ppm");
    cout << "performed convolution" << endl;
    
    cout << "wrote the image to file" << endl;

    return 0;
}