#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>
#include <vector>

void buildAnnGraph(double* edgeWeight, double* vertices1, double* vertices2,
				   double* R, double* G, double* B, int cols, int rows, int K){
	// prepare feature
	cv::Mat features(rows*cols,5,CV_64FC1);
	for(int y=0; y<rows; y++){
	for(int x=0; x<cols; x++){
		int id = x+y*cols;
		features.at<double>(id,0)=x;
		features.at<double>(id,1)=y;
		features.at<double>(id,2)=R[id];
		features.at<double>(id,3)=G[id];
		features.at<double>(id,4)=B[id];
	}}
	
	// construct flann indexing
	cv::flann::GenericIndex<cv::flann::L2<double>> index(features, cvflann::AutotunedIndexParams());

	// query for every pixel
	cv::Mat indices(rows*cols, K+1, CV_32S);
	cv::Mat dist(rows*cols, K+1, CV_64F);
	index.knnSearch(features, indices, dist, K+1, cvflann::SearchParams(32));

	// construct edges
	int num=0;
	for(int y=0; y<rows; y++){
	for(int x=0; x<cols; x++){
		int i=x+y*cols;
		for(int j=0; j<K+1; j++){
			if(indices.at<int>(i,j)==i)
				continue;
			edgeWeight[num]=sqrt(dist.at<double>(i,j));
			vertices1[num]=i;
			vertices2[num]=features.at<double>(indices.at<int>(i,j),0)
				+features.at<double>(indices.at<int>(i,j),1)*cols;
			num++;
		}
	}}
}