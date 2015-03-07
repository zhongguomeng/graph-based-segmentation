/* USAGE:
 [SegMapR SegMapG SegMapB SegNum] = SegGraph(segResult, width, height);
*/
#include "mex.h"
#include "Seg2ColorCore.cpp"

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[]){
    
	if(nrhs!=3) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
                "Three inputs required.");
    }
    if(nlhs!=4) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
                "Four output required.");
    }
	// inputs
	double* segMap = mxGetPr(prhs[0]); // edgeWeights
	double width = mxGetScalar(prhs[1]); // cols
    double height = mxGetScalar(prhs[2]); // rows
	int num_nodes = static_cast<int> (width*height);

    //output data
    double* R;
    double* G;
    double* B;
    double* numSeg;
    
    plhs[0] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //R
    plhs[1] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //G
    plhs[2] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //B
    plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);// numSeg

	size_t temp = Seg2Color(R, G, B, segMap, num_nodes);
    *numSeg = (double) temp;

}