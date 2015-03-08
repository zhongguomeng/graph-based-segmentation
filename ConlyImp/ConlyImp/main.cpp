//
//  main.cpp
//  ConlyImp
//
//  Created by StevenM on 3/4/15.
//  Copyright (c) 2015 StevenM. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#include "buildGraph.h"

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
    
    // Build the graph,
    uint* edgeWeight;
    uint* vertices;
    eightNeighborGridGraph(edgeWeight,vertices, image,1,1,1);
    
    //namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    //imshow( "Display window", image );                   // Show our image inside it.
    
    //waitKey(0);                                          // Wait for a keystroke in the window
    
    
    return 0;
}