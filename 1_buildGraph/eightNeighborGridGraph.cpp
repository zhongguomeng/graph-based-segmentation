/*
 * Build eight nieghbor grid graph
 *
 * Input : a three-channel imH x imW image 
 * Outputs  : edge weights and end vertices
 *
 * The calling syntax is:
 *
<<<<<<< HEAD:build_8N_GridGraph_RGB.cpp
 *		[edgeWeights, vertices] = build_8N_GridGraph_RGB(R,G,B,w1,w2,w3);
=======
 *		[edgeWeights, vertices] = eightNeighborGridGraph(R,G,B,w1,w2,w3);
>>>>>>> ba306a3c3d3ea1a60d4fa880ce79f241a7951339:eightNeighborGridGraph.cpp
 *      R,G,B : in general it can be any three channels for images
 *      w1,w2,w3 : differnet weights for measuring the difference 
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"
#include <math.h>


// dissimilarity measure between pixels
static inline float diff(int x1, int y1, int x2, int y2, int rowNum, 
        double* R, double* G, double* B, double w1, double w2, double w3) {
  double Rd = R[x1 + y1*rowNum] - R[x2 + y2*rowNum];
  double Gd = G[x1 + y1*rowNum] - G[x2 + y2*rowNum];
  double Bd = B[x1 + y1*rowNum] - B[x2 + y2*rowNum];
  return sqrt(w1*Rd*Rd+w2*Gd*Gd+w3*Bd*Bd);
}

void eightNeighborGridGraph(double* edgeWeight, double* vertices,
						double* R, double* G, double* B, mwSize width, mwSize height, double w1, double w2, double w3);

void mexFunction(int nlhs, mxArray *plhs[], 
				 int rlhs, const mxArray *prhs[]){
	double* inMatrixR; //get 1 - D matrix
    double* inMatrixG; //get 1 - D matrix
    double* inMatrixB; //get 1 - D matrix
	mwSize imH;
	mwSize imW;
	double* edgeWeights; 
	double* vertices;
	long outArraySize;
    
    double *w1,*w2,*w3;

	/* create a pointer to the real data in the input matrix  */
	inMatrixR = mxGetPr(prhs[0]);
    inMatrixG = mxGetPr(prhs[1]);
    inMatrixB = mxGetPr(prhs[2]);
    
    w1 = mxGetPr(prhs[3]);
    w2 = mxGetPr(prhs[4]);
    w3 = mxGetPr(prhs[5]);
    
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
	eightNeighborGridGraph(edgeWeights, vertices,
						inMatrixR, inMatrixG, inMatrixB, imW, imH, *w1, *w2, *w3);

}

void eightNeighborGridGraph(double* edgeWeight, double* vertices,
						double* R, double* G, double* B, mwSize imW, mwSize imH, double w1, double w2, double w3){

	long num = 0;

	for(int y = 0; y < imW; y++){
		for(int x = 0; x < imH; x++){

			//Connect vertical edges
			if(x < imH - 1){
				edgeWeight[num] = diff(x,y,x+1,y,imH,R, G, B, w1, w2, w3);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = x+1 + y*imH;
				num++;
			}

			//Connect horizontal edges
			if (y < imW-1){
				edgeWeight[num] = diff(x,y,x,y+1,imH, R, G, B, w1, w2, w3);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = x + (y+1)*imH;
				num++;
			}

			//Connect up-left to down-left edges
			if(x < imH -1 && y < imW-1){
				edgeWeight[num] = diff(x,y,x+1,y+1,imH, R, G, B, w1, w2, w3);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = (x+1) + (y+1)*imH;
				num++;
			}

			//Connect down-left to up-right edges
			if(x > 0 && y < imW-1){
				edgeWeight[num] = diff(x,y,x-1,y+1,imH, R, G, B, w1, w2, w3);
				vertices[2*num] = x + y*imH;
				vertices[2*num+1] = (x-1) + (y+1)*imH;
				num++;
			}

		}
	}
}

