#include <l01.h>
#include <l02lib.h>

std::vector<Point> generate_random_points(int num_points) {
    std::vector<Point> points;
    for(int i = 0; i < num_points; i++) {
        points.push_back(Point(generate_pos(), generate_pos()));
    }
    return points;
}

std::vector<Point> bruteforce_closest_points(std::vector<Point> points) {
    double currmin = 2; // max possible would be sqrt(2) which is 1.414
    Point p1; Point p2;
    for(int i = 0; i < points.size(); i++) {
        for(int j = 0; j < points.size(); j++) {
            if(i == j) {
                continue;
            }
            if(points[i].distance(points[j]) < currmin) {
                currmin = points[i].distance(points[j]);
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    std::vector<Point> to_ret;
    to_ret.push_back(p1); to_ret.push_back(p2);
    return to_ret;
}

int main() {
    srand(time(nullptr));
    int(*matrix)[WIDTH] = new int[HEIGHT][WIDTH];

    std::vector<Point> points = generate_random_points(60);
    for(Point p: points) {
        p.draw(matrix, 99);
    }
    std::vector<Point> closest_points = bruteforce_closest_points(points);
    LineSegment seg = LineSegment(closest_points[0], closest_points[1]);
    seg.draw(matrix, 3);
    seg.getA().draw(matrix, 1);
    seg.getB().draw(matrix, 1);


    writePPM("l03.ppm", HEIGHT, WIDTH, matrix);

    return 0;
}

