//
//  mexImgSegment.cpp
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015年 Weiting. All rights reserved.
//

/**
 disjointSetIdx = segment_graph(num, sortedIdx, edgeWeights, lefts, rights, K)
 
 
**/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "meximgSegment.h"
#include <map>
//#include "mex.h"

#define PATH "/Users/weitingco/Dropbox/UCSB/2015-winter/ECE278A/Final_Project/DIP_FinalProject/DIP_FinalProject/graph/lena512_08_"
#define OPATH "/Users/weitingco/Dropbox/UCSB/2015-winter/ECE278A/Final_Project/DIP_FinalProject/DIP_FinalProject/output/lena512_08_"
#define pmK 500
#define CMAX 255
typedef struct { double R, G, B; } RGB;

size_t Seg2Color(double* R, double* G, double*B, double* segMap, int len);

float MInt(const myDJSNode& a, const myDJSNode& b, float K);
float MInt(const float w1, const float w2, float K);
inline float max3(float a, float b, float c);
void mexFunction(int node_num, int edge_num, double* segMap,
                 int* sortedIdx, float* edgeW, int* l, int* r, float K);
edges* constructGraph(float* w, int* l, int* r,int num);
myDisjointSet* constructSegment(edges* graph, int* sortedIdx, int num, float K, int edge_num);


string ToString(size_t sz) {
    stringstream ss;
    ss << sz;
    return ss.str();
}
/*
void mexFunction(int nlhs, mxArray *plhs[],
                 int rlhs, const mxArray *prhs[]){
    
}
*/

int main(){
    
    int height = 512;
    int width  = 512;
    
    int dlength = (height-1)*width + (width-1)*height + 2*(width-1)*(height-1);
    int num_nodes = height*width;
    string myPath(PATH);
    
    size_t numSeg;
    
    //input file
    ifstream fSortedIdx;
    fSortedIdx.open(myPath+"sortIdx.txt");
    int* sortedIdx = new int[dlength];
    
    ifstream fCNodes;
    fCNodes.open(myPath+"currentNodes.txt");
    int* cNodes = new int[dlength];
    
    ifstream fNNodes;
    fNNodes.open(myPath+"neighborNodes.txt");
    int* nNodes = new int[dlength];
    
    ifstream fWeights;
    fWeights.open(myPath+"weights.txt");
    float* edgeW = new float[dlength];
    
    
    int temp;
    int idx = 0;
    if(fSortedIdx.is_open()){
        idx = 0;
        while (fSortedIdx >> temp) {
            sortedIdx[idx] = temp;
            idx++;
        }
    }
    
    if (fCNodes.is_open()) {
        idx = 0;
        while (fCNodes >> temp) {
            cNodes[idx] = temp;
            idx++;
        }
    }
    if (fNNodes.is_open()) {
        idx = 0;
        while (fNNodes >> temp) {
            nNodes[idx] = temp;
            idx++;
        }
    }
    
    float temp_f;
    if(fWeights.is_open()){
        idx = 0;
        while (fWeights>>temp_f) {
            edgeW[idx] = temp_f;
            idx++;
        }
    }
    
    double* segMap = new double[num_nodes];
    
    mexFunction(num_nodes, dlength, segMap,
                sortedIdx, edgeW, cNodes, nNodes, pmK);
    
    
    
    fSortedIdx.close();
    fNNodes.close();
    fCNodes.close();
    
    double* R = new double[num_nodes];
    double* G = new double[num_nodes];
    double* B = new double[num_nodes];
    
    numSeg = Seg2Color(R, G, B, segMap, num_nodes);
    
    string sNumSeg = ToString(numSeg);
    
    string myOPath(OPATH);
    
    string savePath = myOPath+sNumSeg+"_";
    
    ofstream oFileR;
    oFileR.open(savePath+"mySegR.txt");
    
    ofstream oFileG;
    oFileG.open(savePath+"mySegG.txt");
    
    ofstream oFileB;
    oFileB.open(savePath+"mySegB.txt");
    
    for (int idx = 0; idx < num_nodes; ++idx) {
        oFileR << R[idx] << " ";
        oFileB << B[idx] << " ";
        oFileG << G[idx] << " ";
    }
    
    oFileB.close();
    oFileG.close();
    oFileR.close();
    
    delete [] R;
    delete [] G;
    delete [] B;
    delete [] segMap;
    delete [] sortedIdx;
    delete [] edgeW;
    delete [] nNodes;
    delete [] cNodes;
    
    return 0;
}

void mexFunction(int node_num, int edge_num, double* segMap,
                 int* sortedIdx, float* edgeW, int* l, int* r, float K){
    edges* Graph = constructGraph(edgeW, l, r, edge_num);
    myDisjointSet* Segments = constructSegment(Graph, sortedIdx, node_num, K,edge_num);
    string myPath(OPATH);
    int rep;
    //output file
    ofstream oFile;
    oFile.open(myPath+"myTestSeg.txt");
    //output the representive for each node
    for (int idx = 0; idx < node_num; ++idx) {
        rep = Segments->findSet(idx);
        segMap[idx] = rep;
        oFile <<  rep << " ";
    }
    oFile.close();
}


edges* constructGraph(float* w, int* l, int* r, int num){
    edges* graph = new edges[num];
    
    for (int idx = 0; idx < num; idx++) {
        graph[idx].w = w[idx];
        graph[idx].a = l[idx];
        graph[idx].b = r[idx];
    }
    
    return graph;
}

float MInt(const myDJSNode& a, const myDJSNode& b, float K){
    float tempW1 = a.MSTweight + K/(a.size);
    float tempW2 = b.MSTweight + K/(b.size);
    
    return ( (tempW1 < tempW2) ? tempW1 : tempW2);
}

inline float max3(float a, float b, float c){
    float temp = ( (a > b) ? a:b );
    return ( (temp > c) ? temp:c );
}

myDisjointSet* constructSegment(edges* graph, int* sortedIdx, int num, float K, int edge_num){
    myDisjointSet* segmentGraph = new myDisjointSet(num);
    int scanIdx = 0;
    //assign internal difference
    for (int temp_idx; temp_idx < num; temp_idx++) {
        //singalton
        segmentGraph->assignMSTW(temp_idx, 0);
    }
    
    //std::sort(graph, graph + edge_num);
    
    for (int idx = 0; idx < edge_num; idx++) {
        scanIdx = sortedIdx[idx];
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
    
    return mymap.size();
}