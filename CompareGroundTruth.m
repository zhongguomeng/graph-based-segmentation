%load file
fileName = 'redberry_rb03';
gtN = '38';
filePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/',fileName,'_',gtN,'.png');

%set the representive coordinate
rep_xy = [235,100];%[159, 137];%[258, 103];
author = 0;
%load ground truth
gt = imread(filePath);
[m,n,~] = size(gt);

gt_T = zeros(m,n);
%groud truth color
gt_color = [255, 0, 0];

for x = 1:m
    for y = 1:n
        temp = gt(x,y,:);
        temp = reshape(temp,1,3);
        if(sum(temp == gt_color) == 3)
            gt_T(x,y) = 1;
        end
    end
end

if author ~=1
    %our segmentation
    myFilePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/my_',fileName,'.png');
else
    myFilePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/author_',fileName,'.png');
end
mySeg = imread(myFilePath);

mySeg_T = zeros(m,n);
%get color
rep_color = reshape(mySeg(rep_xy(1),rep_xy(2),:),1,3);

%overlap area
overlap = 0;

for x = 1:m
    for y = 1:n
        temp = mySeg(x,y,:);
        temp = reshape(temp,1,3);
        if(sum(temp == rep_color) == 3)
            mySeg_T(x,y) = 1;
            
            %precision
            if gt_T(x,y) == 1
                overlap = overlap + 1;
            end
            
        end
        
    end
end
figure,
imshow(mySeg_T);

% accuracy = 1 - (false + miss)/(actrual volumn)
accuracy = 1 - sum(sum(abs(mySeg_T-gt_T)))/sum(sum(gt_T));

% F-measure
precision = overlap/sum(sum(mySeg_T));
recall = overlap/sum(sum(gt_T));

F = 2*precision*recall/(precision+recall);
disp(F)

if author == 0
    myBinFilePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/myBin_',fileName,'.png');
else
    myBinFilePath = strcat('./11_ImagesWithGroundTruth/',fileName,'/author_Bin_',fileName,'.png');
end
imwrite(mySeg_T,myBinFilePath)
