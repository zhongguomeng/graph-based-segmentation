# graph-based-segmentation
THIS is EVILLLLLLLLL

# Matlab Workflow

1. Run read_image.m to read the image and construct the graph (edges). The graph is stored in txt files. 
2. Run the cpp file (meximgSegment.cpp). The segmented images (3 channels) are stored in .txt files (which stored as "imageName"_"NumberOfSegmentation"_mySeg"Channel".) In addition, the raw segmentation file is stored (pixel to segmentation index).
3. Run readSeg.m to visualize the result. 

## TODO
### fix MATLAB interface: compile multiple .cpp files to mex and bypass .txt files [Steven]
### Android implementation

-Interface

-Algorithm [Weiting]

### Graph construction in different space

-Nearest neighbor search in (x,y,r,g,b) [TJ]

-In other spaces

### Comparison with other implemenations
### Think about video segmentation (using time to help)
~~### Prepare slides for presentation (BY TUESDAY!!!!) [Steven]~~


## Maybe do
-use boost for disjoint set
-shape recognition for object tracking

http://download.springer.com/static/pdf/25/art%253A10.1007%252Fs11263-006-7934-5.pdf?auth66=1424049572_e94520cf58dfcdc9db07e7f5f0ad4b8d&ext=.pdf
