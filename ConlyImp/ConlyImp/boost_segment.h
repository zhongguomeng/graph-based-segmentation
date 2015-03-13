//
//  boost_segment.h
//  ConlyImp
//
//  Created by StevenM on 3/12/15.
//  Copyright (c) 2015 StevenM. All rights reserved.
//

#ifndef ConlyImp_boost_segment_h
#define ConlyImp_boost_segment_h

typedef std::vector<int> VecInt;

void B_Segmentation(int, int, double*,
             uint*, edge_list, int);

void B_Segmentation(int nNodes, int nEdges, double * segMap,
             uint * edgeWeight, edge_list vertices, int k)
{
    VecInt rank (100);
    VecInt parent (100);
    boost::disjoint_sets<int*,int*> ds(&rank[0], &parent[0]);
}

#endif
