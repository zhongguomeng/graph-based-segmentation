/*
 * Build eight nieghbor grid graph
 *
 * Input : a three-channel imH x imW image 
 * Outputs  : edge weights and end vertices
 *
 * The calling syntax is:
 *
 *		[edgeWeights, vertices] = buildGraph8N3C(R,G,B,w1,w2,w3);
 *      R,G,B : in general it can be any three channels for images
 *      w1,w2,w3 : differnet weights for measuring the difference 
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"
#include "buildGraphCore.cpp"

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]){
    if(nrhs!=6) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "Six inputs required.");
    }
    if(nlhs!=2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "Two output required.");
    }
    // input
	double* inMatrixR; //get 1 - D matrix
    double* inMatrixG; //get 1 - D matrix
    double* inMatrixB; //get 1 - D matrix
    double *w1,*w2,*w3;
    // output
    double* edgeWeights; 
	double* vertices;
    
	/* create a pointer to the real data in the input matrix  */
	inMatrixR = mxGetPr(prhs[0]);
    inMatrixG = mxGetPr(prhs[1]);
    inMatrixB = mxGetPr(prhs[2]);
    w1 = mxGetPr(prhs[3]);
    w2 = mxGetPr(prhs[4]);
    w3 = mxGetPr(prhs[5]);
    
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
	buildGraph8N3C(edgeWeights, vertices,
						inMatrixR, inMatrixG, inMatrixB, imW, imH, *w1, *w2, *w3);
}