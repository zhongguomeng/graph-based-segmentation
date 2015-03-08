//
//  buildGraph.h
//  ConlyImp
//
//  Created by StevenM on 3/4/15.
//  Copyright (c) 2015 StevenM. All rights reserved.
//

#ifndef ConlyImp_buildGraph_h
#define ConlyImp_buildGraph_h

/*
 * Build eight nieghbor grid graph
 *
 * Input : a three-channel imH x imW image
 * Outputs  : edge weights and end vertices
 *
 * The calling syntax is:
 *
 <<<<<<< HEAD:build_8N_GridGraph_RGB.cpp
 *		[edgeWeights, vertices] = build_8N_GridGraph_RGB(R,G,B,w1,w2,w3);
 =======
 *		[edgeWeights, vertices] = eightNeighborGridGraph(R,G,B,w1,w2,w3);
 >>>>>>> ba306a3c3d3ea1a60d4fa880ce79f241a7951339:eightNeighborGridGraph.cpp
 *      R,G,B : in general it can be any three channels for images
 *      w1,w2,w3 : differnet weights for measuring the difference
 *
 * This is a MEX-file for MATLAB.
 */

#include <math.h>

static inline float diff(int x1, int y1, int x2, int y2,
                         Mat &img, double w1, double w2, double w3) {
    uchar R1,R2,G1,G2,B1,B2;
    Vec3b pix;
    
    // First pixel
    pix=img.at<Vec3b>(y1,x1);
    R1=pix.val[0];
    G1=pix.val[1];
    B1=pix.val[2];
    
    // Second pixel
    pix=img.at<Vec3b>(y2,x2);
    R2=pix.val[0];
    G2=pix.val[1];
    B2=pix.val[2];
    
    float Rd = R1 - R2;
    float Gd = G1 - G2;
    float Bd = B1 - B2;
    return sqrt(w1*Rd*Rd+w2*Gd*Gd+w3*Bd*Bd);
}


void eightNeighborGridGraph(uint* edgeWeight,
                            vector<pair<uint, uint> > &vertices, Mat &img, double w1=1, double w2=1, double w3=1){
    
    //Allocating memory for the graph
    uint imW=img.cols;
    uint imH=img.rows;
    
    uint num = 0;
    
    for(uint y = 0; y < imW; y++){
        for(uint x = 0; x < imH; x++){

            //Connect vertical edges
            if(x < imH - 1){
                //edgeWeight[num] = diff(x,y,x+1,y,imH,R, G, B, w1, w2, w3);
                edgeWeight[num]= diff(x,y,x+1,y,img, w1, w2, w3);
                vertices.push_back(make_pair(x + y*imH, x+1 + y*imH));
                num++;
            }

            //Connect horizontal edges
            if (y < imW-1){
                edgeWeight[num] = diff(x,y,x,y+1,img,w1, w2, w3);
                vertices.push_back(make_pair(x + y*imH, x + (y+1)*imH));
                num++;
            }
            
            //Connect up-left to down-left edges
            if(x < imH -1 && y < imW-1){
                edgeWeight[num] = diff(x,y,x+1,y+1,img, w1, w2, w3);
                vertices.push_back(make_pair(x + y*imH, (x+1) + (y+1)*imH));
                num++;
            }
            
            //Connect down-left to up-right edges
            if(x > 0 && y < imW-1){
                edgeWeight[num] = diff(x,y,x-1,y+1,img, w1, w2, w3);
                vertices.push_back(make_pair(x + y*imH, (x-1) + (y+1)*imH));
                num++;
            }
            
        }
    }
}



#endif
