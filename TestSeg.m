%% Initialize
% input image
<<<<<<< HEAD
tic;

%load image
fileName = 'postjp';
filePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/',fileName,'.png');

rgb = imread(filePath);
=======
rgb = imread('./0_testImages/dog.jpg');
>>>>>>> a3a0579021a02c340e1769e7b8fa675cf18d46ef

[m,n] = size(rgb(:,:,1));

subplot(1,2,1);
imshow(rgb);

tic;
%image segmentation parameters
K = 200;
minSize = 100;
forceMerge = 1;

%gaussian filter
sigma = 4;
hsize = 2*[sigma, sigma];%[4 4]; % same as Felzenszwalb's

% Tianchen start, 2015/2/25
gaussian = fspecial('gaussian',hsize, sigma);
blurred=imfilter(double(rgb), gaussian);
% Tianchen end

%% diff = sqrt(RR+BB+GG)
%build graph
% weight == sqrt(RR+GG+BB)
[edgeWeights, vertices1, vertices2] = buildGraph8N3C(blurred(:,:,1),blurred(:,:,2),blurred(:,:,3),1,1,1);

%convert the array index 0 to 1
vertices1 = vertices1 + 1;
vertices2 = vertices2 + 1;

edgeWeights = abs(edgeWeights);

[sortedW,sortedIdx] = sort(edgeWeights,'ascend');

%convert the array index from 1 to 0
vertices1 = vertices1 - 1;
vertices2 = vertices2 - 1;
sortedIdx = sortedIdx - 1;

%build segmentation
segMap = SegGraph(edgeWeights, vertices1, vertices2,m,n,length(edgeWeights), K, minSize, forceMerge);
                                           
[mySegR, mySegG, mySegB, numSeg] = Seg2Color(segMap, m,n);
SegImg = zeros(m,n,3);
SegImg(:,:,1) = reshape(mySegB,[m,n]);
SegImg(:,:,2) = reshape(mySegR,[m,n]);
SegImg(:,:,3) = reshape(mySegG,[m,n]);

toc;
% Tianchen start, 2015/2/25
subplot(1,2,2);
imshow(uint8(SegImg));
set(gcf,'OuterPosition',[100,100,1200,600]);
% Tianchen end
<<<<<<< HEAD
toc;

%save image
% save current segmentation
myFilePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/my_',fileName,'.png');
imwrite(uint8(SegImg),myFilePath);
=======
>>>>>>> a3a0579021a02c340e1769e7b8fa675cf18d46ef
%% seg == Intersect(Rseg,Gseg,Bseg)