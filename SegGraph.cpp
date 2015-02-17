//
//  mexImgSegment.cpp
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015 Weiting. All rights reserved.
//

/**
 [SegMapR SegMapG SegMapB SegNum] = SegGraph(sortedIdx, edgeWeights, lefts, rights, K);
**/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "meximgSegment2.h"
#include <map>
#include "mex.h"
#define CMAX 255
typedef struct { double R, G, B; } RGB;

size_t Seg2Color(double* R, double* G, double*B, double* segMap, int len);

float MInt(const myDJSNode& a, const myDJSNode& b, double K);
float MInt(const float w1, const float w2, float K);
inline float max3(double a, double b, double c);
void Segmentation(int node_num, int edge_num, double* segMap,
                 double* sortedIdx, double* edgeW, double* l, double* r, double K);
edges* constructGraph(double* w, double* l, double* r,int num);
myDisjointSet* constructSegment(edges* graph, double* sortedIdx, int num, double K, int edge_num);


string ToString(size_t sz) {
    stringstream ss;
    ss << sz;
    return ss.str();
}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int rlhs, const mxArray *prhs[]){
    
    int height = 512;
    int width  = 512;
    
    int dlength = (height-1)*width + (width-1)*height + 2*(width-1)*(height-1);
    int num_nodes = height*width;
    
    double* sortedIdx; //get 1-D matrix
    double* edgeW; //get 1 - D matrix
    double* cNodes; //get 1 - D matrix
    double* nNodes; //get 1 - D matrix
    double pmK; //get scaler
    
    //get data from matlab
    sortedIdx = mxGetPr(prhs[0]);
    edgeW = mxGetPr(prhs[1]);
    cNodes = mxGetPr(prhs[2]);
    nNodes = mxGetPr(prhs[3]);
    
    pmK = mxGetScalar(prhs[4]);
    
    //output data
    double* R;
    double* G;
    double* B;
    double* numSeg;
    
    plhs[0] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //R
    plhs[1] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //G
    plhs[2] = mxCreateDoubleMatrix(1, num_nodes, mxREAL); //B
    plhs[3] = mxCreateDoubleMatrix(1, 1, mxREAL);// numSeg
    
    R = mxGetPr(plhs[0]);
    G = mxGetPr(plhs[1]);
    B = mxGetPr(plhs[2]);
    numSeg = mxGetPr(plhs[3]);
            
    double* segMap = new double[num_nodes];
    
    Segmentation(num_nodes, dlength, segMap,
                sortedIdx, edgeW, cNodes, nNodes, pmK);
    
    size_t temp = Seg2Color(R, G, B, segMap, num_nodes);
    //*numSeg = (double) temp;
    
    delete [] segMap;
}

void Segmentation(int node_num, int edge_num, double* segMap,
                 double* sortedIdx, double* edgeW, double* l, double* r, double K){
    edges* Graph = constructGraph(edgeW, l, r, edge_num);
    
    myDisjointSet* Segments = constructSegment(Graph, sortedIdx, node_num, K,edge_num);
    int rep;
    for (int idx = 0; idx < node_num; ++idx) {
        rep = Segments->findSet(idx);
        segMap[idx] = static_cast<double>(rep);
    }
    
    //delete [] Graph;
    //delete [] Segments;
}


edges* constructGraph(double* w, double* l, double* r, int num){
    edges* graph = new edges[num];
    
    for (int idx = 0; idx < num; idx++) {
        graph[idx].w = static_cast<float>(w[idx]);
        graph[idx].a = static_cast<int>(l[idx]+0.5);
        graph[idx].b = static_cast<int>(r[idx]+0.5);
    }
    
    return graph;
}

float MInt(const myDJSNode& a, const myDJSNode& b, double K){
    double tempW1 = a.MSTweight + K/(a.size);
    double tempW2 = b.MSTweight + K/(b.size);
    
    return ( (tempW1 < tempW2) ? tempW1 : tempW2);
}

inline float max3(double a, double b, double c){
    float temp = ( (a > b) ? a:b );
    return ( (temp > c) ? temp:c );
}

myDisjointSet* constructSegment(edges* graph, double* sortedIdx, int num, double K, int edge_num){
    myDisjointSet* segmentGraph = new myDisjointSet(num);
    int scanIdx = 0;
    //assign internal difference
    for (int temp_idx; temp_idx < num; temp_idx++) {
        //singalton
        segmentGraph->assignMSTW(temp_idx, 0);
    }
    
    //std::sort(graph, graph + edge_num);
    
    for (int idx = 0; idx < edge_num; idx++) {
        scanIdx = static_cast<int>(sortedIdx[idx]+0.5);
        edges* c_edge = &graph[scanIdx]; //the current edge
        
        //find components through the representitives
        int s1 = segmentGraph->findSet(c_edge->a);
        int s2 = segmentGraph->findSet(c_edge->b);
        
        float w1 = (*segmentGraph)[s1].MSTweight;
        float w2 = (*segmentGraph)[s2].MSTweight;
        
        if (s1 != s2) {
            //compute predicate MInt
            float threshold = MInt((*segmentGraph)[s1], (*segmentGraph)[s2], K);
            if (c_edge->w <= threshold) {
                segmentGraph->unionSets(s1, s2);
                //update MST weight
                int rep = segmentGraph->findSet(s1);
                (*segmentGraph)[rep].MSTweight = max3(w1, w2, c_edge->w);
            }
        }
        
        
    } //end segmentation
    
    return segmentGraph;
}

size_t Seg2Color(double* R, double* G, double*B, double* segMap, int len){
    map<double,RGB> mymap;
    map<double,RGB>::iterator it;
    
    double tR;
    double tG;
    double tB;
    
    double cNode;
    
    for (int i = 0; i < len; ++i){
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
    size_t temp = mymap.size();
    mymap.clear();
    return temp;
}