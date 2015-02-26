function [edgeWeights, vertices] = GridGraph_8N(I)
[imH, imW] = size(I);
outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);
edgeWeights = zeros(1,outArraySize);
vertices = zeros(2,outArraySize);

    function d = diff(x1,y1,x2,y2)
        d = I(x1+(y1-1)*imH) - I(x2+(y2-1)*imH);
    end

num = 1;
for x = 1:imH
    for y = 1:imW
        
        %Connect vertical edges
        if(x < imH)
            edgeWeights(num) = diff(x,y,x+1,y);
            vertices(2*num-1) = x + (y-1)*imH;
            vertices(2*num) = x+1 + (y-1)*imH;
            num = num +1;
        end
        
        %Connect horizontal edges
        if (y < imW)
            edgeWeights(num) = diff(x,y,x,y+1);
            vertices(2*num-1) = x + (y-1)*imH;
            vertices(2*num) = x + y*imH;
            num = num + 1;
        end
        
        %Connect up-left to down-left edges
        if(x < imH && y < imW)
            edgeWeights(num) = diff(x,y,x+1,y+1);
            vertices(2*num-1) = x + (y-1)*imH;
            vertices(2*num) = (x+1) + y*imH;
            num = num + 1;
        end
        
        %Connect down-left to up-right edges
        if(x > 1 && y < imW)
            edgeWeights(num) = diff(x,y,x-1,y+1);
            vertices(2*num-1) = x + (y-1)*imH;
            vertices(2*num) = (x-1) + y*imH;
            num = num + 1;
        end
        
    end
end


end