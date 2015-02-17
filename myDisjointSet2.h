//
//  myDisjointSet.h
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015 Weiting. All rights reserved.
//

#ifndef __DIP_FinalProject__myDisjointSet__
#define __DIP_FinalProject__myDisjointSet__

#include <stdio.h>
#include <vector>
#include "edges.h"

using namespace std;
//#include "typeUtility.h"

class myDJSNode{
    //copy constructor
public:
    int p; //index to the parent
    int rank;
    int size; //size of the disjoint set
    float MSTweight;
    
};

class myDisjointSet{
public:
    myDisjointSet(int num);
    ~myDisjointSet(){ delete [] nodes; }
    
    //Disjoint Set methods
    
    //makeSet is already performed in initialization
    //void makeSet(int idx);
    
    int findSet(int idx); //return the representitive
    void unionSets(int u, int v);
    
    int size(int idx){ return nodes[idx].size; }
    
    int numOfSet(){ return setNum; }
    
    inline void assignMSTW(int idx, float w){ nodes[idx].MSTweight = w; }
    inline float weight(int idx){ return nodes[idx].MSTweight; }
    
    inline myDJSNode& operator[](const int idx){ return nodes[idx]; }
    inline myDJSNode& operator[](const int idx) const { return nodes[idx]; }
    
private:
    myDJSNode* nodes;
    int setNum; //the number of disjoint sets
    
};




myDisjointSet::myDisjointSet(int num){
    nodes = new myDJSNode[num];
    setNum = num;
    //make set
    for (int n = 0; n < setNum; n++) {
        nodes[n].size = 1;
        nodes[n].rank = 0;
        nodes[n].p = n; // make every node a disjoint set
        nodes[n].MSTweight = 0;
    }
}


int myDisjointSet::findSet(int idx){
    
    if (nodes[idx].p == idx) {
        return idx;
    }
    
    vector<int> A; //empty set
    int c_idx = idx; //current idx
    
    while (nodes[c_idx].p != c_idx) { //current node is not the root
        A.push_back(c_idx);
        c_idx = nodes[c_idx].p;
    }
    
    //Path compression
    for (vector<int>::iterator it = A.begin(); it != A.end(); it++) {
        nodes[(*it)].p = c_idx;
    }
    
    return c_idx;
}

void myDisjointSet::unionSets(int u, int v){
    int uParent = findSet(u);
    int vParent = findSet(v);
    
    
    if (nodes[uParent].rank == nodes[vParent].rank) {
        nodes[uParent].rank = nodes[uParent].rank + 1;
        //merge v into u
        nodes[vParent].p = uParent;
        nodes[uParent].size += nodes[vParent].size;
    } else if (nodes[uParent].rank > nodes[vParent].rank) {
        //merge v into u
        nodes[vParent].p = uParent;
        nodes[uParent].size += nodes[vParent].size;
    } else{
        //merge u into v
        nodes[uParent].p = vParent;
        nodes[vParent].size += nodes[uParent].size;
    }
    // # of disjoint set -1
    setNum = setNum - 1;
}



#endif /* defined(__DIP_FinalProject__myDisjointSet__) */
