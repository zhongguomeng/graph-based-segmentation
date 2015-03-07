/*
 * Build eight nieghbor grid graph
 *
 * Input : a one-channel imH x imW image 
 * Outputs  : edge weights and end vertices
 *
 * The calling syntax is:
 *
 *		[edgeWeights, vertices] = buildGraph8N1C(C);
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
    if(nlhs!=2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "Two output required.");
    }
    // input
	double* inMatrix; //get 1 - D matrix
    // output
    double* edgeWeights; 
	double* vertices;
    
	/* create a pointer to the real data in the input matrix  */
	inMatrix = mxGetPr(prhs[0]);
    
    /* initialize helper variables */
	mwSize imW = mxGetN(prhs[0]); //get # of column
	mwSize imH = mxGetM(prhs[0]); //get # of row
	long outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
    
    /* create a pointer to the real data in the output matrix */
	plhs[0] = mxCreateDoubleMatrix(1, outArraySize, mxREAL); 
	edgeWeights = mxGetPr(plhs[0]);
	plhs[1] = mxCreateDoubleMatrix(2, outArraySize, mxREAL);
	vertices = mxGetPr(plhs[1]);
    
	/* call the computational routine */
	buildGraph8N1C(edgeWeights, vertices,
						inMatrix,imW, imH);
}

