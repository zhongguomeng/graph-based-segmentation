package com.example.android.photobyintent;

import android.graphics.Bitmap;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.android.Utils;

/**
 * Created by weitingco on 15/2/27.
 */
public class myImageHandle {
    Mat matImg;
    //Bitmap image;
    Bitmap segmentation;
    int CMAX = 256;
    //segmentation factors
    double sigma = 0.8;
    int ks = 5;
    int C = 200;
    //For graph
    float weights [];
    int leftNodes [];
    int rightNodes [];

    public native int[] ImageSegmentation(float[] w, int[] l, int[] r, float K, int x, int y);

    void setSigma(double s){
        if(s > 0) {
            sigma = s;
        }
    }

    void setKSize(int k){
        if(k > 0) {
            ks = k;
        }
    }

    void setC(int c){
        if(c>0){
            C = c;
        }
    }

    void setImage(Bitmap input){

        //CV_32F -> float[]
        matImg = new Mat(input.getHeight(), input.getWidth(), CvType.CV_32FC3);
        Mat matTemp2 = new Mat(input.getHeight(), input.getWidth(), CvType.CV_32FC3);
        //CV_8U -> byte[]
        Mat matTemp = new Mat(input.getHeight(), input.getWidth(), CvType.CV_8UC3);
        Utils.bitmapToMat(input,matTemp);

        Size ksize = new Size(ks,ks);

        Imgproc.cvtColor(matTemp, matTemp2, Imgproc.COLOR_RGB2Lab);

        Imgproc.GaussianBlur(matTemp2, matImg, ksize, sigma);

        matTemp.release();
        matTemp2.release();
    }

    Mat multiply(Mat a, Mat b){
        int s = a.rows();
        Mat A = new Mat(s,s,CvType.CV_32F);
        double[] e1;
        double[] e2;
        float [] r = new float[1];
        for(int x = 0; x < s; x++){
            e1 = a.get(x,0);
            for (int y = 0; y <s; y++){
               e2 = b.get(y,0);
               r[0] = (float)(e1[0]*e2[0]);
               A.put(x,y,r);
            }
        }
        return A;
    }

    Bitmap getSegmentation(){
        segmentation = Bitmap.createBitmap(matImg.cols(),matImg.rows(), Bitmap.Config.ARGB_8888);
        build8NGraph(1,1,1);
        int [] segIdx = ImageSegmentation(weights, leftNodes, rightNodes,
                                           C, matImg.rows(), matImg.cols());
        //release memory
        weights = null;
        leftNodes = null;
        rightNodes = null;

        seg2Color(segIdx);

        Utils.matToBitmap(matImg,segmentation);

        return segmentation;
    }


    void seg2Color(int [] segIdx){
        Map<Integer, RGB> colorMap = new HashMap<Integer, RGB>();
        int imH = matImg.rows();
        int imW = matImg.cols();
        int num = imH*imW;
        matImg = new Mat(imH, imW, CvType.CV_8UC3);

        Random rand = new Random();
        int cNode;
        byte [] C = new byte[3];

        for (int idx = 0; idx < num; idx++){
            cNode = segIdx[idx];
            if(colorMap.get(cNode) == null){ //new segmentation
                RGB rgb = new RGB();

                rgb.R = (byte)rand.nextInt(CMAX);
                rgb.B = (byte)rand.nextInt(CMAX);
                rgb.G = (byte)rand.nextInt(CMAX);

                C[0] = rgb.R;
                C[1] = rgb.B;
                C[2] = rgb.G;

                colorMap.put(cNode,rgb);
            } else {
                RGB rgb = colorMap.get(cNode);
                C[0] = rgb.R;
                C[1] = rgb.B;
                C[2] = rgb.G;
            }
            int rowNum = idx%imH;
            int colNum = (idx-rowNum)/imH;
            matImg.put(rowNum,colNum,C);
        }

    }


    float diff(int x1, int y1, int x2, int y2, float w1, float w2, float w3){
        double[] p1 = matImg.get(x1,y1);
        double[] p2 = matImg.get(x2,y2);
        float d1 = (float) ((p1[0]-p2[0])*(p1[0]-p2[0]));
        float d2 = (float) ((p1[1]-p2[1])*(p1[1]-p2[1]));
        float d3 = (float) ((p1[2]-p2[2])*(p1[2]-p2[2]));
        return w1*d1+w2*d2+w3*d3;
    }


    void build8NGraph(float w1, float w2, float w3){
        int imW = matImg.cols();
        int imH = matImg.rows();
        int outArraySize = (imH-1)*imW + (imW-1)*imH + 2*(imH-1)*(imW-1);

        weights = new float[outArraySize];
        leftNodes = new int[outArraySize];
        rightNodes = new int[outArraySize];

        int num = 0;

        for(int y = 0; y < imW; y++){
            for(int x = 0; x < imH; x++){

                //Connect vertical edges
                if(x < imH - 1){
                    weights[num] = diff(x,y,x+1,y,w1,w2,w3);
                    leftNodes[num] = x + y*imH;
                    rightNodes[num] = x+1 + y*imH;
                    num++;
                }

                //Connect horizontal edges
                if (y < imW-1){
                    weights[num] = diff(x,y,x,y+1,w1,w2,w3);
                    leftNodes[num] = x + y*imH;
                    rightNodes[num] = x + (y+1)*imH;
                    num++;
                }

                //Connect up-left to down-left edges
                if(x < imH -1 && y < imW-1){
                    weights[num] = diff(x,y,x+1,y+1,w1,w2,w3);
                    leftNodes[num] = x + y*imH;
                    rightNodes[num] = (x+1) + (y+1)*imH;
                    num++;
                }

                //Connect down-left to up-right edges
                if(x > 0 && y < imW-1){
                    weights[num] = diff(x,y,x-1,y+1,w1,w2,w3);
                    leftNodes[num] = x + y*imH;
                    rightNodes[num] = (x-1) + (y+1)*imH;
                    num++;
                }

            }
        }

    }

}
