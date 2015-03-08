/* mex core graph building c/c++ functions
*/

#include <math.h>
#include <iostream>
#include <fstream>
////////////////// 1 Channel //////////////////

// dissimilarity measure between pixels
static inline double diff(int x1, int y1, int x2, int y2, int colNum, double* c) {
    return abs(c[x1 + y1*colNum] - c[x2 + y2*colNum]);
}

void buildGraph8N1C(double* edgeWeight, double* vertices1, double*vertices2,
                double* c,
                int imW, int imH){
	long num = 0; // 4 byte int
	for(int y = 0; y < imH; y++){
	for(int x = 0; x < imW; x++){
        //Connect vertical edges
        if(x < imW - 1){
            edgeWeight[num] = diff(x,y,x+1,y,imW, c);
            vertices1[num] = x + y*imW;
            vertices2[num] = x+1 + y*imW;
            num++;
        }
        //Connect horizontal edges
        if (y < imH-1){
            edgeWeight[num] = diff(x,y,x,y+1,imW, c);
            vertices1[num] = x + y*imW;
            vertices2[num] = x + (y+1)*imW;
            num++;
        }
        //Connect top-left to bot-right edges
        if(x < imW -1 && y < imH-1){
            edgeWeight[num] = diff(x,y,x+1,y+1,imW, c);
            vertices1[num] = x + y*imW;
            vertices2[num] = (x+1) + (y+1)*imW;
            num++;
        }
        //Connect bot-left to top-right edges
        if(x > 0 && y < imH-1){
            edgeWeight[num] = diff(x,y,x-1,y+1,imW, c);
            vertices1[num] = x + y*imW;
            vertices2[num] = (x-1) + (y+1)*imW;
            num++;
        }
    }}
}

///////////// 3 Channel ///////////////

// dissimilarity measure between pixels
static inline double diff(int x1, int y1, int x2, int y2, int colNum,
        double* R, double* G, double* B, double w1, double w2, double w3, int num) {
    double Rd = R[x1 + y1*colNum] - R[x2 + y2*colNum];
    double Gd = G[x1 + y1*colNum] - G[x2 + y2*colNum];
    double Bd = B[x1 + y1*colNum] - B[x2 + y2*colNum];
	return sqrt(w1*Rd*Rd+w2*Gd*Gd+w3*Bd*Bd);
}

void buildGraph8N3C(double* edgeWeight, double* vertices1, double* vertices2,
                double* R, double* G, double* B, 
                int imW, int imH, 
                double w1, double w2, double w3){
	int num = 0; // 4 byte int
	for(int y = 0; y < imH; y++){
	for(int x = 0; x < imW; x++){
        //Connect vertical edges
        if(x < imW - 1){
            edgeWeight[num] = diff(x,y,x+1,y,imW,R, G, B, w1, w2, w3,num);
            vertices1[num] = x + y*imW;
            vertices2[num] = x+1 + y*imW;
            num++;
        }
        //Connect horizontal edges
        if (y < imH-1){
            edgeWeight[num] = diff(x,y,x,y+1,imW, R, G, B, w1, w2, w3,num);
            vertices1[num] = x + y*imW;
            vertices2[num] = x + (y+1)*imW;
            num++;
        }
        //Connect top-left to bot-right edges
        if(x < imW -1 && y < imH-1){
            edgeWeight[num] = diff(x,y,x+1,y+1,imW, R, G, B, w1, w2, w3,num);
            vertices1[num] = x + y*imW;
            vertices2[num] = (x+1) + (y+1)*imW;
            num++;
        }
        //Connect bot-left to top-right edges
        if(x > 0 && y < imH-1){
            edgeWeight[num] = diff(x,y,x-1,y+1,imW, R, G, B, w1, w2, w3,num);
            vertices1[num] = x + y*imW;
            vertices2[num] = (x-1) + (y+1)*imW;
            num++;
        }
    }}
}