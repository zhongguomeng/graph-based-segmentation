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

typedef bg::model::point<float, 5, bg::cs::cartesian> Feature; // (x,y,r,g,b)
typedef std::pair<Feature, unsigned int> Node;

void buildKnnGraph(double* edgeWeight, double* vertices1, double* vertices2,
				   double* R, double* G, double* B, int cols, int rows, int K){
	// initialize R-Tree
	bgi::rtree<Node, bgi::quadratic<16> > rtree;

	// construct R-Tree
	unsigned int num=0;
	for(int y=0; y<rows; y++){
	for(int x=0 ; x<cols ; x++){
		// create a feature point
		Feature f;
		bg::set<0>(f,x);
		bg::set<1>(f,y);
		bg::set<2>(f,R[x+y*cols]);
		bg::set<3>(f,G[x+y*cols]);
		bg::set<4>(f,B[x+y*cols]);
		// insert new value
		rtree.insert(std::make_pair(f, num));
		num++;
	}}

	// find K nearest value to every feature point
	num=0;
	std::vector<Node> q_result;
	for(int y=0; y<rows; y++){
	for(int x=0 ; x<cols ; x++){
		// create a feature point
		Feature target;
		bg::set<0>(target,x);
		bg::set<1>(target,y);
		bg::set<2>(target,R[x+y*cols]);
		bg::set<3>(target,G[x+y*cols]);
		bg::set<4>(target,B[x+y*cols]);
		// find K+1 nearest neighbor (including itself)
		rtree.query(bgi::nearest(target, K+1), std::back_inserter(q_result));
		// construct edges
		for(auto it=q_result.begin(); it!=q_result.end(); it++){
			if(boost::geometry::equals(target, (*it).first))
				continue;
			edgeWeight[num] = bg::distance(target,(*it).first);
			vertices1[num] = x+y*cols;
			vertices2[num] = (*it).first.get<0>()+(*it).first.get<1>()*cols;
			num++;
		}
		q_result.clear();
	}}
}