#include <l01.h>
#include <l02lib.h>

#include <algorithm>
#include <iomanip>
#include <unordered_map>

// 64-bit unsigned integer
typedef unsigned long long bigint;
// So that we can overload and add the hash function
typedef std::pair<bigint, bigint> GridSquare;

// from stackoverlfow
struct hash_pair {
	template <class T1, class T2>
	bigint operator()(const std::pair<T1, T2>& p) const {
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ hash2;
	}
};

typedef std::unordered_map<GridSquare, Point, hash_pair> GridMap;

GridSquare makeGridSquare(const Point& point, double delta) {
	return GridSquare(
		(bigint)(point.getX() / (delta / 2)),
		(bigint)(point.getY() / (delta / 2))
	);
}

class PointPair
{ // stores two points and the distance between them
private:
    Point a, b;
    double distance;
    bool initialized = false;

public:
    PointPair() {}

    PointPair(Point a, Point b) : a(a), b(b) {
        distance = a.distance(b);
        initialized = true;
    }

    PointPair(std::vector<Point> points) {
        a = points[0];
        b = points[1];
        distance = a.distance(b);
        initialized = true;
    }

    double getDistance() const {
        return distance;
    }

    Point getA() const { return a; }
    Point getB() const { return b; }

    bool minify(const PointPair &other) {
        if (other.initialized && (!initialized || other.getDistance() < distance)) {
            a = other.a;
            b = other.b;
            distance = other.distance;
            initialized = true;
            return true;
        }
        return false;
    }

    void minify(PointPair *other) {
        if (other != nullptr) {
            this->minify(*other);
        }
    }

    bool isInitialized() const {
        return this->initialized;
    }

    std::vector<Point> to_vector() {
        std::vector<Point> to_ret;
        to_ret.push_back(a); to_ret.push_back(b);
        return to_ret;
    }
};

std::vector<Point> generate_random_points(int num_points) {
    std::vector<Point> points;
    for (int i = 0; i < num_points; i++) {
        points.push_back(Point(generate_pos(), generate_pos()));
    }
    return points;
}

std::vector<Point> bruteforce_closest_points(std::vector<Point> points) {
    double currmin = 2; // max possible would be sqrt(2) which is 1.414
    Point p1;
    Point p2;
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            if (i == j) {
                continue;
            }
            if (points[i].distance(points[j]) < currmin) {
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

PointPair recursive_helper(std::vector<Point> points, int begin, int end) {
    int numPoints = (end - begin);
    int mid = (begin + end) >> 1;
    if (numPoints == 2) {
        return PointPair(points[begin], points[end - 1]);
    }
    else if (numPoints == 3) {
        PointPair closest;
        closest.minify(PointPair(points[begin + 0], points[begin + 1]));
        closest.minify(PointPair(points[begin + 1], points[begin + 2]));
        closest.minify(PointPair(points[begin + 2], points[begin + 0]));

        return closest;
    }
    else {
        PointPair closest;
        // Right side
        closest.minify(recursive_helper(points, begin, mid));
        // Left side
        closest.minify(recursive_helper(points, mid, end));
        // Closest pair where both are in left or both are in right
        double d = closest.getDistance();

        // Maybe one of the points is on the left side and one is on the right?
        // Make a 'strip' of points down the middle
        int stripLeft = mid, stripRight = mid;
        double middleX = points[mid].getX();

        // [begin, mid) left, [mid, end] right
        while ((stripLeft > begin) && (points[stripLeft - 1].getX() >= middleX - d)) {
            stripLeft--;
        }

        while ((stripRight < end) && (points[stripRight].getX() <= middleX + d)) {
            stripRight++;
        }

        // compare points on both sides until the distance is small enough
        for (int i = stripLeft; i < mid; i++) {
            double maxX = points[i].getX() + d;
            for (int j = mid; j < stripRight && points[j].getX() < maxX; j++) {
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
  return recursive_helper(points, 0, points.size());
}

std::vector<Point> recursive_closest_points(std::vector<Point> points) {
    return recursive_closest_pointpair(points).to_vector();
}

void knuth_shuffle(std::vector<Point> points) {
	for (int currentIndex = points.size() - 1; currentIndex >= 0; currentIndex--) {
		// each time, choose a point to move to the back of the vector
		int swapIndex = (int)(generate_pos() * currentIndex);

		// swap index i and the random index
		Point currentPoint = points[currentIndex];
		Point swapPoint = points[swapIndex];

		points[currentIndex] = swapPoint;
		points[swapIndex] = currentPoint;
	}
}

PointPair get_closest_point(const GridMap& gridmap, const Point& point, double delta) {
	GridSquare center = makeGridSquare(point, delta);
	bigint startX = (center.first > 2) * (center.first - 2);
	bigint startY = (center.second > 2) * (center.second - 2);
	PointPair pair;

	for (bigint x = startX; x < startX + 5; x++) {
		for (bigint y = startY; y < startY + 5; y++) {
			GridSquare square(x, y);
			if (gridmap.count(square)) {
				pair.minify(PointPair(gridmap.at(square), point));
			}
		}
	}
	
	return pair;
}

PointPair grid_closest_pointpair(std::vector<Point> points) {
	knuth_shuffle(points);

	GridMap grid;
	PointPair closest(points[0], points[1]);
	
	for (unsigned long int index = 0; index < points.size(); index++) {
		PointPair closestPoint = get_closest_point(grid, points[index], closest.getDistance());
		if (closest.minify(closestPoint)) {
			grid.clear();

			for (unsigned long int i = 0; i < index; i++) {
				grid[makeGridSquare(points[i], closest.getDistance())] = points[i];
			}
		}
		grid[makeGridSquare(points[index], closest.getDistance())] = points[index];
	}
	return closest;
}

std::vector<Point> grid_closest_points(std::vector<Point> points) {
    return grid_closest_pointpair(points).to_vector();
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

    // std::vector<Point> points = generate_random_points(1000000);

    // save to file
    // savePoints(points, "point1m.txt");
    std::vector<Point> points = readPoints("point750k.txt");

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
