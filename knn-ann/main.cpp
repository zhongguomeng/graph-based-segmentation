#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

// to store queries results
#include <vector>

// just for output
#include <boost/foreach.hpp>

#include "..\..\graph-based-segmentation\1_buildGraph\buildKnnGraph.cpp"
#include "..\..\graph-based-segmentation\1_buildGraph\buildAnnGraph.cpp"
#include "..\..\graph-based-segmentation\1_buildGraph\buildGraphCore.cpp"
#include "..\..\graph-based-segmentation\2_segmentation\segGraphCore.cpp"
#include "..\..\graph-based-segmentation\3_seg2color\Seg2ColorCore.cpp"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned int> value;

typedef bg::model::point<float, 5, bg::cs::cartesian> myPoint;
typedef std::pair<myPoint, unsigned int> myV;

typedef bg::model::point<float, 5, bg::cs::cartesian> feature;
typedef std::pair<feature, unsigned int> node;

void seg8N3C(); // test func for 8N3C
void segKnn(char*, char*); // test func for Knn
void segAnn(char*, char*); // test func for Ann
void mat2array(double*,cv::Mat&); // util

int main(){
	//seg8N3C();
	//segKnn("../lena128seg.png","../lena128color.tiff");
	//segKnn("../lena256seg.png","../lena256color.tiff");
	//segKnn("../lena512seg.png","../lena512color.tiff");
	//segAnn("../lena128Ann.png","../lena128color.tiff");
	//segAnn("../lena256Ann.png","../lena256color.tiff");
	segAnn("../lena512HSV.png","../lena512color.tiff");
	//segAnn("../beachAnn.png","../beach.jpg");

	//system("PAUSE");
	return 0;
}

void segAnn(char* dst, char* src){
	cv::Mat imageC3 = cv::imread(src); // 8-bit unsigned int
	std::cout<<imageC3.rows<<' '<<imageC3.cols<<' '<<imageC3.channels()<<std::endl;
	int rows = imageC3.rows;
	int cols = imageC3.cols;

	cv::GaussianBlur( imageC3, imageC3, cv::Size(5,5), 0.8 );
	cv::cvtColor(imageC3, imageC3, CV_BGR2HSV);

	cv::Mat channels[3];
	cv::split(imageC3,channels);
	channels[0].convertTo(channels[0], CV_64F);
	channels[1].convertTo(channels[1], CV_64F);
	channels[2].convertTo(channels[2], CV_64F);

	double *R, *G, *B;
	B = new double[rows*cols*8];
	G = new double[rows*cols*8];
	R = new double[rows*cols*8];
	
	mat2array(R,channels[0]);
	mat2array(G,channels[1]);
	mat2array(B,channels[2]);

	// build graph
	int K=6;
	int edgeNum = 6*rows*cols;
	double* edgeWeight = new double[edgeNum];
	double* vertices1 = new double[edgeNum];
	double* vertices2 = new double[edgeNum];
	buildAnnGraph(edgeWeight, vertices1, vertices2, R,G,B, cols, rows,K);

	// segmentation
	double* segMap = new double[rows*cols];
	Segmentation(rows*cols,edgeNum,segMap,edgeWeight,vertices1, vertices2, 200,20,true);

	// seg to color
	double* segB = new double[rows*cols*channels[0].elemSize()];
	double* segG = new double[rows*cols*channels[1].elemSize()];
	double* segR = new double[rows*cols*channels[2].elemSize()];
	size_t segNum = Seg2Color(segR,segG,segB,segMap,rows*cols);
	cv::Mat resultRGB[3];
	resultRGB[0]=cv::Mat(rows,cols,CV_64FC1,segR);
	resultRGB[1]=cv::Mat(rows,cols,CV_64FC1,segG);
	resultRGB[2]=cv::Mat(rows,cols,CV_64FC1,segB);
	cv::Mat result(rows,cols,CV_64FC3);
	cv::merge(resultRGB,3,result);
	result.convertTo(result,CV_8U);

	//cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    //cv::imshow( "Display window", result );                   // Show our image inside it.
	cv::imwrite(dst,result);
    //cv::waitKey(0); 

	delete [] R;
	delete [] G;
	delete [] B;
	delete [] edgeWeight;
	delete [] vertices1;
	delete [] vertices2;
	delete [] segMap;
	delete [] segB;
	delete [] segG;
	delete [] segR;
}

