%% Build Mex Files

delete *.mex*

mex ./1_buildGraph/eightNeighborGridGraph.cpp
mex ./2_segmentation/SegGraph.cpp
