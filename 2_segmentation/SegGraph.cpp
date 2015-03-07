//
//  mexImgSegment.cpp
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015 Weiting. All rights reserved.
//
// 
/**
 [segResult] = SegGraph(edgeWeights, lefts, rights, imWidth, imHeight, edgeLength, K);
**/

#include "mex.h"
#include "SegGraphCore.cpp"

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]){
    
	if(nrhs!=7) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "Seven inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "One output required.");
    }
    double* edgeW; //get 1 - D matrix
    double* cNodes; //get 1 - D matrix
    double* nNodes; //get 1 - D matrix
    //get scaler
    double pmK; 
    double edgeNum;
    double height;
    double width;
    
    //get data from matlab
    edgeW = mxGetPr(prhs[0]); // edgeWeights
    cNodes = mxGetPr(prhs[1]);
    nNodes = mxGetPr(prhs[2]);
    //Segmentation parameter
    width = mxGetScalar(prhs[3]); // cols
    height = mxGetScalar(prhs[4]); // rows
    edgeNum = mxGetScalar(prhs[5]); // number of edges
    pmK = mxGetScalar(prhs[6]); // K
    
    int dlength = static_cast<int>((height-1)*width 
                        + (width-1)*height + 2*(width-1)*(height-1));
    int num_nodes = static_cast<int>(height*width);
    
    //output data
	plhs[0] = mxCreateDoubleMatrix(1, num_nodes, mxREAL);
    double* segMap = mxGetPr(plhs[0]);        
    
    Segmentation(num_nodes, dlength, segMap,
                edgeW, cNodes, nNodes, pmK);
}
