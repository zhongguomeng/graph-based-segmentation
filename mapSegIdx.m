function [imgR,imgG,imgB, setNum] = mapSegIdx(raw)
img_size = length(raw);
imgR = zeros(1,img_size);
imgG = zeros(1,img_size);
imgB = zeros(1,img_size);
setNum =[];
acc = 0;
for idx = 0:img_size-1
    idxset = raw == idx;
    tempnum = sum(idxset);
    if(tempnum ~= 0)
        %random color 
        ranC = ceil(rand*255);
        setNum = [setNum ranC];
        imgR(idxset) = ranC;
        ranC = ceil(rand*255);
        imgG(idxset) = ranC;
        ranC = ceil(rand*255);
        imgB(idxset) = ranC;
    end
    
    acc = acc+tempnum;
    
    if acc == img_size
        break;
    end
        
end