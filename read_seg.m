% input image
rgb = imread('pears.png');
% rgb to gray
I = rgb2gray(rgb);

[m,n] = size(I);
num = m*n;

%read segmentation
fid = fopen('test_myTestSeg.txt');
mySeg = fscanf(fid,'%f');

maxV = max(mySeg);

SegRGB = zeros(m,n,3);

[SegImgR,SegImgG,SegImgB, nSeg] = mapSegIdx(mySeg);
SegImgR = reshape(SegImgR,[m,n]);
SegImgG = reshape(SegImgG,[m,n]);
SegImgB = reshape(SegImgB,[m,n]);

SegRGB(:,:,1) = SegImgR;
SegRGB(:,:,2) = SegImgG;
SegRGB(:,:,3) = SegImgB;
figure,
imshow(uint8(SegRGB))

% mySeg = reshape(mySeg,[m,n]);
% imshow(mySeg/maxV)

