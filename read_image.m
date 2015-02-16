% input image
rgb = imread('lena512color.tiff');

[m,n] = size(rgb(:,:,1));

figure,
imshow(rgb)

%gaussian filter
sigma = 0.8;
hsize = 20;
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

%control parameters
writefile = 1;

[edgeWeights, vertices] = build_8N_GridGraph_RGB(blured(:,:,1),blured(:,:,2),blured(:,:,3),1,1,1);
str = './graph/lena512_08_';
%convert the array index 0 to 1
vertices = vertices + 1;

edgeWeights = abs(edgeWeights);

[sortedW,sortedIdx] = sort(edgeWeights);

if writefile == 1
    %output file
    fSortIdx = fopen(strcat(str,'sortIdx.txt'),'w');
    fCNodes = fopen(strcat(str,'currentNodes.txt'),'w');
    fNNodes = fopen(strcat(str,'neighborNodes.txt'),'w');
    fWeights = fopen(strcat(str,'weights.txt'),'w');
    %convert the index from 1 to 0
    vertices = vertices - 1;
    sortedIdx = sortedIdx - 1;
    
    for idx = 1: length(sortedIdx)
        fprintf(fSortIdx,'%d ', sortedIdx(idx));
        fprintf(fCNodes,'%d ', vertices(1,idx));
        fprintf(fNNodes,'%d ', vertices(2,idx));
        fprintf(fWeights,'%f ',edgeWeights(idx));
    end
    fclose(fWeights);
    fclose(fSortIdx);
    fclose(fCNodes);
    fclose(fNNodes);
end
