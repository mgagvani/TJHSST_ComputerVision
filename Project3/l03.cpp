#include <l01.h>
#include <l02lib.h>

#include <algorithm>
#include <iomanip>

class PointPair
{ // stores two points and the distance between them
private:
    Point a, b;
    double distance;
    bool initialized = false;

public:
    PointPair() {}

    PointPair(Point a, Point b) : a(a), b(b)
    {
        distance = a.distance(b);
        initialized = true;
    }

    PointPair(std::vector<Point> points) {
        a = points[0];
        b = points[1];
        distance = a.distance(b);
        initialized = true;
    }

    double getDistance() const
    {
        return distance;
    }

    Point getA() const { return a; }
    Point getB() const { return b; }

    bool minify(const PointPair &other)
    {
        if (other.initialized && (!initialized || other.getDistance() < distance))
        {
            a = other.a;
            b = other.b;
            distance = other.distance;
            initialized = true;
            return true;
        }

        return false;
    }

    void minify(PointPair *other)
    {
        if (other != nullptr)
        {
            this->minify(*other);
        }
    }

    bool isInitialized() const
    {
        return this->initialized;
    }

    std::vector<Point> to_vector() 
    {
        std::vector<Point> to_ret;
        to_ret.push_back(a); to_ret.push_back(b);
        return to_ret;
    }
};

std::vector<Point> generate_random_points(int num_points)
{
    std::vector<Point> points;
    for (int i = 0; i < num_points; i++)
    {
        points.push_back(Point(generate_pos(), generate_pos()));
    }
    return points;
}

std::vector<Point> bruteforce_closest_points(std::vector<Point> points)
{
    double currmin = 2; // max possible would be sqrt(2) which is 1.414
    Point p1;
    Point p2;
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            if (points[i].distance(points[j]) < currmin)
            {
                currmin = points[i].distance(points[j]);
                p1 = points[i];
                p2 = points[j];
            }
        }
    }
    std::vector<Point> to_ret;
    to_ret.push_back(p1);
    to_ret.push_back(p2);
    return to_ret;
}

bool comparePointXValues(Point first, Point second) {
	return first.getX() < second.getX();
}

bool comparePointYValues(Point first, Point second) {
	return first.getY() < second.getY();
}

PointPair helper2(std::vector<Point> points, int begin, int end)
{
    int numPoints = (end - begin);
    int mid = (begin + end) >> 1;
    if (numPoints == 2)
    {
        return PointPair(points[begin], points[end - 1]);
    }
    else if (numPoints == 3)
    {
        PointPair closest;
        closest.minify(PointPair(points[begin + 0], points[begin + 1]));
        closest.minify(PointPair(points[begin + 1], points[begin + 2]));
        closest.minify(PointPair(points[begin + 2], points[begin + 0]));

        return closest;
    }
    else
    {
        PointPair closest;
        // Right side
        closest.minify(helper2(points, begin, mid));
        // Left side
        closest.minify(helper2(points, mid, end));
        // Closest pair where both are in left or both are in right
        double d = closest.getDistance();

        // Maybe one of the points is on the left side and one is on the right?
        // Make a 'strip' of points down the middle
        int stripLeft = mid, stripRight = mid;
        double middleX = points[mid].getX();

        // [begin, mid) left, [mid, end] right
        while ((stripLeft > begin) && (points[stripLeft - 1].getX() >= middleX - d))
        {
            stripLeft--;
        }

        while ((stripRight < end) && (points[stripRight].getX() <= middleX + d))
        {
            stripRight++;
        }

        // compare points on both sides until the distance is small enough
        for (int i = stripLeft; i < mid; i++)
        {
            double maxX = points[i].getX() + d;
            for (int j = mid; j < stripRight && points[j].getX() < maxX; j++)
            {
                closest.minify({points[i], points[j]});
            }
        }

        return closest;
    }
}

PointPair recursive_closest_pointpair(std::vector<Point> points) {
  // sort  points by X value
  std::sort(points.begin(), points.end(), comparePointXValues);

  // Now, divide-and-conquer
  return helper2(points, 0, points.size());
}

std::vector<Point> recursive_closest_points(std::vector<Point> points) {
    return recursive_closest_pointpair(points).to_vector();
}

void savePoints(std::vector<Point> points, const char* filename) {
	std::ofstream out(filename);
	
    out << std::fixed << std::setprecision(20); // this increases the precision to 20 decimal places
	for (auto it = points.begin(); it != points.end(); it++) {
		out << it->getX() << "  " << it->getY() << "\n";
	}

	out.close();
}

std::vector<Point> readPoints(const char* filename) {
	std::ifstream in(filename);
	std::vector<Point> points;
	double x, y;
	while (in >> x) {
		in >> y;
		points.push_back(Point(x, y));
	}
	return points;
}


int main()
{
    srand(time(nullptr));
    int(*matrix)[WIDTH] = new int[HEIGHT][WIDTH];

    std::vector<Point> points = generate_random_points(250000);

    // save to file
    // savePoints(points, "point250k.txt");
    std::vector<Point> points = readPoints("point50k.txt");

    // for (Point p : points)
    // {
    //     p.draw(matrix, 99);
    // }
    std::vector<Point> closest_points = recursive_closest_points(points);
    LineSegment seg = LineSegment(closest_points[0], closest_points[1]);
    seg.draw(matrix, 3);
    seg.getA().draw(matrix, 1);
    seg.getB().draw(matrix, 1);

    writePPM("l03.ppm", HEIGHT, WIDTH, matrix);

    return 0;
}
