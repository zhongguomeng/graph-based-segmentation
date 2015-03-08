/*
 * Build eight nieghbor grid graph
 *
 * Input : a one-channel imH x imW image 
 * Outputs  : edge weights and end vertices
 *
 * The calling syntax is:
 *
 *		[edgeWeights, vertices1, vertices2] = buildGraph8N1C(C);
 *      C: single-channel img data
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"
#include "buildGraphCore.cpp"

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]){
    if(nrhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "One inputs required.");
    }
    if(nlhs!=3) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "Three output required.");
    }
    // input
	double* inMatrix; //get 1 - D matrix
    // output
    double* edgeWeights; 
	double* vertices1;
	double* vertices2;
    
	/* create a pointer to the real data in the input matrix  */
	inMatrix = mxGetPr(prhs[0]);
    
    /* initialize helper variables */
	mwSize imW = mxGetN(prhs[0]); //get # of column
	mwSize imH = mxGetM(prhs[0]); //get # of row
	long outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
    
    /* create a pointer to the real data in the output matrix */
	plhs[0] = mxCreateDoubleMatrix(1, outArraySize, mxREAL); 
	edgeWeights = mxGetPr(plhs[0]);
	plhs[1] = mxCreateDoubleMatrix(1, outArraySize, mxREAL);
	vertices1 = mxGetPr(plhs[1]);
	plhs[2] = mxCreateDoubleMatrix(1, outArraySize, mxREAL);
	vertices2 = mxGetPr(plhs[2]);
    
	/* call the computational routine */
	buildGraph8N1C(edgeWeights, vertices1, vertices2,
						inMatrix,imW, imH);
}