void segKnn(char* dst, char* src){
	cv::Mat imageC3 = cv::imread(src); // 8-bit unsigned int
	int rows = imageC3.rows;
	int cols = imageC3.cols;
	//std::cout<<imageC3.channels()<<std::endl;
	cv::Mat channels[3];
	cv::split(imageC3,channels);
	channels[0].convertTo(channels[0], CV_64F); 
	channels[1].convertTo(channels[1], CV_64F);
	channels[2].convertTo(channels[2], CV_64F);
	
	//std::cout<<channels[0].at<double>(0,0)<<std::endl;

	double *R, *G, *B;
	B = new double[rows*cols*8];
	G = new double[rows*cols*8];
	R = new double[rows*cols*8];
	
	mat2array(R,channels[0]);
	mat2array(G,channels[1]);
	mat2array(B,channels[2]);

	// build graph
	int K=6;
	int edgeNum = 6*rows*cols;
	double* edgeWeight = new double[edgeNum];
	double* vertices1 = new double[edgeNum];
	double* vertices2 = new double[edgeNum];
	buildKnnGraph(edgeWeight, vertices1, vertices2, R,G,B, cols, rows,K);

	// segmentation
	double* segMap = new double[rows*cols];
	Segmentation(rows*cols,edgeNum,segMap,edgeWeight,vertices1, vertices2, 200,20, true);

	// seg to color
	double* segB = new double[rows*cols*channels[0].elemSize()];
	double* segG = new double[rows*cols*channels[1].elemSize()];
	double* segR = new double[rows*cols*channels[2].elemSize()];
	size_t segNum = Seg2Color(segR,segG,segB,segMap,rows*cols);
	cv::Mat resultRGB[3];
	resultRGB[0]=cv::Mat(rows,cols,CV_64FC1,segR);
	resultRGB[1]=cv::Mat(rows,cols,CV_64FC1,segG);
	resultRGB[2]=cv::Mat(rows,cols,CV_64FC1,segB);
	cv::Mat result(rows,cols,CV_64FC3);
	cv::merge(resultRGB,3,result);
	result.convertTo(result,CV_8U);

	//cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    //cv::imshow( "Display window", result );                   // Show our image inside it.
	cv::imwrite(dst,result);
    //cv::waitKey(0); 

	delete [] R;
	delete [] G;
	delete [] B;
	delete [] edgeWeight;
	delete [] vertices1;
	delete [] vertices2;
	delete [] segMap;
	delete [] segB;
	delete [] segG;
	delete [] segR;
}

void seg8N3C(){
		cv::Mat imageC3 = cv::imread("../lena512color.tiff"); // 8-bit unsigned int
	int rows = imageC3.rows;
	int cols = imageC3.cols;
	//std::cout<<imageC3.channels()<<std::endl;
	cv::Mat channels[3];
	cv::split(imageC3,channels);
	channels[0].convertTo(channels[0], CV_64F);
	channels[1].convertTo(channels[1], CV_64F);
	channels[2].convertTo(channels[2], CV_64F);
	
	//std::cout<<channels[0].at<double>(0,0)<<std::endl;

	double *R, *G, *B;
	B = new double[rows*cols*8];
	G = new double[rows*cols*8];
	R = new double[rows*cols*8];
	
	mat2array(R,channels[0]);
	mat2array(G,channels[1]);
	mat2array(B,channels[2]);

	// build graph
	int edgeNum = (rows-1)*cols + (cols-1)*rows + 2*(rows-1)*(cols-1);
	double* edgeWeight = new double[edgeNum];
	double* vertices1 = new double[edgeNum];
	double* vertices2 = new double[edgeNum];
	buildGraph8N3C(edgeWeight, vertices1, vertices2, R,G,B, cols, rows, 1,1,1);

	// segmentation
	double* segMap = new double[rows*cols];
	Segmentation(rows*cols,edgeNum,segMap,edgeWeight,vertices1, vertices2, 200,20,true);

	// seg to color
	double* segB = new double[rows*cols*channels[0].elemSize()];
	double* segG = new double[rows*cols*channels[1].elemSize()];
	double* segR = new double[rows*cols*channels[2].elemSize()];
	size_t segNum = Seg2Color(segR,segG,segB,segMap,rows*cols);
	cv::Mat resultRGB[3];
	resultRGB[0]=cv::Mat(rows,cols,CV_64FC1,segR);
	resultRGB[1]=cv::Mat(rows,cols,CV_64FC1,segG);
	resultRGB[2]=cv::Mat(rows,cols,CV_64FC1,segB);
	cv::Mat result(rows,cols,CV_64FC3);
	cv::merge(resultRGB,3,result);
	result.convertTo(result,CV_8U);

	cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", result );                   // Show our image inside it.

    cv::waitKey(0); 

	delete [] R;
	delete [] G;
	delete [] B;
	delete [] edgeWeight;
	delete [] vertices1;
	delete [] vertices2;
	delete [] segMap;
	delete [] segB;
	delete [] segG;
	delete [] segR;
}

void mat2array(double* dst, cv::Mat& src){
	bool continuous = src.isContinuous(); 
	if (continuous)
		memcpy(dst, src.ptr(),src.rows*src.cols*src.elemSize1());
	else {
		for (int i = 0; i < src.rows; i++) {
			memcpy(dst+src.cols*src.elemSize1(),src.ptr(i),src.rows*src.cols*src.elemSize1());
		}
	}
}