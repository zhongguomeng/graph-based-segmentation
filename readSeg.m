m = 512;
n = 512;
num = m*n;
%read string
fname = './output/lena512_08_462_mySeg';
RN = 'R.txt';
GN = 'G.txt';
BN = 'B.txt';

%read segmentation
fRid = fopen(strcat(fname,RN));
mySegR = fscanf(fRid,'%f');

fGid = fopen(strcat(fname,GN));
mySegG = fscanf(fGid,'%f');

fBid = fopen(strcat(fname,BN));
mySegB = fscanf(fBid,'%f');

fclose(fRid);
fclose(fGid);
fclose(fBid);

SegImg = zeros(m,n,3);
SegImg(:,:,1) = reshape(mySegB,[m,n]);
SegImg(:,:,2) = reshape(mySegR,[m,n]);
SegImg(:,:,3) = reshape(mySegG,[m,n]);
figure,
imshow(uint8(SegImg))


