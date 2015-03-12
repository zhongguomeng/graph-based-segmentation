//
//  myDisjointSet.cpp
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015å¹?Weiting. All rights reserved.
//


#include "myDisjointSet.h"


myDisjointSet::myDisjointSet(int num){
    nodes = new myDJSNode[num];
    setNum = num;
    maxNum = num;
    //make set
    for (int n = 0; n < setNum; n++) {
        nodes[n].size = 1;
        nodes[n].rank = 0;
        nodes[n].p = n; // make every node a disjoint set
        nodes[n].MSTweight = 0;
    }
	A.reserve(num);
}


int myDisjointSet::findSet(int idx){
    
    if (nodes[idx].p == idx) {
            return idx;
    }
	//vector<int> A;

    int c_idx = idx; //current idx
    
    /** PATH COMPRESSION **/
    //collect nodes for path compression
    while (nodes[c_idx].p != c_idx) { //current node is not the root
		A.push_back(c_idx);
        //A[ptr] = c_idx;
        //ptr = ptr + 1;
        c_idx = nodes[c_idx].p;
    }

	//Path compression
	for (vector<int>::iterator it = A.begin(); it != A.end(); it++) {
		nodes[(*it)].p = c_idx;
	}
    /**END PATH COMPRESSION **/
    
    
    //without path compression
    //nodes[idx].p = c_idx;
    
    A.clear();
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
