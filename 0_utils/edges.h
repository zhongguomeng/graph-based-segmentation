//
//  edges.h
//  DIP_FinalProject
//
//  Created by Lin Weiting on 2015/2/8.
//  Copyright (c) 2015年 Weiting. All rights reserved.
//

#ifndef DIP_FinalProject_edges_h
#define DIP_FinalProject_edges_h

#include "typeUtility.h"

class edges{
public:
    friend bool operator<(const edges& left, const edges& right);
    friend bool operator>(const edges& left, const edges& right);
    friend bool operator==(const edges& left, const edges& right);
    friend bool operator!=(const edges& left, const edges& right);
    float w;
    int a,b;
};

inline bool operator< (const edges& left, const edges& right){
    return left.w < right.w;
}

inline bool operator> (const edges& left, const edges& right){
    return left.w > right.w;
}

inline bool operator== (const edges& left, const edges& right){
    return left.w == right.w;
}

inline bool operator!= (const edges& left, const edges& right){
    return left.w == right.w;
}



#endif
