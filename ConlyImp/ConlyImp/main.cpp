//
//  main.cpp
//  ConlyImp
//
//  Created by StevenM on 3/4/15.
//  Copyright (c) 2015 StevenM. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

// Performance measurements
#include <time.h>

using namespace cv;
using namespace std;

typedef vector<pair<uint, uint> > edge_list;

#include "buildGraph.h"
#include "Segment.h"
#include "segColor.h"


int main( int argc, char** argv )
{
    if( argc != 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
        return -1;
    }
    
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    GaussianBlur( image, image, Size( 0, 0 ), 0.8, 0.8 );
    
    // Build the graph,
    uint* edgeWeight;
    //Allocate memory for graph
    //Allocating memory for the graph
    uint imW=image.cols;
    uint imH=image.rows;
    uint outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
    
    cout << "Number of edges: "<< outArraySize << endl;
    
    edgeWeight = new uint [outArraySize];
    
    
    vector<pair<uint, uint> > vertices;
    eightNeighborGridGraph(edgeWeight,vertices, image,1,1,1);
    
    // Return structure,
    double* segMap;
    segMap = new double [outArraySize];
    
    Segmentation(outArraySize, 2*outArraySize, segMap,
                 edgeWeight, vertices, 500);
    
    Seg2Color(image, segMap, imW, imH);
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    
    waitKey(0);                                          // Wait for a keystroke in the window
    
    
    return 0;
}