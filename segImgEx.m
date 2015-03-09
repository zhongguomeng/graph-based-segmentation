% input image
rgb = imread('./0_testImages/dog.ppm');
t=tic();

[m,n] = size(rgb(:,:,1));

subplot(1,2,1);
imshow(rgb);

%
K = 200;
minSize = 400;
forceMerge = 1;

%gaussian filter
sigma = 0.8;
hsize = ceil(4*sigma);

% Tianchen start, 2015/2/25
gaussian = fspecial('gaussian',[hsize,hsize], sigma);
blurred=imfilter(double(rgb), gaussian);
% Tianchen end

%build graph
[edgeWeights, vertice1, vertice2] = buildGraph8N3C(blurred(:,:,1),blurred(:,:,2),blurred(:,:,3),1,1,1);

%convert the array index 0 to 1
vertice1 = vertice1 + 1;
vertice2 = vertice2 + 1;

edgeWeights = abs(edgeWeights);

[sortedW,sortedIdx] = sort(edgeWeights);

%build segmentation

%convert the array index from 1 to 0
vertice1 = vertice1 - 1;
vertice2 = vertice2 - 1;
sortedIdx = sortedIdx - 1;

[mySegR, mySegG, mySegB, numSeg] = SegGraph(edgeWeights, vertice1, vertice2,...
                                                m,n,length(edgeWeights), K, minSize, forceMerge);

SegImg = zeros(m,n,3);
SegImg(:,:,1) = reshape(mySegB,[m,n]);
SegImg(:,:,2) = reshape(mySegR,[m,n]);
SegImg(:,:,3) = reshape(mySegG,[m,n]);
t=toc(t);

disp(['Completed in ', num2str(t), ' secs']);

% Tianchen start, 2015/2/25
subplot(1,2,2);
imshow(SegImg/255);
set(gcf,'OuterPosition',[100,100,1200,600]);
% Tianchen end