#include <jni.h>
#include <stdio.h>
#include "SegGraph.h"
#include <math.h>
#include <stdlib.h>
#include <android/log.h>
//#include "include/HelloJNI.h"

#define CMAX 255
typedef struct { int R, G, B; } RGB;

//size_t Seg2Color(int* R, int* G, int*B, float* segMap, int len);
/*
jint JNI_OnLoad(JavaVM* vm, void* reserved) { //cache objects
//create global references
return JNI_VERSION_1_6;
}


void JNI_OnUnload(JavaVM *vm, void *reserved);
*/

extern "C"{

void Segmentation(int node_num, int edge_num, int* segMap,
                 float* edgeW, int* l, int* r, float K);

//Utility
inline float max3(float a, float b, float c){
    float temp = ( (a > b) ? a:b );
    return ( (temp > c) ? temp:c );
}

float MInt(const myDJSNode& a, const myDJSNode& b, float K){
    float tempW1 = a.MSTweight + K/(a.size);
    float tempW2 = b.MSTweight + K/(b.size);

    return ( (tempW1 < tempW2) ? tempW1 : tempW2);
}

int compareEdges (const void * a, const void * b)
{
  if ( *(edges*)a <  *(edges*)b ) return -1;
  if ( *(edges*)a == *(edges*)b ) return 0;
  if ( *(edges*)a >  *(edges*)b ) return 1;
}


JNIEXPORT jintArray JNICALL Java_com_example_android_photobyintent_myImageHandle_ImageSegmentation
          (JNIEnv *env, jobject thisObj, jfloatArray w, jintArray l, jintArray r, float K, int x, int y) {

    __android_log_print(ANDROID_LOG_VERBOSE, "ImageSegmentation", "Start...");
    //parameters for segmentation
    jsize dlength = (env)->GetArrayLength(w);
    int num_nodes = x*y;

    //get array from java
    jint* jlNodes = (env)->GetIntArrayElements(l,NULL);
    jint* jrNodes = (env)->GetIntArrayElements(r,NULL);
    jfloat* jedgeW = (env)->GetFloatArrayElements(w,NULL);

    __android_log_print(ANDROID_LOG_VERBOSE, "ImageSegmentation", "Assignment done...");

    int* cNodes = jlNodes;
    int* nNodes = jrNodes;
    float* edgeW = jedgeW;

    jintArray result = (env)->NewIntArray(num_nodes);

    int* segMap = new int[num_nodes];

    __android_log_print(ANDROID_LOG_VERBOSE, "ImageSegmentation", "Start Seg...");

    Segmentation(num_nodes, dlength, segMap,
                    edgeW, cNodes, nNodes, K);

    // move from the temp structure to the java structure
    (env)->SetIntArrayRegion(result, 0, num_nodes, segMap);
    //delete [] mySeg;
    return result;
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



myDisjointSet* constructSegment(edges* graph, int num, float K, int edge_num){
    myDisjointSet* segmentGraph = new myDisjointSet(num);
    int scanIdx = 0;
    //assign internal difference
    for (int temp_idx; temp_idx < num; temp_idx++) {
        //singalton
        segmentGraph->assignMSTW(temp_idx, 0);
    }

    //std::sort(graph, graph + edge_num);
    qsort(graph, edge_num, sizeof(edges), compareEdges);

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
            }
        }


    } //end segmentation

    return segmentGraph;
}


void Segmentation(int node_num, int edge_num, int* segMap,
                 float* edgeW, int* l, int* r, float K){

    edges* Graph = constructGraph(edgeW, l, r, edge_num);

    myDisjointSet* Segments = constructSegment(Graph, node_num, K,edge_num);
    int rep;
    for (int idx = 0; idx < node_num; ++idx) {
        rep = Segments->findSet(idx);
        segMap[idx] = rep;
    }
}

/*
size_t Seg2Color(int* R, int* G, int*B, int* segMap, int len){
    std::map<int,RGB> mymap;
    std::map<int,RGB>::iterator it;

    int tR;
    int tG;
    int tB;

    int cNode;

    for (int i = 0; i < len; ++i){
        cNode = segMap[i];
        //haven't assigned the color
        it = mymap.find(cNode);
        if ( it == mymap.end() ){
            //assign the color
            tR = round(rand() % CMAX);
            tG = round(rand() % CMAX);
            tB = round(rand() % CMAX);

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
*/
}

