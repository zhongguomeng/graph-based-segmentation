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
#include <array>

// Performance measurements
#include <time.h>

using namespace cv;
using namespace std;

typedef pair<uint, uint> edge_list;

#include "buildGraph.h"
#include "Segment.h"
#include "segColor.h"


int main( int argc, char** argv )
{
    float sigma=0.8;
    short k=200;
    cout << argc << endl;
    if( argc < 2)
    {
        cout <<" Usage: display_image ImageToLoadAndDisplay [k] [sigma]" << endl;
        return -1;
    }
    else if( argc == 3)
    {
        k=atoi(argv[2]);
    }
    else if (argc == 4)
    {
        k=atoi(argv[2]);
        sigma=atof(argv[3]);
    }
    
    
    
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    clock_t t_total,t;
    GaussianBlur( image, image, Size( 0, 0 ), sigma, sigma );
    
    t_total=clock();
    // Build the graph,
    uint* edgeWeight;
    //Allocate memory for graph
    //Allocating memory for the graph
    uint imW=image.rows;
    uint imH=image.cols;
    uint outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
    
    edgeWeight = new uint [outArraySize];
    
    t=clock();
    edge_list *vertices;
    vertices= new edge_list [outArraySize];
    eightNeighborGridGraph(edgeWeight,vertices, image,1,1,1);
    t=clock()-t;
    printf ("Building graph took me %d clicks (%f seconds).\n",(int)t,((float)t)/CLOCKS_PER_SEC);
    
    // Return structure,
    double* segMap;
    segMap = new double [outArraySize];
    
    t=clock();
    Segmentation(imW*imH, outArraySize, segMap,
                 edgeWeight, vertices, k);
    t=clock()-t;
    printf ("Segmentation took me %d clicks (%f seconds).\n",(int)t,((float)t)/CLOCKS_PER_SEC);
    
    Seg2Color(image, segMap, imW, imH);
    t_total = clock() - t_total;
    printf ("In total, it took me %d clicks (%f seconds).\n",(int)t_total,((float)t_total)/CLOCKS_PER_SEC);
    
    imwrite("./output.jpg",image);
    
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.
    
    waitKey(0);                                          // Wait for a keystroke in the window
    
    
    return 0;
}