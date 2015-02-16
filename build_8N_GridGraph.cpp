/*
 * Build eight nieghbor grid graph
 *
 * Input : a single channel imH x imW image 
 * Outputs  : 4*imH*imW
 *
 * The calling syntax is:
 *
 *		[edgeWeights vertices] = build_8N_GridGraph(image)
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"

// dissimilarity measure between pixels
static inline float diff(int x1, int y1, int x2, int y2, int rowNum, double* inMatrix) {
  return inMatrix[x1 + y1*rowNum] - inMatrix[x2 + y2*rowNum];
}

void build_8N_GridGraph(double* edgeWeight, double* vertices,
						double* inMatrix, mwSize width, mwSize height);

void mexFunction(int nlhs, mxArray *plhs[], 
				 int rlhs, const mxArray *prhs[]){
	double* inMatrix; //get 1 - D matrix
	mwSize imH;
	mwSize imW;
	double* edgeWeights; 
	double* vertices;
	long outArraySize;

	/* create a pointer to the real data in the input matrix  */
	inMatrix = mxGetPr(prhs[0]);
	imW = mxGetN(prhs[0]); //get # of column
	imH = mxGetM(prhs[0]); //get # of row

	outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
	/* create the output matrix */
	plhs[0] = mxCreateDoubleMatrix(1, outArraySize, mxREAL); 
	/* get a pointer to the real data in the output matrix */
	edgeWeights = mxGetPr(plhs[0]);

	plhs[1] = mxCreateDoubleMatrix(2, outArraySize, mxREAL);
	vertices = mxGetPr(plhs[1]);

	/* call the computational routine */
	build_8N_GridGraph(edgeWeights, vertices,
						inMatrix, imW, imH);

}

void build_8N_GridGraph(double* edgeWeight, double* vertices,
						double* inMatrix, mwSize imW, mwSize imH){

	long num = 0;

	for(int x = 0; x < imH; x++){
		for(int y = 0; y < imW; y++){

			//Connect vertical edges
			if(x < imH - 1){
				edgeWeight[num] = diff(x,y,x+1,y,imH,inMatrix);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = x+1 + y*imH;
				num++;
			}

			//Connect horizontal edges
			if (y < imW-1){
				edgeWeight[num] = diff(x,y,x,y+1,imH,inMatrix);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = x + (y+1)*imH;
				num++;
			}

			//Connect up-left to down-left edges
			if(x < imH -1 && y < imW-1){
				edgeWeight[num] = diff(x,y,x+1,y+1,imH,inMatrix);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = (x+1) + (y+1)*imH;
				num++;
			}

			//Connect down-left to up-right edges
			if(x > 0 && y < imW-1){
				edgeWeight[num] = diff(x,y,x-1,y+1,imH,inMatrix);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = (x-1) + (y+1)*imH;
				num++;
			}

		}
	}
}

