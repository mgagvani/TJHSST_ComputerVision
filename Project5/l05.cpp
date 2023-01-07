#include <l01.h>
#include <l02lib.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <cmath>

#include <algorithm>
#include <vector>

typedef std::pair<double,double> point;

using namespace std;

point origin;

long long crossproduct(const point& a, const point& b) {
  return a.first * b.second - a.second * b.first;
}

// Is the angle between the vectors ab and bc counter clockwise?
bool convex (const point& a, const point& b, const point& c) {
  const point ab = {b.first - a.first, b.second - a.second};
  const point bc = {c.first - b.first, c.second - b.second};

  return crossproduct(ab, bc) > 0;
}

bool compareAngles(const point& a, const point& b) {
  if (convex(origin, a, b)) {
    return true;
  } else if (convex(origin, b, a)) {
    return false;
  } else {
    // The two points are collinear with the origin, we have to compare
    // the distances. We could use the vector norm or the squared norm
    // for this, but the manhattan distance is also sufficient because
    // it also gets bigger if we scale a vector up without changing its
    // direction.
    const int distA = std::abs(a.first-origin.first) + std::abs(a.second-origin.second);
    const int distB = std::abs(b.first-origin.first) + std::abs(b.second-origin.second);

    return distA < distB;
  }
}


// The function that will be called to find the convex hull
// of a set of points.
std::vector<point> convex_hull(std::vector<point> points) {
  int n = points.size();

  // Trivial inputs can be solved right away. Depending on your
  // usecase you might have to make sure to sort the points in
  // the correct direction as well, i.e. counter-clockwise in
  // this case.
  if (n<3)
    return points;

  // We take the bottom right point and store it in points[0]
  // and in the gloabl variable `origin`
  int index_lowest = 0;
  for (int i = 1; i < n; ++i) {
    if (std::make_pair(points[i].second, -points[i].first) <
       std::make_pair(points[index_lowest].second, -points[index_lowest].first)) {
      index_lowest = i;
    }
  }
  std::swap(points[0], points[index_lowest]);
  origin = points[0];

  // The other points (without origin) get sorted using our own
  // compare function `compareAngles`
  std::sort(points.begin()+1, points.end(), compareAngles);

  // st is used as a stack to find the points of the convex hull
  std::vector<point> st;
  st.push_back(points[0]);
  st.push_back(points[1]);

  // the variable `pointer` stores, how far we got though the
  // sorted array of points already.
  int pointer = 2;
  while (pointer < n) {
    point a = st[st.size()-2];
    point b = st[st.size()-1];
    point c = points[pointer];
    if (convex(a,b,c)) {
      // The points form a convex corner. (ccw)
      st.push_back(c);
      ++pointer;
    } else {
      // The middle point does not lie on the convex hull.
      st.pop_back();
      if (st.size()<2) {
        st.push_back(c);
        ++pointer;
      }
    }
  }

  // If three collinear points are found at the end, we
  // remove the middle one.
  point a = st[st.size()-2];
  point b = st[st.size()-1];
  point c = st[0];
  if (!convex(a,b,c)) {
    st.pop_back();
  }

  return st;
}

bool cw(const point &a, const point &b, const point &c) {
    return (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first) < 0;
}

vector<point> convexHull(vector<point> p) {
    int n = p.size();
    if (n <= 1)
        return p;
    int k = 0;
    sort(p.begin(), p.end());
    vector<point> q(n * 2);
    for (int i = 0; i < n; q[k++] = p[i++])
        for (; k >= 2 && !cw(q[k - 2], q[k - 1], p[i]); --k)
            ;
    for (int i = n - 2, t = k; i >= 0; q[k++] = p[i--])
        for (; k > t && !cw(q[k - 2], q[k - 1], p[i]); --k)
            ;
    q.resize(k - 1 - (q[0] == q[1]));
    return q;
}


std::vector<point> generate_random_points(int num_points) {
    std::vector<point> points;
    for (int i = 0; i < num_points; i++) {
        points.push_back(make_pair(generate_pos(), generate_pos()));
    }
    return points;
}

int main() {
    // initialize the matrix to draw on
    int (*result)[WIDTH] = new int[HEIGHT][WIDTH] ; // Matrix of integers
    int n = 7;

    // Test the grahamsScan function with a few points
    std::vector<point> points = generate_random_points(n);
    for(point p: points) {
        cout << "(" << p.first << ", " << p.second << ")" << endl;
    }

    // generate vector of fancy Points from the vector of pairs
    std::vector<Point> Points;
    for(point p: points) {
        Points.push_back(Point(p.first, p.second));
    }

    // draw the points
    for(Point p: Points) {
        p.draw(result, 6);
    }

    // call convexHull
    std::vector<point> ch = convexHull(points);
    cout << "convexHull:" << endl;

    // generate vector of fancy Points from the vector of pairs
    std::vector<Point> convexHullPoints;
    for(point p: ch) {
        convexHullPoints.push_back(Point(p.first, p.second));
    }

    // draw the convex hull by generating LineSegments and drawing those
    for(int i = 0; i < convexHullPoints.size(); i++) {
        cout << "(" << convexHullPoints[i].x << ", " << convexHullPoints[i].y << ")" << endl;
        LineSegment l(convexHullPoints[i], convexHullPoints[(i+1)%convexHullPoints.size()]);
        l.draw(result, 2);
    }

    // Polygon(Points).draw(result, 999);

    // call convexHull
    // Polygon p(convex_hull(Points)); cout << p.getSides().size() << endl;
// 
    // for(LineSegment l: p.getSides()) {
    //     Line line = Line(l.getA(), l.getB());
    //     cout << line.getSlope() << line.getIntercept() << endl;
    //     line.draw(result, 2);
    // }
    // cout << "sorted Points:" << endl;
    // for(Point p: Points) {
    //     cout << "(" << p.x << ", " << p.y << ")" << endl;
    // }
// 
    // // draw the convex hull
    // p.draw(result, 2);

    // Polygon p(convexHull);
    // write the matrix to a file
    writePPM("l05.ppm", HEIGHT, WIDTH, result);

    return 0;
}