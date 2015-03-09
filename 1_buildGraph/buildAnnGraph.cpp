#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>
#include <vector>

void buildAnnGraph(double* edgeWeight, double* vertices1, double* vertices2,
				   double* R, double* G, double* B, int cols, int rows, int K){

	double maxR=-99999,minR=99999;
	double maxG=-99999,minG=99999;
	double maxB=-99999,minB=99999;
	for(int i=0; i<rows*cols; i++){
		if(maxR<R[i])
			maxR=R[i];
		if(minR>R[i])
			minR=R[i];
		if(maxG<G[i])
			maxG=G[i];
		if(minG>G[i])
			minG=G[i];
		if(maxB<B[i])
			maxB=B[i];
		if(minB>B[i])
			minB=B[i];
	}
	std::cout<<"min max done"<<std::endl;
	// prepare feature
	cv::Mat features(rows*cols,5,CV_64FC1);
	for(int y=0; y<rows; y++){
	for(int x=0; x<cols; x++){
		int id = x+y*cols;
		features.at<double>(id,0)=x/cols*1024;
		features.at<double>(id,1)=y/rows*1024;
		features.at<double>(id,2)=(R[id]-minR)/(maxR-minR)*1024;
		features.at<double>(id,3)=(G[id]-minG)/(maxG-minG)*1024;
		features.at<double>(id,4)=(B[id]-minB)/(maxB-minB)*1024;
	}}
	std::cout<<"feature prepare done"<<std::endl;
	// construct flann indexing
	cv::flann::GenericIndex<cv::flann::L2<double>> index(features, cvflann::AutotunedIndexParams());
	std::cout<<"index constructed"<<std::endl;
	// query for every pixel
	cv::Mat indices(rows*cols, K+1, CV_32S);
	cv::Mat dist(rows*cols, K+1, CV_64F);
	index.knnSearch(features, indices, dist, K+1, cvflann::SearchParams(32));
	std::cout<<"ANN found"<<std::endl;
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
	std::cout<<"graph constructed"<<std::endl;
}