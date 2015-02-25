% input image
rgb = imread('lena512color.tiff');

[m,n] = size(rgb(:,:,1));

figure,
imshow(rgb)

%gaussian filter
sigma = 0.8;
hsize = ceil(4*sigma);
%h = fspecial('gaussian', hsize, sigma);
h = GaussianKernel(sigma,hsize);

% padding

rgb_pad = padarray(rgb,[hsize hsize],'symmetric');

% How do you do 2D convolution, this parts take like forever...

outR = conv2(double(rgb_pad(:,:,1)),h,'same');
outG = conv2(double(rgb_pad(:,:,2)),h,'same');
outB = conv2(double(rgb_pad(:,:,3)),h,'same');

outR = outR(hsize+1:hsize+m,hsize+1:hsize+n);
outG = outG(hsize+1:hsize+m,hsize+1:hsize+n);
outB = outB(hsize+1:hsize+m,hsize+1:hsize+n);

blured = zeros(m,n,3);
blured(:,:,1) = outR;
blured(:,:,2) = outG;
blured(:,:,3) = outB;
figure,
imshow(uint8(blured))


%build graph
[edgeWeights, vertices] = eightNeighborGridGraph(blured(:,:,1),blured(:,:,2),blured(:,:,3),1,1,1);

%convert the array index 0 to 1
vertices = vertices + 1;

edgeWeights = abs(edgeWeights);

[sortedW,sortedIdx] = sort(edgeWeights);

%build segmentation

%convert the array index from 1 to 0
vertices = vertices - 1;
sortedIdx = sortedIdx - 1;

[mySegR, mySegG, mySegB, numSeg] = SegGraph(edgeWeights, vertices(1,:), vertices(2,:),...
                                                m,n,length(edgeWeights), 200);

SegImg = zeros(m,n,3);
SegImg(:,:,1) = reshape(mySegB,[m,n]);
SegImg(:,:,2) = reshape(mySegR,[m,n]);
SegImg(:,:,3) = reshape(mySegG,[m,n]);
figure,
imshow(uint8(SegImg))
