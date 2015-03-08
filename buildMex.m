%% Build Mex Files

delete *.mex*
delete *.lib*
delete *.exp*
delete *.txt*

% mex ./1_buildGraph/eightNeighborGridGraph.cpp
mex ./1_buildGraph/buildGraph8N3C.cpp
mex ./1_buildGraph/buildGraph8N1C.cpp
mex ./2_segmentation/SegGraph.cpp
mex ./3_seg2color/Seg2Color.cpp