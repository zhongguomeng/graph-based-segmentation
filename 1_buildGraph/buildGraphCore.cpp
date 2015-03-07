/* mex core graph building c/c++ functions
*/

#include <math.h>

////////////////// 1 Channel //////////////////

// dissimilarity measure between pixels
static inline double diff(int x1, int y1, int x2, int y2, int rowNum,
        double* c) {
    return abs(c[x1 + y1*rowNum] - c[x2 + y2*rowNum]);
}

void buildGraph8N1C(double* edgeWeight, double* vertices,
                double* c,
                mwSize imW, mwSize imH){
	long num = 0; // 4 byte int
	for(int y = 0; y < imW; y++){
	for(int x = 0; x < imH; x++){
        //Connect vertical edges
        if(x < imH - 1){
            edgeWeight[num] = diff(x,y,x+1,y,imH, c);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = x+1 + y*imH;
            num++;
        }
        //Connect horizontal edges
        if (y < imW-1){
            edgeWeight[num] = diff(x,y,x,y+1,imH, c);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = x + (y+1)*imH;
            num++;
        }
        //Connect top-left to bot-right edges
        if(x < imH -1 && y < imW-1){
            edgeWeight[num] = diff(x,y,x+1,y+1,imH, c);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = (x+1) + (y+1)*imH;
            num++;
        }
        //Connect bot-left to top-right edges
        if(x > 0 && y < imW-1){
            edgeWeight[num] = diff(x,y,x-1,y+1,imH, c);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = (x-1) + (y+1)*imH;
            num++;
        }
    }}
}

///////////// 3 Channel ///////////////

// dissimilarity measure between pixels
static inline double diff(int x1, int y1, int x2, int y2, int rowNum,
        double* R, double* G, double* B, double w1, double w2, double w3) {
    double Rd = R[x1 + y1*rowNum] - R[x2 + y2*rowNum];
    double Gd = G[x1 + y1*rowNum] - G[x2 + y2*rowNum];
    double Bd = B[x1 + y1*rowNum] - B[x2 + y2*rowNum];
    return sqrt(w1*Rd*Rd+w2*Gd*Gd+w3*Bd*Bd);
}

void buildGraph8N3C(double* edgeWeight, double* vertices,
                double* R, double* G, double* B, 
                mwSize imW, mwSize imH, 
                double w1, double w2, double w3){
	long num = 0; // 4 byte int
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
        //Connect top-left to bot-right edges
        if(x < imH -1 && y < imW-1){
            edgeWeight[num] = diff(x,y,x+1,y+1,imH, R, G, B, w1, w2, w3);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = (x+1) + (y+1)*imH;
            num++;
        }
        //Connect bot-left to top-right edges
        if(x > 0 && y < imW-1){
            edgeWeight[num] = diff(x,y,x-1,y+1,imH, R, G, B, w1, w2, w3);
            vertices[2*num] = x + y*imH;
            vertices[2*num+1] = (x-1) + (y+1)*imH;
            num++;
        }
    }}
}