/*
 * Build eight nieghbor grid graph
 *
 * Input : a single channel imH x imW image 
 * Outputs  : 4*imH*imW
 *
 * The calling syntax is:
 *
 *		[R G B num] = Seg2Color(Seg)
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"
#include <vector>
#include <map>

#define CMAX 255

typedef struct { double R, G, B; } RGB;

int Seg2Color(double* R, double* G, double*B, double* segMap, mwSize len);

void mexFunction(int nlhs, mxArray *plhs[], 
				 int rlhs, const mxArray *prhs[]){
	double* inMatrix;
	mwSize len;

	double* R;
	double* G;
	double* B;
    double* segSize;

	inMatrix = mxGetPr(prhs[0]);
	len = mxGetN(prhs[0]);

	plhs[0] = mxCreateDoubleMatrix(1, len, mxREAL); 
	plhs[1] = mxCreateDoubleMatrix(1, len, mxREAL); 
	plhs[2] = mxCreateDoubleMatrix(1, len, mxREAL); 
    plhs[3] = mxCreateDoubleMatrix(1,1,mxREAL);
    
	R = mxGetPr(plhs[0]);
	G = mxGetPr(plhs[1]);
	B = mxGetPr(plhs[2]);
    
    segSize = Seg2Color(R,G,B,inMatrix,len);

}

int Seg2Color(double* R, double* G, double*B, double* segMap, mwSize len){
	std::map<double,RGB> mymap;
	std::map<char,int>::iterator it;

	double tR;
	double tG;
	double tB;

	double cNode;

	for (mwSize i = 0; i < len; ++i){
		cNode = segMap[i];
		//haven't assigned the color
		it = mymap.find(cNode);
		if ( it == mymap.end() ){
			//assign the color
			tR = rand() % CMAX;
			tG = rand() % CMAX;
			tB = rand() % CMAX;

			RGB* rgb = new RGB;
            
            (*rgb).R = tR;
            (*rgb).B = tB;
            (*rgb).G = tG;
            
			mymap.insert(std::pair<double,RGB>(cNode,*rgb));
		} else {
			tR = it->second.R;
            tB = it->second.B;
            tG = it->second.G;
		}
        R[i] = tR;
        G[i] = tG;
        B[i] = tB;
	}
    
    return mymap.size();
}