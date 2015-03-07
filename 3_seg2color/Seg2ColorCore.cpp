#include <map>
using namespace std;

#define CMAX 255

using namespace std;

typedef struct { double R, G, B; } RGB;

size_t Seg2Color(double* R, double* G, double*B, double* segMap, int len){
    map<double,RGB> mymap; 
    map<double,RGB>::iterator it;
    
    double tR;
    double tG;
    double tB;
    
    double cNode;
    
    for (int i = 0; i < len; ++i){
        cNode = (double)segMap[i];
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
    size_t temp = mymap.size();
    mymap.clear();
    return temp;
}