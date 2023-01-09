#include "../Project4/l04lib.cpp"

using namespace std;

#define MAX_COLOR_F 255.0f

const std::vector<vector<int>> gaussian = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
const std::vector<vector<int>> sobely = {{1, 2, 1}, {0, 0, 0,}, {-1, -2, -1}};
const std::vector<vector<int>> sobelx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

struct SobelWrapper {
    RGBImage sobelxImg;
    RGBImage sobelyImg;
    float(*thetas)[RGB_W];
    SobelWrapper(RGBImage &sobelxImg, RGBImage &sobelyImg, float(*thetas)[RGB_W]) {
        this->sobelxImg = sobelxImg;
        this->sobelyImg = sobelyImg;
        this->thetas = thetas;
    }
};

SobelWrapper sobel(RGBImage &other) {
    RGBImage sobelxImg = other.conv2d(sobelx);
    RGBImage sobelyImg = other.conv2d(sobely);
    float thetas[RGB_H][RGB_W];
    for(int i = 0; i < RGB_H; i++) {
        for(int j = 0; j < RGB_W; j++) {
            thetas[i][j] = atan2(sobelyImg.getGrayscale(i, j), sobelxImg.getGrayscale(i, j));
        }
    }
    return SobelWrapper(sobelxImg, sobelyImg, thetas);
}

RGBImage houghCircles(RGBImage &edgeImg, SobelWrapper sobelInfo) {
    unsigned int(*votes)[RGB_W] = new unsigned int[RGB_H][RGB_W]; // matrix for votes
    for(size_t i = 0; i < RGB_H; i++) {
        for(size_t j = 0; j < RGB_W; j++) {
            votes[i][j] = 0;
        }
    }
    float r; int xv, yv;
    // iterate over every pixel and have it cast votes
    for(size_t i = 10; i < RGB_H - 10; i++) {
        for(size_t j = 10; j < RGB_W - 10; j++) {
            if(edgeImg.getGrayscale(i, j) == 0) continue; // skip if not an edge pixel
            r = 0;
            while(r < (RGB_H/2 + RGB_W/2)) { // forwards
                xv = j + r * cos(sobelInfo.thetas[i][j]); // we are being consistent bc everything is in radians
                yv = i + r * sin(sobelInfo.thetas[i][j]);
                // cout << "xv: " << xv << ", yv: " << yv << endl;
                if(xv < 0 || xv >= RGB_W || yv < 0 || yv >= RGB_H) break;
                votes[yv][xv]++;
                xv = j - r * cos(sobelInfo.thetas[i][j]); // this time we decrement instead of increment
                yv = i - r * sin(sobelInfo.thetas[i][j]);
                // cout << "xv: " << xv << ", yv: " << yv << endl;
                if(xv < 0 || xv >= RGB_W || yv < 0 || yv >= RGB_H) break;
                votes[yv][xv]++;
                r += 0.1;
            }
        }
    }
    // find the max vote
    unsigned int maxVote = 0;
    pair<int, int> maxVoteCoords;
    for(size_t i = 0; i < RGB_H; i++) {
        for(size_t j = 0; j < RGB_W; j++) {
            if(votes[i][j] > maxVote) {
                maxVote = votes[i][j]; 
                maxVoteCoords = make_pair(i, j);}
        }
    }
    cout << "max vote: " << maxVote << endl;
    float scaleFactor = MAX_COLOR_F / maxVote;
    cout << "scale factor: " << scaleFactor << endl;
    RGBImage houghImg = RGBImage();
    for(size_t i = 0; i < RGB_H; i++) {
        for(size_t j = 0; j < RGB_W; j++) {
            houghImg.setGrayscale(i, j, votes[i][j] * scaleFactor);
        }
    }
    // print max vote coords
    cout << "max vote coords: " << maxVoteCoords.first << ", " << maxVoteCoords.second << endl;
    return houghImg;
}   


int main() {
    cout << "starting" << endl;
    std::string filepath = "easy.ppm";

    static RGBImage img = RGBImage(filepath);
    img.toGrayscale(); img.reduceColors(4);
    cout << "created the image from file" << endl;
    static RGBImage img001 = img.conv2d(gaussian);
    static RGBImage img002 = img001.conv2d(gaussian);
    static RGBImage img003 = img002.conv2d(gaussian);
    static RGBImage img004 = img003.conv2d(gaussian);
    static RGBImage img005 = img004.conv2d(gaussian);
    static RGBImage img006 = img005.conv2d(gaussian);
    static RGBImage img007 = img006.conv2d(gaussian);
    static RGBImage img008 = img007.conv2d(gaussian);
    static RGBImage img009 = img008.conv2d(gaussian);
    static RGBImage img010 = img009.conv2d(gaussian);
    static RGBImage img011 = img010.conv2d(gaussian);
    static RGBImage img012 = img011.conv2d(gaussian);
    static RGBImage img013 = img012.conv2d(gaussian);
    static RGBImage img014 = img013.conv2d(gaussian);
    static RGBImage img015 = img014.conv2d(gaussian);
    static RGBImage img016 = img015.conv2d(gaussian);
    static RGBImage img017 = img016.conv2d(gaussian);
    static RGBImage img018 = img017.conv2d(gaussian);
    static RGBImage img019 = img018.conv2d(gaussian);
    static RGBImage img020 = img019.conv2d(gaussian);
    cout << "performed convolution blur " << endl;
    static RGBImage img2 = img020.conv2d(sobely);
    static RGBImage img3 = img020.conv2d(sobelx);
    cout << "performed convolution sobel " << endl;
    static RGBImage img4 = RGBImage(); img4.combineImgs(img2, img3);
    static RGBImage canny = img.canny(sobelx, sobely, img, 10, 80);
    canny.thinEdges(canny);
    canny.binaryThresh(100);
    static RGBImage edgeY = canny.conv2d(sobely);
    static RGBImage edgeX = canny.conv2d(sobelx);
    static RGBImage edge = RGBImage(); edge.combineImgs(edgeY, edgeX);
    edge.thinEdges(edge);
    static SobelWrapper sobelInfo = sobel(edge);
    static RGBImage houghImg = houghCircles(edge, sobelInfo);
    houghImg.write("l06.ppm");
    // edge.write("l06.ppm"); 
}