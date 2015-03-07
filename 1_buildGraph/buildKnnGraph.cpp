#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

// to store queries results
#include <vector>

// just for output
#include <boost/foreach.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned int> value;

typedef bg::model::point<float, 5, bg::cs::cartesian> myPoint;
typedef std::pair<myPoint, unsigned int> myV;

typedef bg::model::point<float, 5, bg::cs::cartesian> feature;
typedef std::pair<feature, unsigned int> node;

void buildKnnGraph(double* edgeWeight, double* vertices, 
				   double* R, double* G, double* B, int rows, int cols){

}