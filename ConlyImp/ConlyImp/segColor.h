//
//  segColor.h
//  ConlyImp
//
//  Created by StevenM on 3/7/15.
//  Copyright (c) 2015 StevenM. All rights reserved.
//

#ifndef ConlyImp_segColor_h
#define ConlyImp_segColor_h
#include <unordered_map>
using namespace std;

#define CMAX 255

size_t Seg2Color(Mat &img, double* segMap, uint imW, uint imH){
    unordered_map<double,RGB> mymap;
    unordered_map<double,RGB>::iterator it;
    
    double tR;
    double tG;
    double tB;
    
    double cNode;
    
    for(uint y = 0; y < imW; y++){
        for(uint x = 0; x < imH; x++){
            cNode = (double)segMap[x+imH*y];
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
            //Assign to Image
            Vec3b pix;
            
            // First pixel
            pix.val[0]=tR;
            pix.val[1]=tG;
            pix.val[2]=tB;
            img.at<Vec3b>(y,x)=pix;
        }
    }
    size_t temp = mymap.size();
    mymap.clear();
    return temp;
}

#endif
