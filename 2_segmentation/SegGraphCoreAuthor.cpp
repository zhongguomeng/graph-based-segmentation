//
//  mexImgSegment.cpp
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015 Weiting. All rights reserved.
//
// 
/**
 [SegMapR SegMapG SegMapB SegNum] = SegGraph(edgeWeights, lefts, rights, imWidth, imHeight, edgeLength, K);
**/

//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <sstream>
#include <algorithm>
#include "../0_utils/myDisjointSet.h"
#include "../0_utils/myDisjointSet.cpp"

float MInt(const myDJSNode& a, const myDJSNode& b, double K);
float MInt(const float w1, const float w2, float K);
inline float max3(double a, double b, double c);
void Segmentation(int node_num, int edge_num, double* segMap,
                 double* edgeW, double* l, double* r, double K, double minSize, bool forceMerge);
edges* constructGraph(double* w, double* l, double* r,int num);
myDisjointSet* constructSegment(edges* graph, int num, double K, int edge_num, double minSize, bool forceMerge);


//string ToString(size_t sz) {
//    stringstream ss;
//    ss << sz;
//    return ss.str();
//}

void Segmentation(int node_num, int edge_num, double* segMap,
                 double* edgeW, double* l, double* r, double K, double minSize, bool forceMerge){
    edges* Graph = constructGraph(edgeW, l, r, edge_num);
    
    myDisjointSet* Segments = constructSegment(Graph, node_num, K,edge_num, minSize, forceMerge);
    
    for (int idx = 0; idx < node_num; ++idx) {
        segMap[idx] = static_cast<double> (Segments->findSet(idx));
    }
    
    
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

myDisjointSet* constructSegment(edges* graph, int num, double K, int edge_num, 
            double minSize, bool forceMerge){

    myDisjointSet* segmentGraph = new myDisjointSet(num);
    int scanIdx = 0;
    //assign internal difference
    for (int temp_idx=0; temp_idx < num; temp_idx++) {
        //singalton
        segmentGraph->assignMSTW(temp_idx, 0);
    }
    
    std::sort(graph, graph + edge_num);
    
    for (int idx = 0; idx < edge_num; idx++) {
        //scanIdx = static_cast<int>(sortedIdx[idx]+0.5);
        edges* c_edge = &graph[idx]; //the current edge
        
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
            } /*else if(((*segmentGraph)[s1].size < minSize 
                    || (*segmentGraph)[s2].size < minSize)
                    && forceMerge){ 
                segmentGraph->unionSets(s1, s2);

                //update MST weight
                int rep = segmentGraph->findSet(s1);

                //forced merge, keep the MST weight as the largest segment

                if ((*segmentGraph)[s1].size <= (*segmentGraph)[s2].size){
                    (*segmentGraph)[rep].MSTweight = w2;
                }else{
                    (*segmentGraph)[rep].MSTweight = w1;
                }
            } else {
                //do nothing
            }*/
        }
        
        
    } //end segmentation
    
    //post processing
    for (int idx = 0; idx < edge_num; ++idx){
        edges* c_edge = &graph[idx]; //the current edge
        
        int n1 = segmentGraph->findSet(c_edge->a);
        int n2 = segmentGraph->findSet(c_edge->b);
        if((n1 != n2) && ((*segmentGraph)[n1].size < minSize || (*segmentGraph)[n2].size < minSize)){
            segmentGraph->unionSets(n1,n2);
        }
    }
    
    return segmentGraph;
}

