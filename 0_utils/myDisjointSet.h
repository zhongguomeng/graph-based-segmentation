//
//  myDisjointSet.h
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/7.
//  Copyright (c) 2015年 Weiting. All rights reserved.
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
    int maxNum;
};





#endif /* defined(__DIP_FinalProject__myDisjointSet__) */
